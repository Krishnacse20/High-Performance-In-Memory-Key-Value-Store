#pragma once

#include <string>
#include <unordered_map>
#include <list>
#include <optional>
#include "hashmap.h"

class Store {
public:
    Store(size_t max_size = 100);
    ~Store();

    void set(const std::string& key, const std::string& value, size_t ttl = 0);
    std::optional<std::string> get(const std::string& key);
    void del(const std::string& key);

private:
    size_t max_size_;
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> cache_;
    std::list<std::pair<std::string, std::string>> order_;

    void evict();
};
