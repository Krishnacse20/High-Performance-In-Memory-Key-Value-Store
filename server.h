#pragma once
#pragma once
#include "store.h"
#include "thread_pool.h"
#include <thread>
#include <atomic>

class Server {
public:
    Server(int port);
    ~Server();
    void run();

private:
    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    Store store_;
    ThreadPool thread_pool_;

    void init();
    void accept_connections();
    void handle_client(int client_fd);
};
