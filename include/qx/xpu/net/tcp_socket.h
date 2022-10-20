#pragma once

#include "qx/xpu/net/active_socket.h"

namespace xpu {

class tcp_socket : public active_socket {
public:
    /**
     *   construct a tcp socket with no connection
     *   @exception socket_exception thrown if unable to create tcp socket
     */
    tcp_socket();

    /**
     *   construct a tcp socket with a connection to the given foreign address
     *   and port
     *   @param foreign_address foreign address (IP address or name)
     *   @param foreign_port foreign port
     *   @exception SocketException thrown if unable to create tcp socket
     */
    tcp_socket(const std::string &foreign_address, unsigned short foreign_port);

private:
    // access for tcp_server_socket::accept() connection creation
    friend class tcp_server_socket;

    tcp_socket(int new_conn_sd);

}; // class tcp_socket
} // namespace xpu