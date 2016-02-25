

tcp_socket::tcp_socket() throw(socket_exception) : active_socket(SOCK_STREAM, IPPROTO_TCP) 
{
}


tcp_socket::tcp_socket(const std::string &foreign_address, 
                       unsigned short foreign_port) throw (socket_exception) : active_socket(SOCK_STREAM, IPPROTO_TCP) 
{
  connect(foreign_address, foreign_port);
}


tcp_socket::tcp_socket(int new_conn_sd) : active_socket(new_conn_sd) 
{
}


