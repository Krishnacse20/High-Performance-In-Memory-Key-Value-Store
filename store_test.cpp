#include <optional>
#include <cassert>
#include "store.h"

void test_store_lru() {
    Store store(2);  // Limit size to 2

    store.set("key1", "value1");
    store.set("key2", "value2");

    // At this point, key1 and key2 should be in cache
    assert(store.get("key1").has_value());
    assert(store.get("key2").has_value());

    // Adding key3 should evict key1 (LRU)
    store.set("key3", "value3");

    assert(store.get("key1") == std::nullopt);  // Key1 should be evicted
    assert(store.get("key2").has_value());
    assert(store.get("key3").has_value());

    std::cout << "[PASSED] Store LRU eviction test\n";
}

int main() {
    test_store_basic();
    test_store_ttl();
    test_store_lru();
    return 0;
}
