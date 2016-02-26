/**
 * @file        qx_server.cc
 * @author      Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date        25-02-16
 */



#include <xpu/runtime>
#include <server/qx_server.h>


int main(int argc, char **argv)
{
   qx::qx_server server(5555);
   server.start();

   return 0;
}

