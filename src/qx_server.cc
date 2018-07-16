/**
 * @file        qx_server.cc
 * @author      Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date        25-02-16
 */

#include <xpu.h>
#include <xpu/runtime>
#include <qx_server.h>


int main(int argc, char **argv)
{
   println("");
   println("  =================================================================================================== "); 
   println("        _______                                                                                       ");
   println("       /  ___   \\   _  __      ____   ____   __  ___  __  __   __    ___  ______  ____    ___         ");
   println("      /  /   /  |  | |/_/     / __/  /  _/  /  |/  / / / / /  / /   / _ |/_  __/ / __ \\  / _ \\        ");
   println("     /  /___/  /  _>  <      _\\ \\   _/ /   / /|_/ / / /_/ /  / /__ / __ | / /   / /_/ / / , _/        ");
   println("     \\______/\\__\\ /_/|_|    /___/  /___/  /_/  /_/  \\____/  /____//_/ |_|/_/    \\____/ /_/|_|         ");
   println("                                                                                                      ");
   println("     version 0.1 beta - QuTech - 2016 - report bugs and suggestions to: nader.khammassi@gmail.com     ");
   println("  =================================================================================================== ");
   println("");

   xpu::init();

   size_t port = 5555;

   if (argc == 2)
      port = atoi(argv[1]);

   qx::qx_server server(port);
   server.start();

   return 0;
}

