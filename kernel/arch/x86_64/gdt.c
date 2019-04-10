#include <stdint.h>
#include <stdbool.h>

#include <arch/x86_64/gdt.h>

#define GDT_ENTRY_COUNT 8192

#define GDT_ENTRY_FLAG_PRESENT 0x80
#define GDT_ENTRY_FLAG_ALWAYS_ONE 0x10
#define GDT_ENTRY_FLAG_EXECUTABLE 0x08
#define GDT_ENTRY_FLAG_RW 0x02

#define GDT_ENTRY_FLAG_PAGE_GRANULARITY 0x08
#define GDT_ENTRY_FLAG_LONG 0x02

typedef struct gdt_register
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_register_t;

typedef struct gdt_entry
{
    uint16_t limit_low;

    uint16_t base_low;
    uint8_t base_middle;

    uint8_t access_flags;

    uint8_t limit_high : 4;

    uint8_t flags : 4;

    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

gdt_register_t gdt_register;

gdt_entry_t gdt_entries[GDT_ENTRY_COUNT];

extern void gdt_load(uint64_t gdt_register_address);

void gdt_create_null_entry(uint16_t index)
{
    gdt_entries[index].limit_low = 0x0000;
    
    gdt_entries[index].base_low = 0x0000;
    gdt_entries[index].base_middle = 0x00;

    gdt_entries[index].access_flags = 0x0000;

    gdt_entries[index].limit_high = 0x0;

    gdt_entries[index].flags = 0x0;
}

void gdt_create_long_entry(uint16_t index, uint8_t privilege_level, bool executable)
{
    gdt_entries[index].limit_low = 0xFFFF;
    
    gdt_entries[index].base_low = 0x0000;
    gdt_entries[index].base_middle = 0x00;

    gdt_entries[index].access_flags = GDT_ENTRY_FLAG_PRESENT | (privilege_level << 5) | GDT_ENTRY_FLAG_ALWAYS_ONE | GDT_ENTRY_FLAG_RW;
    if (executable == true) gdt_entries[index].access_flags |= GDT_ENTRY_FLAG_EXECUTABLE;

    gdt_entries[index].limit_high = 0xF;

    gdt_entries[index].flags = GDT_ENTRY_FLAG_PAGE_GRANULARITY | GDT_ENTRY_FLAG_LONG;
}

void gdt_initialize()
{
    gdt_create_null_entry(0);

    gdt_create_long_entry(1, DESCRIPTOR_PRIVILEGE_LEVEL_0, true);
    gdt_create_long_entry(2, DESCRIPTOR_PRIVILEGE_LEVEL_0, false);

    gdt_create_long_entry(3, DESCRIPTOR_PRIVILEGE_LEVEL_3, true);
    gdt_create_long_entry(4, DESCRIPTOR_PRIVILEGE_LEVEL_3, false);

    for (uint16_t index = 5; index < GDT_ENTRY_COUNT; index++)
    {
        gdt_create_null_entry(index);
    }

    gdt_register.limit = (sizeof(gdt_entries) - 1);
    gdt_register.base = (uint64_t)(&gdt_entries);

    gdt_load((uint64_t)(&gdt_register));
}