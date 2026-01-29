#ifndef MEMORY_H
#define MEMORY_H

typedef struct Memory
{
    CF_Arena scratch_arena;
} Memory;

void init_memory();
void* scratch_alloc(size_t size);
void scratch_reset();

#endif //MEMORY_H
