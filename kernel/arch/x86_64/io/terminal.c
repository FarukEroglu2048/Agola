#include <stdint.h>
#include <stddef.h>

#include <kernel/string.h>
#include <kernel/devfs.h>

#include <arch/x86_64/io/terminal.h>

#define VGA_MEMORY_ADDRESS 0xFFFFFFFF800B8000

#define VGA_TEXT_MODE_WIDTH 80
#define VGA_TEXT_MODE_HEIGHT 25

uint16_t* vga_memory = (uint16_t*)VGA_MEMORY_ADDRESS;

uint8_t terminal_x = 0;
uint8_t terminal_y = 0;

uint8_t terminal_foreground_color = 0;
uint8_t terminal_background_color = 0;

void terminal_clear()
{
    for (size_t index = 0; index < VGA_TEXT_MODE_WIDTH * VGA_TEXT_MODE_HEIGHT; index++)
    {
        vga_memory[index] = ((terminal_background_color << 12) | (terminal_foreground_color << 8) | 0x20);
    }
}

void terminal_scroll()
{
    memcpy(vga_memory, vga_memory + VGA_TEXT_MODE_WIDTH, VGA_TEXT_MODE_WIDTH * (VGA_TEXT_MODE_HEIGHT - 1) * 2);

    for (size_t index = VGA_TEXT_MODE_WIDTH * (VGA_TEXT_MODE_HEIGHT - 1); index < VGA_TEXT_MODE_WIDTH * VGA_TEXT_MODE_HEIGHT; index++)
    {
        vga_memory[index] = ((terminal_background_color << 12) | (terminal_foreground_color << 8) | 0x20);
    }   

    terminal_y = VGA_TEXT_MODE_HEIGHT - 1;
}

void terminal_print_char(char char_to_print)
{
    if (char_to_print == '\n') terminal_x = VGA_TEXT_MODE_WIDTH;
    else vga_memory[terminal_y * VGA_TEXT_MODE_WIDTH + terminal_x] = ((terminal_background_color << 12) | (terminal_foreground_color << 8) | (uint16_t)char_to_print);

    if (++terminal_x >= VGA_TEXT_MODE_WIDTH)
    {
        terminal_x = 0;

        if (++terminal_y >= VGA_TEXT_MODE_HEIGHT)
        {
            terminal_scroll();
        }
    }
}

void terminal_write(size_t size, const char* buffer)
{
    for (size_t index = 0; index < size; index++)
    {
        terminal_print_char(buffer[index]);
    }
}

void terminal_print_string(const char* string_to_print)
{
    terminal_write(strlen(string_to_print), string_to_print);
}

int terminal_devfs_write(size_t size, uint64_t offset, void* buffer)
{
    terminal_write(size, buffer);

    return 0;
}

void terminal_initialize(uint8_t foreground_color, uint8_t background_color)
{
    terminal_foreground_color = foreground_color;
    terminal_background_color = background_color;

    terminal_clear();

    device_t* device = devfs_create_device("terminal", NULL, terminal_devfs_write);

    devfs_register_device(device);
}