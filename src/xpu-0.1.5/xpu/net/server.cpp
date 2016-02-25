
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

   // tcp server
   
   tcp_server_socket server(5555);
   tcp_socket      * sock = server.accept();
   std::cout << "client: " << sock->get_foreign_address() << ":" << sock->get_foreign_port() << std::endl;
   sock->send("welcom to server !", 19);
   sock->recv(buf, 256); 
   std::cout << buf << std::endl;
   sock->send("received.", 10);

   return 0;
}
