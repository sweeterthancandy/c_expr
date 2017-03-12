#include "memory_pool.h"

void MemoryPool_Init(MemoryPool* pool){
        pool->head = pool->mem;
}
void* MemoryPool_Alloc(MemoryPool* pool, unsigned long long size){
        void* ptr = pool->head;
        pool->head += size;
        return ptr;
}
