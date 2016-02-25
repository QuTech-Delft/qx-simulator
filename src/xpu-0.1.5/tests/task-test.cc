/**
 * @file		task-test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @brief	     task definition from functions and class methods 	
 */

#include <xpu.h>
#include <xpu/task.h>

#define print(x)   std::cout << x 
#define println(x) std::cout << x << std::endl

/**
 * functions
 */

int func(int i)
{ println("func()"); }


int func1(int i0, 
          int i1)
{ println("func1(" << i0 << ")"); usleep(i0*10000); }

int func2(int i0, 
          int i1,
          int i2)
{ println("func2(" << i0 << ")"); usleep(i0*10000); }

int func3(int i0, 
          int i1,
          int i2,
          int i3)
{ println("func3()"); }

int func4(int i0, 
          int i1,
          int i2,
          int i3,
          int i4)
{ println("func4()"); }

int func5(int i0, 
          int i1,
          int i2,
          int i3,
          int i4,
          int i5)
{ println("func5()"); }

int func6(int i0, 
          int i1,
          int i2,
          int i3,
          int i4,
          int i5,
          int i6)
{ println("func6()"); }

int func7(int i0, 
          int i1,
          int i2,
          int i3,
          int i4,
          int i5,
          int i6,
	     int i7)
{ println("func7()"); }


/**
 * classe methods
 */

class cls
{
   public:
	 
	 int meth(int i)
	 { println("meth()"); }


	 int meth1(int i0, 
		  int i1)
	 { println("meth1()"); }

	 int meth2(int i0, 
		  int i1,
		  int i2)
	 { println("meth2()"); }

	 int meth3(int i0, 
		  int i1,
		  int i2,
		  int i3)
	 { println("meth3()"); }

	 int meth4(int i0, 
		  int i1,
		  int i2,
		  int i3,
		  int i4)
	 { println("meth4()"); }

	 int meth5(int i0, 
		  int i1,
		  int i2,
		  int i3,
		  int i4,
		  int i5)
	 { println("meth5()"); }

	 int meth6(int i0, 
		  int i1,
		  int i2,
		  int i3,
		  int i4,
		  int i5,
		  int i6)
	 { println("meth6()"); }

	 int meth7(int i0, 
		  int i1,
		  int i2,
		  int i3,
		  int i4,
		  int i5,
		  int i6,
		  int i7)
	 { println("meth7()"); }

};

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

int stage(int i, int * x)
{ println("writer1()"); }



/**
 * main
 */
int main(int argc, char **argv)
{
   println("[+] initialization ...");

   xpu::init();
   
   println("[+] static callback tests ...");

   xpu::task t(func,0);
   xpu::task t1(func1,0,1);
   xpu::task t2(func2,0,1,2);
   xpu::task t3(func3,0,1,2,3);
   xpu::task t4(func4,0,1,2,3,4);
   xpu::task t5(func5,0,1,2,3,4,5);
   xpu::task t6(func6,0,1,2,3,4,5,6);
   xpu::task t7(func7,0,1,2,3,4,5,6,7);

   t.run();
   t1.run();
   t2.run();
   t3.run();
   t4.run();
   t5.run();
   t6.run();
   t7.run();

   println("[+] dynamic callback tests ...");

   cls c;

   xpu::task ct(&c,&cls::meth,0);
   xpu::task ct1(&c,&cls::meth1,0,1);
   xpu::task ct2(&c,&cls::meth2,0,1,2);
   xpu::task ct3(&c,&cls::meth3,0,1,2,3);
   xpu::task ct4(&c,&cls::meth4,0,1,2,3,4);
   xpu::task ct5(&c,&cls::meth5,0,1,2,3,4,5);
   xpu::task ct6(&c,&cls::meth6,0,1,2,3,4,5,6);
   xpu::task ct7(&c,&cls::meth7,0,1,2,3,4,5,6,7);

   ct.run();
   ct1.run();
   ct2.run();
   ct3.run();
   ct4.run();
   ct5.run();
   ct6.run();
   ct7.run();
 
   println("[+] asynchronous task execution : delegate task execution to peer threads ...");
   
   println("run task \"t\" five times on processor 0");
   for (int i=0; i<5; i++)
   {
	 xpu::sync s = spawn(t,0);
	 wait(s);
   }

   println("run task \"t\" five time on available processors (automatic load balancing)");
   for (int i=0; i<5; i++)
   {
	 xpu::sync s = spawn(t);
	 wait(s);
   }
   
   xpu::clean();

   return 0;
}
