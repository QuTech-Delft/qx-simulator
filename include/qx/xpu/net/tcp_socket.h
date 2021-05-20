#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include "qx/xpu/net/active_socket.h"


namespace xpu
{

class tcp_socket : public active_socket
{
   public:

  /**
   *   construct a tcp socket with no connection
   *   @exception socket_exception thrown if unable to create tcp socket
   */
   tcp_socket() throw (socket_exception);

  /**
   *   construct a tcp socket with a connection to the given foreign address
   *   and port
   *   @param foreign_address foreign address (IP address or name)
   *   @param foreign_port foreign port
   *   @exception SocketException thrown if unable to create tcp socket
   */
  tcp_socket(const std::string &foreign_address, unsigned short foreign_port) throw(socket_exception);

private:
  
  // access for tcp_server_socket::accept() connection creation
  friend class tcp_server_socket;
  
  tcp_socket(int new_conn_sd);

}; // class tcp_socket

#include "qx/xpu/net/tcp_socket.cc"

}

#endif // __TCP_SOCKET_H__

