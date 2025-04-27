#include "server.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include <cstring>
#include <iostream>

Server::Server(int port)
    : port_(port), running_(true), thread_pool_(4) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    init();
}

Server::~Server() {
    running_ = false;
#ifdef _WIN32
    closesocket(server_fd_);
    WSACleanup();
#else
    close(server_fd_);
#endif
}

void Server::init() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd_, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

void Server::run() {
    accept_connections();
}

void Server::accept_connections() {
    fd_set master_set, read_set;
    FD_ZERO(&master_set);
    FD_SET(server_fd_, &master_set);

    int max_fd = server_fd_;

    while (running_) {
        read_set = master_set;

        if (select(max_fd + 1, &read_set, nullptr, nullptr, nullptr) < 0) {
            perror("select error");
            continue;
        }

        for (int fd = 0; fd <= max_fd; ++fd) {
            if (FD_ISSET(fd, &read_set)) {
                if (fd == server_fd_) {
                    // New connection
                    int new_socket;
                    sockaddr_in client_address;
#ifdef _WIN32
                    int addrlen = sizeof(client_address);
#else
                    socklen_t addrlen = sizeof(client_address);
#endif
                    new_socket = accept(server_fd_, (struct sockaddr*)&client_address, &addrlen);
                    if (new_socket >= 0) {
                        FD_SET(new_socket, &master_set);
                        if (new_socket > max_fd) max_fd = new_socket;
                    }
                }
                else {
                    // Existing connection
                    thread_pool_.enqueue([this, fd]() { handle_client(fd); });
                    FD_CLR(fd, &master_set);
                }
            }
        }
    }
}

void Server::handle_client(int client_fd) {
    char buffer[1024] = { 0 };
#ifdef _WIN32
    int valread = recv(client_fd, buffer, sizeof(buffer), 0);
#else
    int valread = read(client_fd, buffer, sizeof(buffer));
#endif

    if (valread <= 0) {
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return;
    }

    std::string request(buffer, valread);
    std::string response;

    if (request.starts_with("GET")) {
        std::string key = request.substr(4);
        response = store_.get(key);
    }
    else if (request.starts_with("SET")) {
        size_t sep = request.find(' ', 4);
        std::string key = request.substr(4, sep - 4);
        std::string value = request.substr(sep + 1);
        store_.set(key, value);
        response = "OK";
    }
    else if (request.starts_with("DEL")) {
        std::string key = request.substr(4);
        store_.del(key);
        response = "OK";
    }
    else {
        response = "Unknown Command";
    }

#ifdef _WIN32
    send(client_fd, response.c_str(), response.length(), 0);
    closesocket(client_fd);
#else
    send(client_fd, response.c_str(), response.length(), 0);
    close(client_fd);
#endif
}
