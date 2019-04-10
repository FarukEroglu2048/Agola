.section .text

.global get_current_pml4_table
get_current_pml4_table:
    mov %cr3, %rax

    ret

.global switch_pml4_table
.type switch_pml4_table, @function
switch_pml4_table:
    mov %cr3, %rax

    cmp %rdi, %rax
    je .return

    mov %rdi, %cr3

    .return:
        ret

.global invalidate_page
.type invalidate_page, @function
invalidate_page:
    invlpg (%rdi)

    ret

.global invalidate_pml4_table
.type invalidate_pml4_table, @function
invalidate_pml4_table:
    mov %cr3, %rax
    mov %rax, %cr3

    ret