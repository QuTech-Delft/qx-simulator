/**
 * @file		pipeline-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @brief	     parallel for example 	
 */

#include <xpu.h>
//#include <xpu/runtime>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl

/**
 * main task which will be called of loop iterations
 */
int process(int from, int to, int step, int arg)
{ 
  usleep(from*1000); 
  println("[thread : " << xpu::thread_id() << "] : process(from=" << from << ", to=" << to << ", step=" << step << ", data=" << arg << ")"); 
}



/**
 * main
 */
int main(int argc, char **argv)
{
   println("[+] initialization ...");
   xpu::init();

   println("[+] parallel for loop construction...");
   
   int data = 0;

   xpu::task t(process, 0, 0, 0, data);
   xpu::parallel_for pf(0,100,1,&t);
   
   println("[+] parallel for loop execution...");
   pf.run();
   
   println("[+] shutdown ...");
   xpu::clean();
   
   println("[+] done.");
    
   return 0;
}
