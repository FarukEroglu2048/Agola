#include <stddef.h>
#include <stdint.h>

#include <kernel/heap.h>
#include <kernel/string.h>

#include <kernel/vfs.h>

vfs_entry_t* first_vfs_entry = NULL;

const char* get_vfs_path(const char* path)
{
    return strstr(path, VFS_SEPERATOR) + strlen(VFS_SEPERATOR);
}

vfs_entry_t* get_vfs_by_path(const char* path)
{
    vfs_entry_t* vfs_entry = first_vfs_entry;

    size_t vfs_name_length = get_vfs_path(path) - strlen(VFS_SEPERATOR) - path;

    while(vfs_entry != NULL)
    {
        if (strncmp(vfs_entry->name, path, vfs_name_length) == 0) break;

        vfs_entry = vfs_entry->next_entry;
    }

    return vfs_entry;   
}

file_t* get_file_by_vfs_path(vfs_entry_t* vfs_entry, const char* vfs_path)
{
    file_t* file = vfs_entry->first_file;

    while(file != NULL)
    {
        if (strcmp(file->vfs_path, vfs_path) == 0) break;

        file = file->next_file;
    }

    return file;     
}

vfs_entry_t* vfs_create_entry(const char* name, vfs_open_t open_function, vfs_close_t close_function, vfs_read_t read_function, vfs_write_t write_function, vfs_get_info_t get_info_function)
{
    vfs_entry_t* vfs_entry = (vfs_entry_t*)kmalloc(sizeof(vfs_entry_t));

    memset(vfs_entry, 0x00, sizeof(vfs_entry_t));

    vfs_entry->name = name;

    vfs_entry->open_function = open_function;
    vfs_entry->close_function = close_function;
    vfs_entry->read_function = read_function;
    vfs_entry->write_function = write_function;
    vfs_entry->get_info_function = get_info_function;

    return vfs_entry;
}

void vfs_register_entry(vfs_entry_t* entry_to_register)
{
    if (first_vfs_entry != NULL)
    {
        vfs_entry_t* vfs_entry = first_vfs_entry;

        while(vfs_entry->next_entry != NULL) vfs_entry = vfs_entry->next_entry;

        vfs_entry->next_entry = entry_to_register;
    }
    else first_vfs_entry = entry_to_register;
}

int vfs_open(const char* path, int mode)
{
    int return_value = -1;

    vfs_entry_t* vfs_entry = get_vfs_by_path(path);

    if (vfs_entry != NULL)
    {
        const char* vfs_path = get_vfs_path(path);

        return_value = vfs_entry->open_function(vfs_path, mode);
        
        if (return_value != -1)
        {
            file_t* file_to_open = (file_t*)kmalloc(sizeof(file_t));

            memset(file_to_open, 0x00, sizeof(file_t));

            file_to_open->vfs_path = vfs_path;

            if (vfs_entry->get_info_function != NULL) file_to_open->info = vfs_entry->get_info_function(vfs_path);

            if (vfs_entry->first_file != NULL)
            {
                file_t* file = vfs_entry->first_file;

                while(file->next_file != NULL) file = file->next_file;

                file->next_file = file_to_open;
            }
            else vfs_entry->first_file = file_to_open;
        }
    }

    return return_value;
}

int vfs_close(const char* path)
{
    int return_value = -1;

    vfs_entry_t* vfs_entry = get_vfs_by_path(path);

    if (vfs_entry != NULL)
    {
        const char* vfs_path = get_vfs_path(path);

        file_t* file = get_file_by_vfs_path(vfs_entry, vfs_path);

        if (file != NULL)
        {
            return_value = vfs_entry->close_function(vfs_path);

            kfree(file->info);
            kfree(file);
        }
    }

    return return_value;
}

int vfs_read(const char* path, size_t size, size_t offset, void* buffer)
{
    int return_value = -1;

    vfs_entry_t* vfs_entry = get_vfs_by_path(path);

    if (vfs_entry != NULL)
    {
        const char* vfs_path = get_vfs_path(path);

        file_t* file = get_file_by_vfs_path(vfs_entry, vfs_path);

        if (file != NULL) return_value = vfs_entry->read_function(vfs_path, size, offset, buffer);
    }

    return return_value;
}

int vfs_write(const char* path, size_t size, size_t offset, void* buffer)
{
    int return_value = -1;

    vfs_entry_t* vfs_entry = get_vfs_by_path(path);

    if (vfs_entry != NULL)
    {
        const char* vfs_path = get_vfs_path(path);

        file_t* file = get_file_by_vfs_path(vfs_entry, vfs_path);

        if (file != NULL) return_value = vfs_entry->write_function(vfs_path, size, offset, buffer);
    }

    return return_value;
}