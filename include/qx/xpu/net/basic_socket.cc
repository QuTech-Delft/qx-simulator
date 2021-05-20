
// function to fill in address structure given an address and port

inline static void fill_addr(const std::string &address, unsigned short port, 
                             sockaddr_in &addr) {
  memset(&addr, 0, sizeof(addr));  // zero out address structure
  addr.sin_family = AF_INET;       // internet address

  hostent *host;  // Resolve name
  if ((host = gethostbyname(address.c_str())) == NULL) {
    // strerror() will not work for gethostbyname() and hstrerror() 
    // is supposedly obsolete
    throw socket_exception("failed to resolve name (gethostbyname())");
  }
  addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);

  addr.sin_port = htons(port);     // assign port in network byte order
}

// socket code

basic_socket::basic_socket(int type, 
                           int protocol) throw(socket_exception) 
{
  #ifdef WIN32
    if (!initialized) {
      WORD wVersionRequested;
      WSADATA wsaData;

      wVersionRequested = MAKEWORD(2, 0);              // request WinSock v2.0
      if (WSAStartup(wVersionRequested, &wsaData) != 0) {  // load WinSock DLL
        throw socket_exception("unable to load WinSock dll");
      }
      initialized = true;
    }
  #endif

  // make a new socket
  if ((sock_desc = socket(PF_INET, type, protocol)) < 0) {
    throw socket_exception("socket creation failed (socket())", true);
  }
}


basic_socket::basic_socket(int sock_desc) 
{
  this->sock_desc = sock_desc;
}


basic_socket::~basic_socket() 
{
  #ifdef WIN32
    ::closesocket(sock_desc);
  #else
    ::close(sock_desc);
  #endif
  sock_desc = -1;
}



std::string basic_socket::get_local_address() throw (socket_exception) 
{
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sock_desc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw socket_exception("fetch of local address failed (getsockname())", true);
  }
  return inet_ntoa(addr.sin_addr);
}



unsigned short basic_socket::get_local_port() throw(socket_exception) 
{
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sock_desc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) 
  {
    throw socket_exception("fetch of local port failed (getsockname())", true);
  }
  return ntohs(addr.sin_port);
}

void basic_socket::set_local_port(unsigned short local_port) throw(socket_exception) 
{
  // bind the socket to its port
  sockaddr_in local_addr;
  memset(&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(local_port);

  if (bind(sock_desc, (sockaddr *) &local_addr, sizeof(sockaddr_in)) < 0) {
    throw socket_exception("set of local port failed (bind())", true);
  }
}

void basic_socket::set_local_address_and_port(const std::string &local_address,
                                              unsigned short local_port) throw(socket_exception) 
{
  // Get the address of the requested host
  sockaddr_in local_addr;
  fill_addr(local_address, local_port, local_addr);

  if (bind(sock_desc, (sockaddr *) &local_addr, sizeof(sockaddr_in)) < 0) {
    throw socket_exception("set of local address and port failed (bind())", true);
  }
}

void basic_socket::cleanup() throw(socket_exception) 
{
  #ifdef WIN32
    if (WSACleanup() != 0) {
      throw socket_exception("WSACleanup() failed");
    }
  #endif
}

unsigned short basic_socket::resolve_service(const std::string &service,
                                             const std::string &protocol) 
{
  struct servent *serv;        /* structure containing service information */

  if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL)
    return atoi(service.c_str());  /* service is port number */
  else 
    return ntohs(serv->s_port);    /* found port (network byte order) by name */
}


