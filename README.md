# High-Performance In-Memory Key-Value Store

## Overview

This project is a **Redis-like in-memory key-value store** built from scratch in **C++**. It supports basic operations like **GET**, **SET**, **DEL**, and **TTL (Time to Live)** expiry. The key-value store is designed for high performance and supports advanced features like **LRU (Least Recently Used) eviction**, **multithreading** for async operations, and efficient memory usage.

Key features include:
- **GET**, **SET**, **DEL**, and **TTL expiry**
- **LRU eviction** strategy to free memory
- **ThreadPool** for efficient async task execution
- **Memory pooling** for optimal memory management

This project focuses on achieving **sub-millisecond latency** and has been stress-tested to handle **1M operations/sec** under load.

---

## Features

- **GET/SET Operations**: Efficient key-value retrieval and storage.
- **TTL Expiry**: Automatically expires keys after a specified time.
- **LRU Cache**: Evicts least recently used items when memory is full.
- **Multithreading**: Supports async operations with a custom thread pool and I/O using **epoll/select**.
- **Custom HashMap**: Optimized for the key-value store with efficient lookups.

---

## Architecture

- **Store**: Manages key-value pairs, implements LRU eviction, and handles TTL.
- **ThreadPool**: Manages background threads for async task execution.
- **HashMap**: Custom implementation used to store key-value pairs.
- **Memory Pooling**: Optimizes memory allocation to reduce overhead.
  
---

## Installation

To build and run the project, you need a **C++17** compatible compiler and **CMake**.

### 1. Clone the repository

git clone https://github.com/your-username/in-memory-kvstore.git
cd in-memory-kvstore

### 2. Build the project
mkdir build
cd build
cmake ..
cmake --build .

### 3. Run the tests
./store_test

