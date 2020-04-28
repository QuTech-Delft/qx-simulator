/**
 * @file		parallel_tasks.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief		parallel tasks (fork & join)
 * @copyright
 *
 * XPU - C++ Parallel  Programming Library for Multicore Architectures
 *
 * Copyright (C) 2014 Nader Khammassi, All Rights Reserved.
 *
 * This file is part of XPU and has been downloaded from 
 * http://www.xpu-project.net/.
 *
 * XPU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * XPU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __XPU_PARALLEL_TASKS_2_H__
#define __XPU_PARALLEL_TASKS_2_H__


#include <xpu/core/os/thread.h> 
#include <xpu/core/lockable_factory.h>
#include <xpu/core/work.h> 
#include <xpu/task.h>

using namespace xpu::core;

namespace xpu
{
   
   template <int N>
   class parallel_tasks : public task_group
   {
	 public:
	   //parallel_tasks(const task_group * tasks[N])
	   parallel_tasks(task_group ** tasks)
	   {
		 for (int i=0; i<N; i++)
		 {
		    m_tasks[i]    = tasks[i];
		    m_threads[i]  = new xpu::core::os::thread((task*)tasks[i]);
		    m_works[i]    = new basic_work((task*)tasks[i]);
		 }
		 #ifndef XPU_DISABLE_AUTOMATIC_SHARED_MEMORY_DETECTION
		 detect_shared();
           #endif // XPU_DISABLE_AUTOMATIC_SHARED_MEMORY_DETECTION
	   }
	   
	   ~parallel_tasks()
	   {
		 for (int i=0; i<N; i++)
		    delete  m_threads[i];
	   }

	   void run(range& r)
	   {
		 run();
	   }

        void run(int i)
	   {
		 run();
	   }


	   void run()
	   {
		 /*
		 for (int i=0; i<N; i++)
		    m_threads[i]->start();
		 for (int i=0; i<N; i++)
		    m_threads[i]->join();
		 */
		 for (int i=0; i<N; i++)
		    core::dynamic_work_queue.push(m_works[i]);
		 for (int i=0; i<N; i++)
		    m_works[i]->wait();

	   }

	   void detect_shared()
	   {
		for (int i=0; i<N-1; i++)
		{
		   // check in data
		   //__debug(" parallel_tasks: checking for shared data..."); 
		   pointers in_ptrs1 = m_tasks[i]->get_in_data();
		   for (int j=i+1; j<N; j++)
		   {
			 pointers ptrs2  = m_tasks[j]->get_out_data();
			 pointers shared = in_ptrs1 * ptrs2;
			 if (!shared.empty())
			 {
			    for (pointers::iterator it=shared.begin(); it!=shared.end(); it++)
			    {
				  //__debug(" parallel_tasks: shared data detected: " << (void*)(*it)); 
				  std::cout << "[+] xpu::parallel_tasks : shared data detected: " << (void*)(*it) << std::endl; 
				  lockable * l = xpu::core::lockable_factory::instance()->getlockable(*it);
				  //__debug(" parallel_tasks: automatic resource protection: pointer=" << (void*)(*it) << " , lockable=" << (void*)l);
				  std::cout << "[+] xpu::parallel_tasks: automatic memory protection: pointer=" << (void*)(*it) << " , lockable=" << (void*)l << std::endl;
				  m_tasks[i]->set_shared(*it,l);
				  m_tasks[j]->set_shared(*it,l);
			    }
			 }
		   }
		   pointers out_ptrs1 = m_tasks[i]->get_out_data();
		   for (int j=i+1; j<N; j++)
		   {
			 pointers ptrs2  = m_tasks[j]->get_in_data();
			 ptrs2  += m_tasks[j]->get_out_data();
			 pointers shared = out_ptrs1 * ptrs2;
			 if (!shared.empty())
			 {
			    for (pointers::iterator it=shared.begin(); it!=shared.end(); it++)
			    {
				  __debug(" parallel_tasks: shared data detected: " << (void*)(*it)); 
				  lockable * l = xpu::core::lockable_factory::instance()->getlockable(*it);
				  __debug(" parallel_tasks: automatic resource protection: pointer=" << (void*)(*it) << " , lockable=" << (void*)l);
				  m_tasks[i]->set_shared(*it,l);
				  m_tasks[j]->set_shared(*it,l);
			    }
			 }
		   }

		}
	   }
	   
	   pointers get_in_data()
	   {
		 pointers ptrs; 
		 // #pragma unroll
		 for (int j=0; j<N; j++)
		    ptrs += m_tasks[j]->get_in_data();
		 return ptrs;
	   }

	   pointers get_out_data()
	   {
		 pointers ptrs; 
           // #pragma unroll
		 for (int j=0; j<N; j++)
		    ptrs += m_tasks[j]->get_out_data();
		 return ptrs;
	   }

	   void set_shared(pointer p, lockable * l)
	   {
		 // #pragma unroll
		 for (int i=0; i<N; i++)
		    m_tasks[i]->set_shared(p,l);
	   }

	   pointers get_pointers()
	   {
		 pointers ptrs;
		 // #pragma unroll
		 for (int i=0; i<N; i++)
		    ptrs += m_tasks[i]->get_pointers();
		 return ptrs;
	   }

	 private:

	   task_group            * m_tasks[N];
	   xpu::core::os::thread * m_threads[N];
	   basic_work            * m_works[N];
   };


   /**
    * parallel tasks builders
    */
   task_group * parallel(task_group * t1, task_group * t2)
   {
	 task_group * tgs[2]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 return new parallel_tasks<2>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3)
   {
	 task_group * tgs[3]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 return new parallel_tasks<3>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3, task_group * t4)
   {
	 task_group * tgs[4]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 return new parallel_tasks<4>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5)
   {
	 task_group * tgs[5]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 return new parallel_tasks<5>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6)
   {
	 task_group * tgs[6]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 tgs[5] = t6;
	 return new parallel_tasks<6>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7)
   {
	 task_group * tgs[7]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 tgs[5] = t6;
	 tgs[6] = t7;
	 return new parallel_tasks<7>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7, task_group * t8)
   {
	 task_group * tgs[8]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 tgs[5] = t6;
	 tgs[6] = t7;
	 tgs[7] = t8;
	 return new parallel_tasks<8>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7, task_group * t8, task_group * t9)
   {
	 task_group * tgs[9]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 tgs[5] = t6;
	 tgs[6] = t7;
	 tgs[7] = t8;
	 tgs[8] = t9;
	 return new parallel_tasks<9>(tgs);
   }

   task_group * parallel(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7, task_group * t8, task_group * t9, task_group * t10)
   {
	 task_group * tgs[10]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 tgs[5] = t6;
	 tgs[6] = t7;
	 tgs[7] = t8;
	 tgs[8] = t9;
	 tgs[9] = t10;
	 return new parallel_tasks<10>(tgs);
   }
}

#endif
