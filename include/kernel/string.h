#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

void memcpy(void* destination_pointer, const void* source_pointer, size_t size);
void memset(void* destination_pointer, uint8_t source, size_t size);

size_t strlen(const char* string);

int strncmp(const char* string_1, const char* string_2, size_t number);
int strcmp(const char* string_1, const char* string_2);

const char* strchr(const char* string, char char_to_search);
const char* strstr(const char* string, const char* string_to_search);

#endif