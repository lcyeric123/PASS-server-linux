#include "pass_socket.h"

namespace pass
{
    bool socket_init()
    {
        return true;
    }

    sock_t create_tcp_socket()
    {
        return socket(AF_INET, SOCK_STREAM, 0);
    }

    bool bind_socket(sock_t sock, int port)
    {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        return bind(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
    }

    bool listen_socket(sock_t sock, int backlog)
    {
        return listen(sock, backlog) == 0;
    }

    sock_t accept_socket(sock_t server_sock)
    {
        return accept(server_sock, nullptr, nullptr);
    }

    int send_data(sock_t sock, const std::string& data)
    {
        return ::send(sock, data.data(), static_cast<int>(data.size()), 0);
    }

    std::string recv_data(sock_t sock, int buf_size)
    {
        std::string buf(buf_size, 0);
        int len = ::recv(sock, &buf[0], buf_size, 0);
        if (len <= 0) return "";
        buf.resize(len);
        return buf;
    }

    void close_socket(sock_t sock)
    {
        ::close(sock);
    }

    void socket_cleanup()
    {
    }
}
