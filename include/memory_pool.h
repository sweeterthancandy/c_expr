#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

typedef struct MemoryPool{
        char mem[4096];
        char* head;
}MemoryPool;

void MemoryPool_Init(MemoryPool* pool);
void* MemoryPool_Alloc(MemoryPool* pool, unsigned long long size);

#endif /* MEMORY_POOL_H */
