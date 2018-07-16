/**
 * @file		pipeline-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @brief	     parallel for perf test
 */

#include <cmath>

#include <xpu.h>
#include <xpu/runtime>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl


static double * data;

/**
 * main task which will be called of loop iterations
 */
int process(int from, int to, int step, double * d)
{ 
   for (int i=from; i<to; ++i)
   {
      d[i] += sin(d[i]+sqrt(d[i])) / ( cos(d[i]) + 1.0f ); 
      d[i] += sin(d[i]+sqrt(d[i])) / ( cos(d[i]) + 1.0f ); 
   }
   return 0;
}



/**
 * main
 */
int main(int argc, char **argv)
{
   if (argc != 2) 
   {
      println("[x] usage : " << argv[0] << " data_size");
      return -1;
   }
   int size = atoi(argv[1]);
   println("[+] size : " << size);
   println("[+] initialization ...");
   xpu::init();

   data = new double[size];

   println("[+] parallel for loop construction...");
   
   xpu::task t(process, 0, 0, 0, data);
   xpu::parallel_for pf(0,size,1,&t);
   
   println("[+] parallel for loop execution...");
   pf.run();
   
   println("[+] shutdown ...");
   xpu::clean();
   
   println("[+] done.");
    
   return 0;
}
