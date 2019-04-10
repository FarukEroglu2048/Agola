#ifndef THREADING_H
#define THREADING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct thread
{
    uint64_t stack;

    uint64_t stack_top;

    bool user;
    bool blocked;

    struct thread* next_thread;
} thread_t;

typedef struct process
{
    thread_t* first_thread;
    thread_t* current_thread;

    bool blocked;

    uint64_t pml4_table;

    struct process* next_process;
} process_t;

process_t* create_process(void* entry_point, bool user, void** arguments, size_t argument_count, uint64_t pml4_table);
void add_process(process_t* process);

thread_t* create_thread(void* entry_point, bool user, void** arguments, size_t argument_count);
void process_add_thread(process_t* process, thread_t* thread);

void schedule_tasks();

void threading_initialize();

#endif