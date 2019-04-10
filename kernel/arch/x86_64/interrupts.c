#include <stddef.h>
#include <stdint.h>

#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>

#include <arch/x86_64/pic.h>

#include <arch/x86_64/interrupts.h>

extern void exception_handler_0();
extern void exception_handler_1();
extern void exception_handler_2();
extern void exception_handler_3();
extern void exception_handler_4();
extern void exception_handler_5();
extern void exception_handler_6();
extern void exception_handler_7();
extern void exception_handler_8();
extern void exception_handler_9();
extern void exception_handler_10();
extern void exception_handler_11();
extern void exception_handler_12();
extern void exception_handler_13();
extern void exception_handler_14();
extern void exception_handler_15();
extern void exception_handler_16();
extern void exception_handler_17();
extern void exception_handler_18();
extern void exception_handler_19();
extern void exception_handler_20();
extern void exception_handler_21();
extern void exception_handler_22();
extern void exception_handler_23();
extern void exception_handler_24();
extern void exception_handler_25();
extern void exception_handler_26();
extern void exception_handler_27();
extern void exception_handler_28();
extern void exception_handler_29();
extern void exception_handler_30();
extern void exception_handler_31();

extern void irq_handler_0();
extern void irq_handler_1();
extern void irq_handler_2();
extern void irq_handler_3();
extern void irq_handler_4();
extern void irq_handler_5();
extern void irq_handler_6();
extern void irq_handler_7();
extern void irq_handler_8();
extern void irq_handler_9();
extern void irq_handler_10();
extern void irq_handler_11();
extern void irq_handler_12();
extern void irq_handler_13();
extern void irq_handler_14();
extern void irq_handler_15();

interrupt_handler_t interrupt_handlers[256] = {NULL};

void register_exception_handler(uint8_t index, interrupt_handler_t exception_handler)
{
    if (index < 32) interrupt_handlers[index] = exception_handler;
}

void register_irq_handler(uint8_t index, interrupt_handler_t irq_handler)
{
    if (index < 16) interrupt_handlers[index + 32] = irq_handler;
}

void register_interrupt_handler(uint8_t index, interrupt_handler_t interrupt_handler)
{
    if (index >= 48) interrupt_handlers[index] = interrupt_handler;
}

void interrupts_initialize()
{
    idt_create_interrupt_entry(0, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_0));
    idt_create_interrupt_entry(1, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_1));
    idt_create_interrupt_entry(2, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_2));
    idt_create_interrupt_entry(3, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_3));
    idt_create_interrupt_entry(4, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_4));
    idt_create_interrupt_entry(5, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_5));
    idt_create_interrupt_entry(6, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_6));
    idt_create_interrupt_entry(7, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_7));
    idt_create_interrupt_entry(8, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_8));
    idt_create_interrupt_entry(9, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_9));
    idt_create_interrupt_entry(10, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_10));
    idt_create_interrupt_entry(11, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_11));
    idt_create_interrupt_entry(12, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_12));
    idt_create_interrupt_entry(13, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_13));
    idt_create_interrupt_entry(14, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_14));
    idt_create_interrupt_entry(15, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_15));
    idt_create_interrupt_entry(16, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_16));
    idt_create_interrupt_entry(17, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_17));
    idt_create_interrupt_entry(18, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_18));
    idt_create_interrupt_entry(19, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_19));
    idt_create_interrupt_entry(20, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_20));
    idt_create_interrupt_entry(21, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_21));
    idt_create_interrupt_entry(22, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_22));
    idt_create_interrupt_entry(23, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_23));
    idt_create_interrupt_entry(24, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_24));
    idt_create_interrupt_entry(25, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_25));
    idt_create_interrupt_entry(26, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_26));
    idt_create_interrupt_entry(27, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_27));
    idt_create_interrupt_entry(28, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_28));
    idt_create_interrupt_entry(29, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_29));
    idt_create_interrupt_entry(30, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_30));
    idt_create_interrupt_entry(31, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&exception_handler_31));

    pic_initialize(32, 40);

    idt_create_interrupt_entry(32, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_0));
    idt_create_interrupt_entry(33, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_1));
    idt_create_interrupt_entry(34, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_2));
    idt_create_interrupt_entry(35, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_3));
    idt_create_interrupt_entry(36, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_4));
    idt_create_interrupt_entry(37, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_5));
    idt_create_interrupt_entry(38, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_6));
    idt_create_interrupt_entry(39, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_7));
    idt_create_interrupt_entry(40, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_8));
    idt_create_interrupt_entry(41, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_9));
    idt_create_interrupt_entry(42, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_10));
    idt_create_interrupt_entry(43, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_11));
    idt_create_interrupt_entry(44, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_12));
    idt_create_interrupt_entry(45, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_13));
    idt_create_interrupt_entry(46, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_14));
    idt_create_interrupt_entry(47, DESCRIPTOR_PRIVILEGE_LEVEL_0, (uint64_t)(&irq_handler_15));

    enable_interrupts();
}

void kernel_interrupt_handler(interrupt_stack_t* interrupt_stack)
{
    uint8_t idt_index = interrupt_stack->idt_index;

    interrupt_handler_t interrupt_handler = interrupt_handlers[idt_index];

    if (idt_index < 32)
    {
        if (interrupt_handler != NULL)
        {
            interrupt_handler(interrupt_stack);
        }
        else
        {
            for (;;);
        }
    }

    else if (idt_index >= 32 && idt_index < 48)
    {
        pic_send_eoi((idt_index >= 40) ? true : false);

        if (interrupt_handler != NULL)
        {
            interrupt_handler(interrupt_stack);
        }
    }

    else if (interrupt_handler != NULL)
    {
        interrupt_handler(interrupt_stack);
    }
}