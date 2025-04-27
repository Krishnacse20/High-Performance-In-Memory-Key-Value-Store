#include "hashmap.h"
#include <functional>

HashMap::HashMap(size_t initial_capacity)
    : table_(initial_capacity, nullptr),
    size_(0),
    capacity_(initial_capacity),
    pool_(sizeof(Entry)) {
}

HashMap::~HashMap() {
    for (auto& entry : table_) {
        if (entry && entry->state == EntryState::Occupied) {
            entry->~Entry();
            pool_.deallocate(entry);
        }
    }
}

size_t HashMap::hash(const std::string& key) const {
    std::hash<std::string> hasher;
    return hasher(key) % capacity_;
}

void HashMap::set(const std::string& key, const std::string& value) {
    if ((float)size_ / capacity_ >= load_factor_threshold_) {
        resize();
    }

    size_t idx = hash(key);

    while (table_[idx] && table_[idx]->state == EntryState::Occupied) {
        if (table_[idx]->key == key) {
            table_[idx]->value = value;
            return;
        }
        idx = (idx + 1) % capacity_;
    }

    if (!table_[idx]) {
        table_[idx] = static_cast<Entry*>(pool_.allocate());
    }

    table_[idx]->key = key;
    table_[idx]->value = value;
    table_[idx]->state = EntryState::Occupied;
    size_++;
}

std::optional<std::string> HashMap::get(const std::string& key) {
    size_t idx = hash(key);

    for (size_t i = 0; i < capacity_; ++i) {
        if (!table_[idx] || table_[idx]->state == EntryState::Empty) {
            return std::nullopt;
        }
        if (table_[idx]->state == EntryState::Occupied && table_[idx]->key == key) {
            return table_[idx]->value;
        }
        idx = (idx + 1) % capacity_;
    }

    return std::nullopt;
}

void HashMap::del(const std::string& key) {
    size_t idx = hash(key);

    for (size_t i = 0; i < capacity_; ++i) {
        if (!table_[idx] || table_[idx]->state == EntryState::Empty) {
            return;
        }
        if (table_[idx]->state == EntryState::Occupied && table_[idx]->key == key) {
            table_[idx]->state = EntryState::Deleted;
            size_--;
            return;
        }
        idx = (idx + 1) % capacity_;
    }
}

void HashMap::resize() {
    size_t new_capacity = capacity_ * 2;
    std::vector<Entry*> new_table(new_capacity, nullptr);

    for (auto& entry : table_) {
        if (entry && entry->state == EntryState::Occupied) {
            size_t idx = std::hash<std::string>{}(entry->key) % new_capacity;
            while (new_table[idx]) {
                idx = (idx + 1) % new_capacity;
            }
            new_table[idx] = entry;
        }
    }

    table_ = std::move(new_table);
    capacity_ = new_capacity;
}
