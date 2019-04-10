ENTRY(entry_32)

SECTIONS
{
    . = 0xFFFFFFFF80000000 + 1M;

    .text : AT(ADDR(.text) - 0xFFFFFFFF80000000)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata : AT(ADDR(.rodata) - 0xFFFFFFFF80000000)
    {
        *(.rodata)
    }

    .data : AT(ADDR(.data) - 0xFFFFFFFF80000000)
    {
        *(.data)
    }

    .bss : AT(ADDR(.bss) - 0xFFFFFFFF80000000)
    {
        *(COMMON)
        *(.bss)
    }

    kernel_end_symbol = .;
}