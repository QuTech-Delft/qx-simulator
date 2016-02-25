/**
 * @file		parallel-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @brief	     example of parallel tasks execution	
 */

#include <xpu.h>
//#include <xpu/runtime>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl

/**
 * 3 parallel functions
 */
int function1(int x)
{ println("[thread : " << xpu::thread_id() << "] : function1()"); }

int function2(int x)
{ println("[thread : " << xpu::thread_id() << "] : function2()"); }

int function3(int x)
{ println("[thread : " << xpu::thread_id() << "] : function3()"); }


/**
 * main
 */
int main(int argc, char **argv)
{
   println("[+] initialization ...");
   xpu::init();

   println("[+] parallel tasks...");
   
   int data = 0;

   xpu::task t1(function1, data);
   xpu::task t2(function2, data);
   xpu::task t3(function3, data);
   
   xpu::parallel(&t1,&t2,&t3)->run();
   
   println("[+] shutdown ...");
   xpu::clean();
   
   println("[+] done.");
    
   return 0;
}
