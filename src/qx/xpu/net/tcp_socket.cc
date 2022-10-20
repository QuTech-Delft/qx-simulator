#include "qx/xpu/net/tcp_socket.h"

namespace xpu {

tcp_socket::tcp_socket() : active_socket(SOCK_STREAM, IPPROTO_TCP) {}

tcp_socket::tcp_socket(const std::string &foreign_address,
                       unsigned short foreign_port)
    : active_socket(SOCK_STREAM, IPPROTO_TCP) {
    connect(foreign_address, foreign_port);
}

tcp_socket::tcp_socket(int new_conn_sd) : active_socket(new_conn_sd) {}

}