#include <iostream>

#include "socket_exception.h"
#include "basic_socket.h"
#include "active_socket.h"
#include "tcp_socket.h"
#include "tcp_server_socket.h"
#include "udp_socket.h"

int main(int argc, char **argv)
{
   int n = 0;
   char buf[256];
   
   buf[255] = 0;

   // tcp client  
   
   tcp_socket client("127.0.0.1", 5555);

   client.recv(buf, 256);
   std::cout << buf << std::endl;
   client.send("testing...", 12);

   return 0;
}
