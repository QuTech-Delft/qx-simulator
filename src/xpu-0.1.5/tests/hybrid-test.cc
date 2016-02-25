/**
 * @file		hybrid-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		23-03-13
 * @brief	     example of parallel and sequential tasks combination	
 */

#include <xpu.h>
//#include <xpu/runtime>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl

/**
 * functions
 */
int function1(int x)
{ println("[thread : " << xpu::thread_id() << "] : function1()"); }

int function2(int x)
{ println("[thread : " << xpu::thread_id() << "] : function2()"); }

int function3(int x)
{ println("[thread : " << xpu::thread_id() << "] : function3()"); }


int function4(int x)
{ println("[thread : " << xpu::thread_id() << "] : function4()"); }


/**
 * main
 */
int main(int argc, char **argv)
{
   println("[+] initialization ...");
   xpu::init();

   println("[+] executing task graph...");
   
   int data = 0;

   xpu::task t1(function1, data);
   xpu::task t2(function2, data);
   xpu::task t3(function3, data);
   xpu::task t4(function4, data);
   
   xpu::parallel(xpu::sequential(&t1,&t2),&t3,&t4)->run();
   
   println("[+] shutdown ...");
   xpu::clean();
   
   println("[+] done.");
    
   return 0;
}
