#pragma once

#include <cstddef>
#include <mutex>


struct MemBlock{
    MemBlock* next_;
};

class MemPool{
public:
    MemPool(size_t block_size,size_t capacity);
    ~MemPool();
    void*  alloc();
    void   free(void* mem);
    size_t used() const;
    size_t available() const;
private:
    void* mem_;
    MemBlock* head_;
    size_t block_size_;
    size_t capacity_;
    size_t used_;
    mutable std::mutex mutex_; 
};