#pragma once

#include <cstddef>
#include <vector>
#include <stack>
#include <mutex>

class MemoryPool {
public:
    MemoryPool(size_t object_size, size_t block_size = 1024);
    ~MemoryPool();

    void* allocate();
    void deallocate(void* ptr);

private:
    size_t object_size_;
    size_t block_size_;
    std::vector<void*> blocks_;
    std::stack<void*> free_list_;
    std::mutex mutex_;

    void allocate_block();
};
