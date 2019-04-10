#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#define VGA_COLOR_BLACK 0x0
#define VGA_COLOR_BLUE 0x1
#define VGA_COLOR_GREEN 0x2
#define VGA_COLOR_CYAN 0x3
#define VGA_COLOR_RED 0x4
#define VGA_COLOR_MAGENTA 0x5
#define VGA_COLOR_BROWN 0x6
#define VGA_COLOR_LIGHT_GREY 0x7
#define VGA_COLOR_DARK_GREY 0x8
#define VGA_COLOR_LIGHT_BLUE 0x9
#define VGA_COLOR_LIGHT_GREEN 0xA
#define VGA_COLOR_LIGHT_CYAN 0xB
#define VGA_COLOR_LIGHT_RED 0xC
#define VGA_COLOR_LIGHT_MAGENT 0xD
#define VGA_COLOR_LIGHT_BROWN 0xE
#define VGA_COLOR_WHITE 0xF

void terminal_clear();

void terminal_print_char(char char_to_print);
void terminal_write(size_t size, const char* buffer);
void terminal_print_string(const char* string);

void terminal_initialize(uint8_t foreground_color, uint8_t background_color);

#endif