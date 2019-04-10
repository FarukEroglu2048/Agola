#include <multiboot2.h>

.section .multiboot

multiboot2_header_start:
    .align MULTIBOOT_HEADER_ALIGN

    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT_ARCHITECTURE_I386
    .long (multiboot2_header_end - multiboot2_header_start)
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot2_header_end - multiboot2_header_start))

    .align MULTIBOOT_HEADER_ALIGN
    
    .long 0x00000000
multiboot2_header_end:

.section .text

.align 16

.code32

.global entry_32
.type entry_32, @function
entry_32:
    cli
    cld

    mov %cr4, %eax
    or $0x20, %al
    mov %eax, %cr4

    mov $(temp_page_map_l4_table - 0xFFFFFFFF80000000), %eax
    mov %eax, %cr3

    mov $0xC0000080, %ecx
    rdmsr
    or $0x01, %ah
    wrmsr

    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0

    lgdt (Temp_GDT_Register - 0xFFFFFFFF80000000)

    ljmp $0x0008, $(entry_64 - 0xFFFFFFFF80000000)

.code64

.type entry_64, @function
entry_64:
    mov $0x0010, %ax

    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    mov %cr0, %rax
    and $0xFB, %al
    or $0x02, %al
    mov %rax, %cr0

    mov %cr4, %rax
    or $0x06, %ah
    mov %rax, %cr4

    mov $entry_higher_half, %rax
    jmp *%rax

.type start_higher_half, @function
entry_higher_half:
    mov $kernel_stack_top, %rsp

    call kernel_main
    
    cli
    hlt

    .infinite_loop:
        jmp .infinite_loop

.section .data

.align 16

Temp_GDT_Start:
    Temp_GDT_Null:
        .word 0x0000
        .word 0x0000
        .byte 0x00
        .byte 0x00
        .byte 0x00
        .byte 0x00
    Temp_GDT_Code:
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0b10011010
        .byte 0b10101111
        .byte 0x00
    Temp_GDT_Data:
        .word 0xFFFF
        .word 0x0000
        .byte 0x00
        .byte 0b10010010
        .byte 0b10101111
        .byte 0x00
Temp_GDT_End:

Temp_GDT_Register:
    .word (Temp_GDT_End - Temp_GDT_Start - 1)
    .long (Temp_GDT_Start - 0xFFFFFFFF80000000)

.align 4096

.macro create_temp_page_table index
    temp_page_table_\index:
        .set address, (\index * 0x200000)
        .rept 512
            .quad (address + 0x003)
            .set address, (address + 0x1000)
        .endr
.endm

.macro add_temp_page_table index
    .quad ((temp_page_table_\index - 0xFFFFFFFF80000000) + 0x003)
.endm

.set temp_page_table_count, 4

.altmacro

.set index, 0
.rept temp_page_table_count
    create_temp_page_table %index
    .set index, (index + 1)
.endr

temp_page_directory:
    .set index, 0
    .rept temp_page_table_count
        add_temp_page_table %index
        .set index, (index + 1)
    .endr

    .rept (512 - temp_page_table_count)
        .quad 0x0000000000000000
    .endr

.noaltmacro

temp_page_directory_table:
    .quad ((temp_page_directory - 0xFFFFFFFF80000000) + 0x003)

    .rept 509
        .quad 0x0000000000000000
    .endr

    .quad ((temp_page_directory - 0xFFFFFFFF80000000) + 0x003)

    .quad 0x0000000000000000

temp_page_map_l4_table:
    .quad ((temp_page_directory_table - 0xFFFFFFFF80000000) + 0x003)

    .rept 509
        .quad 0x0000000000000000
    .endr

    .quad ((temp_page_map_l4_table - 0xFFFFFFFF80000000) + 0x003)

    .quad ((temp_page_directory_table - 0xFFFFFFFF80000000) + 0x003)

.section .bss

.align 16

kernel_stack_bottom:
    .skip 16384
kernel_stack_top: