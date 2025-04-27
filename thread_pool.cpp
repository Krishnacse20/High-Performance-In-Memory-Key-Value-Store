#include "thread_pool.h"

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this]() { worker(); });
    }
}

ThreadPool::~ThreadPool() {
    stop_ = true;
    condition_.notify_all();
    for (auto& thread : workers_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(task);
    }
    condition_.notify_one();
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });

            if (stop_ && tasks_.empty())
                return;

            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}
