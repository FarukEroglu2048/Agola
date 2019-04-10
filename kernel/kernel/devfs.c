#include <stddef.h>
#include <stdint.h>

#include <kernel/heap.h>
#include <kernel/string.h>

#include <kernel/vfs.h>

#include <kernel/devfs.h>

device_t* first_device = NULL;

device_t* get_device_by_name(const char* name)
{
    device_t* device = first_device;

    while (device != NULL)
    {
        if (strcmp(device->name, name) == 0) break;

        device = device->next_device;
    }

    return device;
}

device_t* devfs_create_device(const char* name, device_read_t read_function, device_write_t write_function)
{
    device_t* device = (device_t*)kmalloc(sizeof(device_t));

    device->name = name;

    device->read_function = read_function;
    device->write_function = write_function;

    return device;
}

void devfs_register_device(device_t* device_to_register)
{
    if (first_device != NULL)
    {
        device_t* device = first_device;

        while(device->next_device != NULL) device = device->next_device;

        device->next_device = device_to_register;
    }
    else first_device = device_to_register;
}

int devfs_open(const char* name, int mode)
{
    int return_value = -1;

    device_t* device = get_device_by_name(name);

    if (device != NULL) return_value = 0;

    return return_value;
}

int devfs_close(const char* name)
{
    return 0;
}

int devfs_read(const char* name, size_t size, uint64_t offset, void* buffer)
{
    int return_value = -1;

    device_t* device = get_device_by_name(name);

    if (device != NULL) return_value = device->read_function(size, offset, buffer);

    return return_value;
}

int devfs_write(const char* name, size_t size, uint64_t offset, void* buffer)
{
    int return_value = -1;

    device_t* device = get_device_by_name(name);

    if (device != NULL) return_value = device->write_function(size, offset, buffer);

    return return_value;
}

void devfs_initialize()
{
    vfs_entry_t* devfs = vfs_create_entry("devfs", devfs_open, devfs_close, devfs_read, devfs_write, NULL);

    vfs_register_entry(devfs);
}