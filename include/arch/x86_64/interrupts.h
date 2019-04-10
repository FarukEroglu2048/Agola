#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

typedef struct interrupt_stack
{
    uint64_t gs;
    uint64_t fs;
    uint64_t es;
    uint64_t ds;

    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t idt_index;

    uint64_t error_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__ ((packed)) interrupt_stack_t;

typedef void (*interrupt_handler_t)(interrupt_stack_t* interrupt_stack);

extern void enable_interrupts();

void register_exception_handler(uint8_t index, interrupt_handler_t exception_handler);
void register_irq_handler(uint8_t index, interrupt_handler_t irq_handler);

void interrupts_initialize();

#endif