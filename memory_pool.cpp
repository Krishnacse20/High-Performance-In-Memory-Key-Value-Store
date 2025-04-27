#include "memory_pool.h"
#include <cstdlib>
#include <cstring>

MemoryPool::MemoryPool(size_t object_size, size_t block_size)
    : object_size_(object_size), block_size_(block_size) {
    allocate_block();  // Allocate first block
}

MemoryPool::~MemoryPool() {
    for (void* block : blocks_) {
        std::free(block);
    }
}

void MemoryPool::allocate_block() {
    void* block = std::malloc(object_size_ * block_size_);
    blocks_.push_back(block);

    char* ptr = static_cast<char*>(block);
    for (size_t i = 0; i < block_size_; ++i) {
        free_list_.push(ptr + i * object_size_);
    }
}

void* MemoryPool::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (free_list_.empty()) {
        allocate_block();
    }

    void* ptr = free_list_.top();
    free_list_.pop();
    return ptr;
}

void MemoryPool::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(mutex_);
    free_list_.push(ptr);
}
