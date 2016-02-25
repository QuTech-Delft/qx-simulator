/**
 * @file		sequential_tasks.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     sequential tasks	
 *
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

#ifndef __XPU_SEQUENTIAL_TASKS_2_H__
#define __XPU_SEQUENTIAL_TASKS_2_H__ 
 
#include <xpu/task_group.h>
//#include <xpu/task.h>

namespace xpu
{

  template <int N>
   class sequential_tasks : public task_group
   {
	 public:
	   
	   sequential_tasks(task_group ** tasks)
	   {
		 for (int i=0; i<N; i++)
		    m_tasks[i]    = tasks[i];
		 detect_shared();
	   }
	   
	   ~sequential_tasks()
	   {
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
		 for (int i=0; i<N; i++)
		    m_tasks[i]->run();
	   }

	   void detect_shared()
	   {
		 for (int i=0; i<N; i++)
		    m_tasks[i]->detect_shared();
	   }
	   
	   pointers get_in_data()
	   {
		 pointers ptrs; 
		 #pragma unroll
		 for (int j=0; j<N; j++)
		    ptrs += m_tasks[j]->get_in_data();
		 return ptrs;
	   }

	   pointers get_out_data()
	   {
		 pointers ptrs; 
		 #pragma unroll
		 for (int j=0; j<N; j++)
		    ptrs += m_tasks[j]->get_out_data();
		 return ptrs;
	   }

	   void set_shared(pointer p, lockable * l)
	   {
		 #pragma unroll
		 for (int i=0; i<N; i++)
		    m_tasks[i]->set_shared(p,l);
	   }

	   pointers get_pointers()
	   {
		 pointers ptrs;
		 #pragma unroll
		 for (int i=0; i<N; i++)
		    ptrs += m_tasks[i]->get_pointers();
		 return ptrs;
	   }

	 private:

	   task_group * m_tasks[N];
   };

   /**
    * sequential tasks builders
    */
   task_group * sequential(task_group * t1, task_group * t2)
   {
	 task_group * tgs[2]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 return new sequential_tasks<2>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3)
   {
	 task_group * tgs[3]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 return new sequential_tasks<3>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3, task_group * t4)
   {
	 task_group * tgs[4]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 return new sequential_tasks<4>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5)
   {
	 task_group * tgs[5]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 return new sequential_tasks<5>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6)
   {
	 task_group * tgs[6]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 tgs[5] = t6;
	 return new sequential_tasks<6>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7)
   {
	 task_group * tgs[7]; 
	 tgs[0] = t1;
	 tgs[1] = t2;
	 tgs[2] = t3;
	 tgs[3] = t4;
	 tgs[4] = t5;
	 tgs[5] = t6;
	 tgs[6] = t7;
	 return new sequential_tasks<7>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7, task_group * t8)
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
	 return new sequential_tasks<8>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7, task_group * t8, task_group * t9)
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
	 return new sequential_tasks<9>(tgs);
   }

   task_group * sequential(task_group * t1, task_group * t2, task_group * t3, task_group * t4, task_group * t5, task_group * t6, task_group * t7, task_group * t8, task_group * t9, task_group * t10)
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
	 return new sequential_tasks<10>(tgs);
   }
 



}

#endif // __XPU_SEQUENTIAL_TASKS_2_H__
