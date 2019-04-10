#include <stdint.h>

#include <arch/x86_64/idt.h>

#define IDT_ENTRY_COUNT 256

#define IDT_ENTRY_FLAG_PRESENT 0x80
#define IDT_ENTRY_FLAG_INTERRUPT_GATE 0x0E

typedef struct idt_register
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_register_t;

typedef struct idt_entry
{
    uint16_t base_low;

    uint16_t entry_code_segment;

    uint8_t always_zero_1;

    uint8_t flags;

    uint16_t base_mid;
    uint32_t base_high;

    uint32_t always_zero_2;
} __attribute__((packed)) idt_entry_t;

idt_register_t idt_register;

idt_entry_t idt_entries[IDT_ENTRY_COUNT];

extern void idt_load(uint64_t idt_register_address);

void idt_create_null_entry(uint8_t index)
{
    idt_entries[index].base_low = 0x0000;

    idt_entries[index].entry_code_segment = 0x00;

    idt_entries[index].always_zero_1 = 0x00;

    idt_entries[index].flags = 0x00;

    idt_entries[index].base_mid = 0x0000;
    idt_entries[index].base_high = 0x00000000;

    idt_entries[index].always_zero_2 = 0x00000000;
}

void idt_create_interrupt_entry(uint8_t index, uint8_t privilege_level, uint64_t base)
{
    idt_entries[index].base_low = (base & 0xFFFF);

    idt_entries[index].entry_code_segment = 0x08;

    idt_entries[index].always_zero_1 = 0x00;

    idt_entries[index].flags = IDT_ENTRY_FLAG_PRESENT | (privilege_level << 6) | IDT_ENTRY_FLAG_INTERRUPT_GATE;

    idt_entries[index].base_mid = ((base >> 16) & 0xFFFF);
    idt_entries[index].base_high = ((base >> 32) & 0xFFFFFFFF);

    idt_entries[index].always_zero_2 = 0x00000000;
}

void idt_initialize()
{
    for (uint16_t index = 0; index < IDT_ENTRY_COUNT; index++)
    {
        idt_create_null_entry(index);
    }

    idt_register.limit = (sizeof(idt_entries) - 1);
    idt_register.base = (uint64_t)(&idt_entries);

    idt_load((uint64_t)(&idt_register));
}