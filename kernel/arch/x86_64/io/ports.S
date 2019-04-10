.global outb
.type outb, @function
outb:
    mov %rdi, %rdx
    mov %rsi, %rax

    out %al, %dx

    ret

.global outw
.type outw @function
outw:
    mov %rdi, %rdx
    mov %rsi, %rax

    out %ax, %dx

    ret

.global outl
.type outl, @function
outl:
    mov %rdi, %rdx
    mov %rsi, %rax

    out %eax, %dx

    ret