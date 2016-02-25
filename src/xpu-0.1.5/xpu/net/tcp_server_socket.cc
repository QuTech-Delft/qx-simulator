

tcp_server_socket::tcp_server_socket(unsigned short local_port, 
                                     int queue_len)  throw (socket_exception) : basic_socket(SOCK_STREAM, IPPROTO_TCP) 
{
  set_local_port(local_port);
  set_listen(queue_len);
}



tcp_server_socket::tcp_server_socket(const std::string &local_address, 
                                     unsigned short local_port, 
							  int queue_len) throw (socket_exception) : basic_socket(SOCK_STREAM, IPPROTO_TCP) 
{
  set_local_address_and_port(local_address, local_port);
  set_listen(queue_len);
}



tcp_socket *tcp_server_socket::accept() throw (socket_exception) 
{
  int new_conn_sd;
  if ((new_conn_sd = ::accept(sock_desc, NULL, 0)) < 0) {
    throw socket_exception("accept failed (accept())", true);
  }

  return new tcp_socket(new_conn_sd);
}

void tcp_server_socket::set_listen(int queue_len) throw (socket_exception) 
{
  if (listen(sock_desc, queue_len) < 0) 
  {
    throw socket_exception("set listening socket failed (listen())", true);
  }
}


