/**
 * @file		init-clean-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @brief	     example of xpu runtime initilization	
 */

#include <xpu.h>
#include <xpu/task.h>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl

/**
 * main
 */
int main(int argc, char **argv)
{
   println("[+] initialization ...");

   xpu::init();
   
   println("[+] shutdown ...");
   
   xpu::clean();

   println("[+] done.");
   
   return 0;
}
