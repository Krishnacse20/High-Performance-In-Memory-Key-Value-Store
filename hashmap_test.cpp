#include "../include/hashmap.h"
#include <cassert>
#include <iostream>

void test_hashmap_basic() {
    HashMap map;

    map.set("key1", "value1");
    map.set("key2", "value2");

    auto v1 = map.get("key1");
    auto v2 = map.get("key2");
    auto v3 = map.get("key3");  // doesn't exist

    assert(v1.has_value() && v1.value() == "value1");
    assert(v2.has_value() && v2.value() == "value2");
    assert(!v3.has_value());

    map.del("key1");
    auto v1_after_del = map.get("key1");
    assert(!v1_after_del.has_value());

    std::cout << "[PASSED] HashMap basic tests\n";
}

int main() {
    test_hashmap_basic();
    return 0;
}
