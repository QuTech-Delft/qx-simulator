#pragma once

#include "qx/xpu/net/tcp_socket.h"

namespace xpu {

/**
 *   tcp socket class for servers
 */
class tcp_server_socket : public basic_socket {
public:
    /**
     *   construct a TCP socket for use with a server, accepting connections
     *   on the specified port on any interface
     *   @param local_port local port of server socket, a value of zero will
     *                   give a system-assigned unused port
     *   @param queue_len maximum queue length for outstanding
     *                   connection requests (default 5)
     *   @exception socket_exception thrown if unable to create TCP server
     * socket
     */
    tcp_server_socket(unsigned short local_port, int queue_len = 5);

    /**
     *   construct a TCP socket for use with a server, accepting connections
     *   on the specified port on the interface specified by the given address
     *   @param local_address local interface (address) of server socket
     *   @param local_port local port of server socket
     *   @param queue_len maximum queue length for outstanding
     *                   connection requests (default 5)
     *   @exception socket_exception thrown if unable to create TCP server
     * socket
     */
    tcp_server_socket(const std::string &local_address,
                      unsigned short local_port, int queue_len = 5);

    /**
     *   blocks until a new connection is established on this socket or error
     *   @return new connection socket
     *   @exception socket_exception thrown if attempt to accept a new
     * connection fails
     */
    tcp_socket *accept();

private:
    void set_listen(int queue_len);
};
} // namespace xpu