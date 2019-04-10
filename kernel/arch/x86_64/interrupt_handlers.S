.section .text

.macro create_exception_handler_no_error_code index
    .global exception_handler_\index
    .type exception_handler_\index, @function
    exception_handler_\index:
        push $0x0000000000000000
        
        push $\index

        jmp common_interrupt_handler
.endm

.macro create_exception_handler_error_code index
    .global exception_handler_\index
    .type exception_handler_\index, @function
    exception_handler_\index:
        push $\index

        jmp common_interrupt_handler
.endm

.macro create_irq_handler index idt_index
    .global irq_handler_\index
    .type irq_handler_\index, @function
    irq_handler_\index:
        push $0x0000000000000000

        push $\idt_index

        jmp common_interrupt_handler
.endm

create_exception_handler_no_error_code 0
create_exception_handler_no_error_code 1
create_exception_handler_no_error_code 2
create_exception_handler_no_error_code 3
create_exception_handler_no_error_code 4
create_exception_handler_no_error_code 5
create_exception_handler_no_error_code 6
create_exception_handler_no_error_code 7
create_exception_handler_error_code 8
create_exception_handler_no_error_code 9
create_exception_handler_error_code 10
create_exception_handler_error_code 11
create_exception_handler_error_code 12
create_exception_handler_error_code 13
create_exception_handler_error_code 14
create_exception_handler_no_error_code 15
create_exception_handler_no_error_code 16
create_exception_handler_error_code 17
create_exception_handler_no_error_code 18
create_exception_handler_no_error_code 19
create_exception_handler_no_error_code 20
create_exception_handler_no_error_code 21
create_exception_handler_no_error_code 22
create_exception_handler_no_error_code 23
create_exception_handler_no_error_code 24
create_exception_handler_no_error_code 25
create_exception_handler_no_error_code 26
create_exception_handler_no_error_code 27
create_exception_handler_no_error_code 28
create_exception_handler_no_error_code 29
create_exception_handler_error_code 30
create_exception_handler_no_error_code 31

create_irq_handler 0, 32
create_irq_handler 1, 33
create_irq_handler 2, 34
create_irq_handler 3, 35
create_irq_handler 4, 36
create_irq_handler 5, 37
create_irq_handler 6, 38
create_irq_handler 7, 39
create_irq_handler 8, 40
create_irq_handler 9, 41
create_irq_handler 10, 42
create_irq_handler 11, 43
create_irq_handler 12, 44
create_irq_handler 13, 45
create_irq_handler 14, 46
create_irq_handler 15, 47

.type common_interrupt_handler, @function
common_interrupt_handler:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp

    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    mov %ds, %rax
    push %rax

    mov %es, %rax
    push %rax

    mov %fs, %rax
    push %rax

    mov %gs, %rax
    push %rax

    mov %rsp, %rdi

    call kernel_interrupt_handler

    pop %rax
    mov %ax, %gs

    pop %rax
    mov %ax, %fs

    pop %rax
    mov %ax, %es

    pop %rax
    mov %ax, %ds

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8

    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
    
    add $16, %rsp

    iretq

.global enable_interrupts
.type enable_interrupts, @function
enable_interrupts:
    sti

    ret