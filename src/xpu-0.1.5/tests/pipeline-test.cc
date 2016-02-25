/**
 * @file		pipeline-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @brief	     example of pipeline execution	
 */

#include <xpu.h>
//#include <xpu/runtime>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl

/**
 * 3 parallel stages
 */
int stage1(int i)
{ usleep(2000); println("[thread : " << xpu::thread_id() << "] : stage1(" << i << ")"); }

int stage2(int i)
{ usleep(1000); println("[thread : " << xpu::thread_id() << "] : stage2(" << i << ")"); }

int stage3(int i)
{ usleep(1000); println("[thread : " << xpu::thread_id() << "] : stage3(" << i << ")"); }


/**
 * main
 */
int main(int argc, char **argv)
{
   println("[+] initialization ...");
   xpu::init();

   println("[+] pipeline execution...");
   
   int data = 0;

   xpu::task t1(stage1, data);
   xpu::task t2(stage2, data);
   xpu::task t3(stage3, data);
   
   xpu::pipeline(10,&t1,&t2,&t3).run();
   
   println("[+] shutdown ...");
   xpu::clean();
   
   println("[+] done.");
    
   return 0;
}
