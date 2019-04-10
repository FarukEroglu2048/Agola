.section .text

.global gdt_load
.type gdt_load, @function
gdt_load:
    lgdt (%rdi)

    mov $0x0010, %ax

    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    pop %rax

    mov %rsp, %rcx

    push $0x0010
    push %rcx
    pushf
    push $0x0008
    push %rax

    iretq

.global idt_load
.type idt_load, @function
idt_load:
    lidt (%rdi)

    ret