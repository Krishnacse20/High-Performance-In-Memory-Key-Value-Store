cmake_minimum_required(VERSION 3.10)
project(KVStore)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(kvstore main.cpp src/hashmap.cpp src/memory_pool.cpp src/store.cpp src/thread_pool.cpp src/server.cpp)

add_executable(hashmap_test test/hashmap_test.cpp src/hashmap.cpp src/memory_pool.cpp)
add_executable(store_test test/store_test.cpp src/store.cpp src/hashmap.cpp src/memory_pool.cpp)

