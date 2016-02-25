


udp_socket::udp_socket() throw (socket_exception) : active_socket(SOCK_DGRAM, IPPROTO_UDP) 
{
  set_broadcast();
}



udp_socket::udp_socket(unsigned short local_port)  throw (socket_exception) : active_socket(SOCK_DGRAM, IPPROTO_UDP) 
{
  set_local_port(local_port);
  set_broadcast();
}



udp_socket::udp_socket(const std::string &local_address, 
                       unsigned short local_port) throw (socket_exception) : active_socket(SOCK_DGRAM, IPPROTO_UDP) 
{
  set_local_address_and_port(local_address, local_port);
  set_broadcast();
}



void udp_socket::set_broadcast() 
{
  // If this fails, we'll hear about it when we try to send.  This will allow 
  // system that cannot broadcast to continue if they don't plan to broadcast
  int broadcast_permission = 1;
  
  setsockopt(sock_desc, 
             SOL_SOCKET, 
             SO_BROADCAST, 
             (raw_type *) &broadcast_permission, 
		   sizeof(broadcast_permission));
}


void udp_socket::disconnect() throw (socket_exception) {
  sockaddr_in null_addr;
  memset(&null_addr, 0, sizeof(null_addr));
  null_addr.sin_family = AF_UNSPEC;

  // try to disconnect
  if (::connect(sock_desc, (sockaddr *) &null_addr, sizeof(null_addr)) < 0) {
   #ifdef WIN32
    if (errno != WSAEAFNOSUPPORT) {
   #else
    if (errno != EAFNOSUPPORT) {
   #endif
      throw socket_exception("disconnect failed (connect())", true);
    }
  }
}

void udp_socket::send_to(const void *buffer, 
                         int buffer_len, 
                         const std::string &foreign_address, unsigned short foreign_port)  throw (socket_exception) 
{
  sockaddr_in dest_addr;
  fill_addr(foreign_address, foreign_port, dest_addr);

  // Write out the whole buffer as a single message.
  if (sendto(sock_desc, (raw_type *) buffer, buffer_len, 0,
             (sockaddr *) &dest_addr, sizeof(dest_addr)) != buffer_len) {
    throw socket_exception("send failed (sendto())", true);
  }
}

int udp_socket::recv_from(void *buffer, int buffer_len, std::string &source_address,
    unsigned short &source_port) throw (socket_exception) {
  sockaddr_in clnt_addr;
  socklen_t addr_len = sizeof(clnt_addr);
  int rtn;
  if ((rtn = recvfrom(sock_desc, (raw_type *) buffer, buffer_len, 0, 
                      (sockaddr *) &clnt_addr, (socklen_t *) &addr_len)) < 0) {
    throw socket_exception("receive failed (recvfrom())", true);
  }
  source_address = inet_ntoa(clnt_addr.sin_addr);
  source_port = ntohs(clnt_addr.sin_port);

  return rtn;
}

void udp_socket::set_multicast_ttl(unsigned char multicast_ttl) throw (socket_exception) {
  if (setsockopt(sock_desc, IPPROTO_IP, IP_MULTICAST_TTL, 
                 (raw_type *) &multicast_ttl, sizeof(multicast_ttl)) < 0) {
    throw socket_exception("multicast TTL set failed (setsockopt())", true);
  }
}

void udp_socket::join_group(const std::string &multicast_group) throw (socket_exception) {
  struct ip_mreq multicast_request;

  multicast_request.imr_multiaddr.s_addr = inet_addr(multicast_group.c_str());
  multicast_request.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(sock_desc, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                 (raw_type *) &multicast_request, 
                 sizeof(multicast_request)) < 0) {
    throw socket_exception("multicast group join failed (setsockopt())", true);
  }
}

void udp_socket::leave_group(const std::string &multicast_group) throw (socket_exception) {
  struct ip_mreq multicast_request;

  multicast_request.imr_multiaddr.s_addr = inet_addr(multicast_group.c_str());
  multicast_request.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(sock_desc, IPPROTO_IP, IP_DROP_MEMBERSHIP, 
                 (raw_type *) &multicast_request, 
                 sizeof(multicast_request)) < 0) {
    throw socket_exception("multicast group leave failed (setsockopt())", true);
  }
}
