#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>

#include <arch/x86_64/interrupts.h>

#include <arch/x86_64/pmm.h>
#include <arch/x86_64/vmm.h>

#include <arch/x86_64/io/terminal.h>

#include <arch/x86_64/threading.h>

#include <kernel/heap.h>

#include <kernel/string.h>

#include <kernel/vfs.h>
#include <kernel/devfs.h>

void kernel_idle()
{
    if (vfs_open("devfs://terminal", 0) == 0)
    {
        char* string = "Hello World!";

        vfs_write("devfs://terminal", strlen(string), 0, string);
    }

    for(;;);
}

void kernel_main()
{
    gdt_initialize();
    idt_initialize();

    interrupts_initialize();

    pmm_initialize(0x1000000);
    vmm_initialize();
    heap_initialize();

    devfs_initialize();

    terminal_initialize(VGA_COLOR_WHITE, VGA_COLOR_CYAN);

    threading_initialize();

    for (;;);
}