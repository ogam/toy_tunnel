#include "common/memory.h"

void init_memory()
{
    s_app->memory.scratch_arena = cf_make_arena(16, MB(8));
}

void* scratch_alloc(size_t size)
{
    return cf_arena_alloc(&s_app->memory.scratch_arena, (s32)size);
}

void scratch_reset()
{
    cf_arena_reset(&s_app->memory.scratch_arena);
}