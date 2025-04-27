#pragma once

#include <string>
#include <vector>
#include <optional>
#include "memory_pool.h"

class HashMap {
public:
    HashMap(size_t initial_capacity = 1024);
    ~HashMap();

    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    void del(const std::string& key);

private:
    enum class EntryState { Empty, Occupied, Deleted };

    struct Entry {
        std::string key;
        std::string value;
        EntryState state = EntryState::Empty;
    };

    std::vector<Entry*> table_;
    size_t size_;
    size_t capacity_;

    MemoryPool pool_;

    size_t hash(const std::string& key) const;
    void resize();

    static constexpr float load_factor_threshold_ = 0.7f;
};
