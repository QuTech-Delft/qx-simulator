/**
 * @file		shared-data-detection-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @brief	     an example of automatic shared data detection and protection	
 */

#include <xpu.h>
//#include <xpu/runtime>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl

/**
 * shared data detection
 */
int reader0(const int * x)
{ println("reader0()"); }

int reader1(const int * x)
{ println("reader1()"); }

int writer0(int * x)
{ println("writer0()"); }

int writer1(int * x)
{ println("writer1()"); }


/**
 * main
 */
int main(int argc, char **argv)
{
   println("[+] initialization ...");
   xpu::init();

   println("[+] shared data detection tests...");
   
   int data = 0;

   xpu::task r0(reader0, (const int *)&data);
   xpu::task r1(reader1, (const int *)&data);
   
   xpu::task w0(writer0, &data);
   xpu::task w1(writer1, &data);

   xpu::parallel(&r0,&r1)->run();
   
   xpu::parallel(&r0,&w1)->run();
   
   xpu::parallel(sequential(&r0,&r1),&w1)->run();


   println("[+] shutdown ...");
   xpu::clean();
   
   println("[+] done.");
    
   return 0;
}
