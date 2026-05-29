#ifndef PASS_SOCKET_H
#define PASS_SOCKET_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using sock_t = int;
constexpr sock_t invalid_sock = -1;

namespace pass
{
    bool socket_init();
    sock_t create_tcp_socket();
    bool bind_socket(sock_t sock, int port);
    bool listen_socket(sock_t sock, int backlog = 5);
    sock_t accept_socket(sock_t server_sock);
    int send_data(sock_t sock, const std::string& data);
    std::string recv_data(sock_t sock, int buf_size = 4096);
    void close_socket(sock_t sock);
    void socket_cleanup();
}

#endif
