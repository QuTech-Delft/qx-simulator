
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
   cout << buf << endl;
   client.send("testing...", 12);

   // tcp server
   /*
   tcp_server_socket server(5555);
   tcp_socket      * sock = server.accept();
   cout << "client: " << sock->get_foreign_address() << ":" << sock->get_foreign_port() << endl;
   sock->recv(buf, 256); 
   cout << buf << endl;
   sock->send("received.", 10);
   */

   // thread_monitor
   /*
   thread_monitor mon(5555); 
   mon.update();
   */

   // packet forge
   packet_factory fact;
   // packet size = 10 bytes
   debug_packet packet;
   fact.thread_creation_packet(0x11223344, packet);
   // print packet 
   packet.dump();

   int i;

   data d(&i, sizeof(int));
   

   return 0;
}
