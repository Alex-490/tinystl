#include "mempool.h"
#include <cassert>
#include <cstdio>

int main(){
    MemPool* pool=new MemPool(32,4);

    MemBlock* a=(MemBlock*)(pool->alloc());
    MemBlock* b=(MemBlock*)(pool->alloc());
    MemBlock* c=(MemBlock*)(pool->alloc());
    MemBlock* d=(MemBlock*)(pool->alloc());
    
    MemBlock* f=(MemBlock*)(pool->alloc());
    assert(f == nullptr);

    pool->free(a);
    MemBlock* g=(MemBlock*)(pool->alloc());
    assert(a == g);

    pool->free(b);
    pool->free(c);
    pool->free(d);
    pool->free(g);

    printf("all test passed\n");
    return 0;
}