#include "../include/tcp_server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <syslog.h>
#include <iostream>

TcpServer::TcpServer(int port) : port(port), server_fd(-1), is_running(false) {}

TcpServer::~TcpServer() {
    stop();
}

bool TcpServer::setupServer() {
    // Open connection to system logger
    openlog("tcp_server", LOG_PID | LOG_CONS, LOG_USER);

    log("[INFO] Server is starting...");

    // Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        log("[ERROR] Socket creation failed");
        return false;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        log("[ERROR] Setsockopt failed");
        close(server_fd);
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        log("[ERROR] Bind failed");
        close(server_fd);
        return false;
    }

    log("[INFO] Server started on port " + std::to_string(port));

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        log("[ERROR] Listen failed");
        close(server_fd);
        return false;
    }

    return true;
}

void TcpServer::start() {
    if (setupServer()) {
        is_running = true;
        handleConnections();
    }
}

void TcpServer::stop() {
    if (is_running) {
        close(server_fd);
        syslog(LOG_INFO, "[INFO] Server stopped");
        closelog();
        is_running = false;
    }
}

void TcpServer::handleConnections() {
    sockaddr_in address;
    int addrlen = sizeof(address);

    while (is_running) {
        log("[INFO] Waiting for a connection...");

        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            log("[ERROR] Accept failed");
            continue; // Keep listening for other clients
        }

        log("[INFO] Connection established!");

        handleClient(client_socket);
    }
}

void TcpServer::handleClient(int client_socket) {
    char buffer[1024] = {0};

    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Clear buffer
        int bytesRead = read(client_socket, buffer, sizeof(buffer) - 1);

        if (bytesRead <= 0 || strlen(buffer) == 0) {
            if (bytesRead < 0) log("[ERROR] Read error");
            if (strlen(buffer) == 0) log("[WARN] Received an empty or invalid message, ignoring...");

            log("[INFO] Connection ended");
            close(client_socket);
            break;
        }

        log("[INFO] Received: " + std::string(buffer));

        // Echo back the message
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            log("[ERROR] Send failed");
            close(client_socket);
            break;
        }
    }
}

void TcpServer::log(const std::string& message) {
    syslog(LOG_INFO, "%s", message.c_str());
}

