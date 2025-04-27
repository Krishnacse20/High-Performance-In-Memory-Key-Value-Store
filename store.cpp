#include "store.h"
#include <iostream>
#include <chrono>

Store::Store(size_t max_size) : max_size_(max_size) {}

Store::~Store() {}

void Store::set(const std::string& key, const std::string& value, size_t ttl) {
    // Eviction logic if the cache size exceeds max_size_
    if (cache_.size() >= max_size_) {
        evict();
    }

    // If the key already exists, move it to the front (most recently used)
    if (cache_.find(key) != cache_.end()) {
        order_.erase(cache_[key]);
    }

    order_.push_front({ key, value });
    cache_[key] = order_.begin();
}

std::optional<std::string> Store::get(const std::string& key) {
    if (cache_.find(key) == cache_.end()) {
        return std::nullopt;
    }

    // Move the accessed item to the front (most recently used)
    auto it = cache_[key];
    order_.splice(order_.begin(), order_, it);

    return it->second;
}

void Store::del(const std::string& key) {
    if (cache_.find(key) != cache_.end()) {
        auto it = cache_[key];
        order_.erase(it);
        cache_.erase(key);
    }
}

void Store::evict() {
    // Evict the least recently used item (tail of the list)
    if (!order_.empty()) {
        auto lru = order_.back();
        cache_.erase(lru.first);
        order_.pop_back();
    }
}
