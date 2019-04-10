#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <arch/x86_64/vmm.h>

#include <kernel/heap.h>
#include <kernel/string.h>

#include <arch/x86_64/threading.h>

extern void kernel_idle();

extern void switch_thread(thread_t* old_thread, thread_t* new_thread);

extern void initialize_thread();

process_t* first_process = NULL;
process_t* current_process = NULL;

thread_t* create_thread(void* entry_point, bool user, void** arguments, size_t argument_count)
{
    thread_t* thread = (thread_t*)kmalloc(sizeof(thread_t));

    memset(thread, 0x00, sizeof(thread_t));

    uint64_t* stack = (uint64_t*)(kmalloc(0x1000) + 0x1000);

    thread->stack_top = (uint64_t)stack;

    *--stack = (uint64_t)entry_point;

    *--stack = (uint64_t)arguments;
    *--stack = argument_count;

    *--stack = user;

    *--stack = (uint64_t)initialize_thread;

    *--stack = 0x0000000000000000;

    *--stack = 0x0000000000000000;

    *--stack = 0x0000000000000000;
    *--stack = 0x0000000000000000;
    *--stack = 0x0000000000000000;
    *--stack = 0x0000000000000000;

    thread->stack = (uint64_t)stack;

    thread->next_thread = NULL;

    return thread;
}

process_t* create_process(void* entry_point, bool user, void** arguments, size_t argument_count, uint64_t pml4_table)
{
    process_t* process = (process_t*)kmalloc(sizeof(process_t));
    
    memset(process, 0x00, sizeof(process_t));

    thread_t* thread = create_thread(entry_point, user, arguments, argument_count);

    process->first_thread = thread;
    process->current_thread = thread;

    process->pml4_table = pml4_table;

    process->next_process = NULL;

    return process;
}

void process_add_thread(process_t* process, thread_t* thread)
{
    thread_t* last_thread = process->first_thread;

    while (last_thread->next_thread != NULL) last_thread = last_thread->next_thread;

    last_thread->next_thread = thread;
}

void add_process(process_t* process)
{
    process_t* last_process = first_process;

    while(last_process->next_process != NULL) last_process = last_process->next_process;

    last_process->next_process = process;
}

void schedule_tasks()
{
    thread_t* old_thread = current_process->current_thread;

    if (old_thread->next_thread == NULL)
    {
        current_process->current_thread = current_process->first_thread;

        if(current_process->next_process == NULL) current_process = first_process;
        else current_process = current_process->next_process;

        switch_pml4_table(current_process->pml4_table);
    }
    else
    {
        current_process->current_thread = current_process->current_thread->next_thread;
    }

    switch_thread(old_thread, current_process->current_thread);
}

void threading_initialize()
{
    process_t* kernel_process = create_process(kernel_idle, false, NULL, 0, get_current_pml4_table());

    first_process = kernel_process;
    current_process = kernel_process;

    switch_thread(NULL, kernel_process->first_thread);
}