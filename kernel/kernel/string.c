#include <stddef.h>
#include <stdint.h>

#include <kernel/string.h>

void memcpy(void* destination_pointer, const void* source_pointer, size_t size)
{
    uint8_t* destination = (uint8_t*)destination_pointer;
    const uint8_t* source = (const uint8_t*)source_pointer;

    for (size_t index = 0; index < size; index++) destination[index] = source[index];
}

void memset(void* destination_pointer, uint8_t source, size_t size)
{
    uint8_t* destination = (uint8_t*)destination_pointer;

    for (size_t index = 0; index < size; index++) destination[index] = source;
}

size_t strlen(const char* string)
{
    size_t index = 0;

    while (string[index] != '\0') index++;

    return index;
}

int strncmp(const char* string_1, const char* string_2, size_t number)
{
    for (size_t index = 0; (*string_1 == *string_2) && (index < number - 1); string_1++, string_2++, index++);

    return *string_1 - *string_2;
}

int strcmp(const char* string_1, const char* string_2)
{
    return strncmp(string_1, string_2, strlen(string_1));
}

const char* strchr(const char* string, char char_to_search)
{
    size_t index = 0;

    while ((index < strlen(string)) && (string[index] != char_to_search)) index++;

    if (string[index] == char_to_search) return string + index;
    else return NULL;
}

const char* strstr(const char* string, const char* string_to_search)
{
    size_t index = 0;

    while ((index < strlen(string)) && (strncmp(string + index, string_to_search, strlen(string_to_search)) != 0)) index++;

    if (strncmp(string + index, string_to_search, strlen(string_to_search)) == 0) return string + index;
    else return NULL;
}