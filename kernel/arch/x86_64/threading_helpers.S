.global switch_thread
.type switch_thread, @function
switch_thread:
    cmp $0x0000000000000000, %rdi
    je .restore_thread

    push %rbx
    
    push %rbp

    push %r12
    push %r13
    push %r14
    push %r15

    mov %rsp, (%rdi)

    .restore_thread:
        mov (%rsi), %rsp

        pop %r15
        pop %r14
        pop %r13
        pop %r12

        pop %rbp

        pop %rbx

        ret

.global initialize_thread
initialize_thread:
    pop %rax

    pop %rsi
    pop %rdi
    
    pop %rbx

    cmp $0x0000000000000000, %rax
    jne .initialize_user_thread

    mov %rsp, %rdx

    push $0x0010
    push %rdx
    push $0x0000000000000202
    push $0x0008
    push %rbx
    
    iretq

    .initialize_user_thread:
        push $0x0023
        push %rdx
        push $0x0000000000000202
        push $0x001B
        push %rbx

        iretq