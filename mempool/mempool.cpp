#include "mempool.h"
#include <cstdlib>
#include <stdexcept>

MemPool::MemPool(size_t block_size,size_t capacity):
    mem_(nullptr),    
    head_(nullptr),
    block_size_(block_size),
    capacity_(capacity),used_(0)
{
    if(block_size_<sizeof(void*)) throw std::invalid_argument("block_size_ < sizeof(void*)");
    mem_=operator new(capacity*block_size);
    for(size_t i=0;i<capacity;i++){
        MemBlock* mem=(MemBlock*)((char*)mem_+i*block_size);
        mem->next_=head_;
        head_=mem;
    }
}

MemPool::~MemPool(){
    operator delete(mem_);
}

void MemPool::free(void* mem){
    char* p=(char*)mem;
    char* start=(char*)mem_;
    char* end=start+block_size_*capacity_;
    if(p<start||p>=end){
        throw std::invalid_argument("pointer does not belong to this pool");
    }
    MemBlock* tmp=(MemBlock*)mem;
    std::unique_lock<std::mutex> lock(mutex_);
    tmp->next_=head_;
    used_--;
    head_=tmp;
}

void* MemPool::alloc(){
    std::unique_lock<std::mutex> lock(mutex_);
    if(!head_){
        return nullptr;
    }
    void* ret=head_;
    head_=head_->next_;
    used_++;
    return ret;
}

size_t MemPool::used()const{
    std::unique_lock<std::mutex> lock(mutex_);
    return used_;
}

size_t MemPool::available()const{
    std::unique_lock<std::mutex> lock(mutex_);
    return capacity_;
}