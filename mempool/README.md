# 简单内存池

# 简介
固定块大小的内存池，预先用malloc分配一块连续内存，分割后用空闲链表管理内存块
alloc和free均为O(1),避免频繁调用malloc/free产生的开销和碎片

# 设计
空闲链表:每个空闲块头部存一个next指针，串成单链表
alloc：从链表头弹出一块，O(1)
free:把块头插回链表头,O(1)
线程安全:mutex保护链条头的读写
block_size_：必须>=sizeof(void*)，否则无法存储next指针

# 接口
MemPool(size_t block_size,size_t capacity)：创建内存池，block_size为块大小，capacity为总数
void*  alloc()：分配一块
void   free(void* mem)：归还一块
size_t used() const：已分配块数
size_t available() const：可用块数