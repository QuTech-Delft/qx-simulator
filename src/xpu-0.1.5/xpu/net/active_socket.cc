
active_socket::active_socket(int type, int protocol)  
                             throw (socket_exception) : basic_socket(type, protocol) 
{
}

active_socket::active_socket(int new_conn_sd) : basic_socket(new_conn_sd) 
{
}



void active_socket::connect(const std::string &foreign_address,
                            unsigned short foreign_port) throw (socket_exception) 
{
  // get the address of the requested host
  sockaddr_in dest_addr;
  fill_addr(foreign_address, foreign_port, dest_addr);

  // try to connect to the given port
  if (::connect(sock_desc, (sockaddr *) &dest_addr, sizeof(dest_addr)) < 0) {
    throw socket_exception("connect failed (connect())", true);
  }
}



void active_socket::send(const void *buffer, int buffer_len) throw (socket_exception) 
{
  if (::send(sock_desc, (raw_type *) buffer, buffer_len, 0) < 0) 
  {
    throw socket_exception("send failed (send())", true);
  }
}



int active_socket::recv(void *buffer, int buffer_len) throw (socket_exception) 
{
  int rtn;
  if ((rtn = ::recv(sock_desc, (raw_type *) buffer, buffer_len, 0)) < 0) 
  {
    throw socket_exception("received failed (recv())", true);
  }

  return rtn;
}

std::string active_socket::get_foreign_address() throw (socket_exception) 
{
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getpeername(sock_desc, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) 
  {
    throw socket_exception("fetch of foreign address failed (getpeername())", true);
  }
  return inet_ntoa(addr.sin_addr);
}

unsigned short active_socket::get_foreign_port() throw (socket_exception) 
{
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getpeername(sock_desc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) 
  {
    throw socket_exception("fetch of foreign port failed (getpeername())", true);
  }
  return ntohs(addr.sin_port);
}


