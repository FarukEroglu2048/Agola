#ifndef VFS_H
#define VFS_H

#include <stddef.h>
#include <stdint.h>

#define VFS_SEPERATOR "://"

typedef struct file_info
{
    uint64_t size;
} file_info_t;

typedef struct file
{
    const char* vfs_path;

    file_info_t* info;

    struct file* next_file;
} file_t;

typedef int (*vfs_open_t)(const char* vfs_path, int mode);
typedef int (*vfs_close_t)(const char* vfs_path);
typedef int (*vfs_read_t)(const char* vfs_path, size_t size, uint64_t offset, void* buffer);
typedef int (*vfs_write_t)(const char* vfs_path, size_t size, uint64_t offset, void* buffer);
typedef file_info_t* (*vfs_get_info_t)(const char* vfs_path);

typedef struct vfs_entry
{
    const char* name;

    vfs_open_t open_function;
    vfs_close_t close_function;
    vfs_read_t read_function;
    vfs_write_t write_function;
    vfs_get_info_t get_info_function;

    file_t* first_file;

    struct vfs_entry* next_entry;
} vfs_entry_t;

vfs_entry_t* vfs_create_entry(const char* name, vfs_open_t open_function, vfs_close_t close_function, vfs_read_t read_function, vfs_write_t write_function, vfs_get_info_t get_info_function);
void vfs_register_entry(vfs_entry_t* entry_to_register);

int vfs_open(const char* path, int mode);
int vfs_close(const char* path);
int vfs_read(const char* path, size_t size, uint64_t offset, void* buffer);
int vfs_write(const char* path, size_t size, uint64_t offset, void* buffer);

#endif