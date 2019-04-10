#ifndef DEVFS_H
#define DEVFS_H

#include <stddef.h>
#include <stdint.h>

typedef int (*device_read_t)(size_t size, uint64_t offset, void* buffer);
typedef int (*device_write_t)(size_t size, uint64_t offset, void* buffer);

typedef struct device
{
    const char* name;

    device_read_t read_function;
    device_write_t write_function;

    struct device* next_device;
} device_t;

device_t* devfs_create_device(const char* name, device_read_t read_function, device_write_t write_function);
void devfs_register_device(device_t* device_to_register);

int devfs_open(const char* name, int mode);
int devfs_close(const char* name);
int devfs_read(const char* name, size_t size, uint64_t offset, void* buffer);
int devfs_write(const char* name, size_t size, uint64_t offset, void* buffer);

void devfs_initialize();

#endif