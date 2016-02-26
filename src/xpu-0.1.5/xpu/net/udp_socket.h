#ifndef __UDP_SOCKET_H__
#define __UDP_SOCKET_H__


namespace xpu
{

/**
  *   udp socket class
  */
class udp_socket : public active_socket {
public:
  /**
   *   Construct a UDP socket
   *   @exception socket_exception thrown if unable to create UDP socket
   */
  udp_socket() throw (socket_exception);

  /**
   *   Construct a UDP socket with the given local port
   *   @param local_port local port
   *   @exception socket_exception thrown if unable to create UDP socket
   */
  udp_socket(unsigned short local_port) throw (socket_exception);

  /**
   *   Construct a UDP socket with the given local port and address
   *   @param local_address local address
   *   @param local_port local port
   *   @exception socket_exception thrown if unable to create UDP socket
   */
  udp_socket(const std::string &local_address, unsigned short local_port) throw (socket_exception);

  /**
   *   Unset foreign address and port
   *   @return true if disassociation is successful
   *   @exception socket_exception thrown if unable to disconnect UDP socket
   */
  void disconnect() throw (socket_exception);

  /**
   *   Send the given buffer as a UDP datagram to the
   *   specified address/port
   *   @param buffer buffer to be written
   *   @param buffer_len number of bytes to write
   *   @param foreign_address address (IP address or name) to send to
   *   @param foreign_port port number to send to
   *   @return true if send is successful
   *   @exception socket_exception thrown if unable to send datagram
   */
  void send_to(const void *buffer, int buffer_len, const std::string &foreign_address,
               unsigned short foreign_port) throw (socket_exception);

  /**
   *   Read read up to buffer_len bytes data from this socket.  The given buffer
   *   is where the data will be placed
   *   @param buffer buffer to receive data
   *   @param buffer_len maximum number of bytes to receive
   *   @param source_address address of datagram source
   *   @param source_port port of data source
   *   @return number of bytes received and -1 for error
   *   @exception socket_exception thrown if unable to receive datagram
   */
  int recv_from(void *buffer, int buffer_len, std::string &source_address, 
               unsigned short &source_port) throw (socket_exception);

  /**
   *   Set the multicast TTL
   *   @param multicastTTL multicast TTL
   *   @exception socket_exception thrown if unable to set TTL
   */
  void set_multicast_ttl(unsigned char multicast_ttl) throw (socket_exception);

  /**
   *   Join the specified multicast group
   *   @param multicastGroup multicast group address to join
   *   @exception socket_exception thrown if unable to join group
   */
  void join_group(const std::string &multicast_group) throw (socket_exception);

  /**
   *   Leave the specified multicast group
   *   @param multicastGroup multicast group address to leave
   *   @exception socket_exception thrown if unable to leave group
   */
  void leave_group(const std::string &multicast_group) throw (socket_exception);

private:
  void set_broadcast();
};


#include "udp_socket.cc"

}

#endif // __UDP_SOCKET_H__
