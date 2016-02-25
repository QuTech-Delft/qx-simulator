/**
 * @file		parallel_for.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		12-11-11
 * @brief	     parallel for implementation (static data partitioning)
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
 */

#ifndef __XPU_PARALLEL_FOR__
#define __XPU_PARALLEL_FOR__

#include <xpu.h>
#include <xpu/pointers.h>
#include <xpu/range.h>

#include <xpu/core/generic_worker.h>

using namespace xpu::core;

namespace xpu
{
   class parallel_for : public task_group
   {
	 public:

	    inline parallel_for(int from, int to, int step, task * t) : m_task(t)
	    {
	       range r(from, to, step);
		  int len = abs((to-from)/step);
		  if (len>=core::workers_count)
		  {
			m_work_count = core::workers_count; 
		     rgs = r.split_dec(m_work_count);
		     //for (int i=0; i<m_work_count; i++)
			  // rgs[i].debug();
		     m_works  = new work * [m_work_count];
			for (int i=1; i<m_work_count; i++)
			{
			work * w = (work*)(new for_work(t,rgs[i]));
			//core::workers[i]->submit(w);
			m_works[i]  = w;
		     }
		  }
		  else if (len>=1)
		  {
			m_work_count = len; 
		     rgs = r.split_dec(m_work_count);
		     //for (int i=0; i<m_work_count; i++)
			  // rgs[i].debug();
		     m_works  = new work * [m_work_count];
			for (int i=1; i<m_work_count; i++)
			{
			work * w = (work*)(new for_work(t,rgs[i]));
			//core::workers[i]->submit(w);
			m_works[i]  = w;
		     }
		  }
		  else
		  {
			m_work_count = 0;
			//m_works[0]  = (work*)(new for_work(t,r);
		  }
		  /*
		  t->run(rgs[0]);

		  for (unsigned int i=1; i<core::workers_count; i++)
			wrqs[i]->wait();
		  for (unsigned int i=1; i<core::workers_count; i++)
			delete wrqs[i];
		  */
	    }

	    inline parallel_for(int from, int to, int step, task * t, int threads) : m_task(t)
	    {
	       range r(from, to, step);
		  int len   = abs((to-from)/step);
		  int parts = threads;
		  if (len>=parts)
		  {
			m_work_count = parts; //core::workers_count; 
		     rgs = r.split_dec(m_work_count);
		     //for (int i=0; i<m_work_count; i++)
			  // rgs[i].debug();
		     m_works  = new work * [m_work_count];
			for (int i=1; i<m_work_count; i++)
			{
			work * w = (work*)(new for_work(t,rgs[i]));
			//core::workers[i]->submit(w);
			m_works[i]  = w;
		     }
		  }
		  else if (len>=1)
		  {
			m_work_count = len; 
		     rgs = r.split_dec(m_work_count);
		     //for (int i=0; i<m_work_count; i++)
			  // rgs[i].debug();
		     m_works  = new work * [m_work_count];
			for (int i=1; i<m_work_count; i++)
			{
			work * w = (work*)(new for_work(t,rgs[i]));
			//core::workers[i]->submit(w);
			m_works[i]  = w;
		     }
		  }
		  else
		  {
			m_work_count = 0;
			//m_works[0]  = (work*)(new for_work(t,r);
		  }
		  /*
		  t->run(rgs[0]);

		  for (unsigned int i=1; i<core::workers_count; i++)
			wrqs[i]->wait();
		  for (unsigned int i=1; i<core::workers_count; i++)
			delete wrqs[i];
		  */
	    }


	    ~parallel_for()
	    {
		  if (m_work_count)
		  {
		  //for (int i=1; i<m_work_count; i++)
		  //	delete m_works[i];
		  }
		  delete [] m_works;
	    }


	    inline void run()
	    {
		  if (!m_work_count)
		  {
			m_task->run(rgs[0]);
			return;
		  }

		  //for (unsigned int i=1; i<core::workers_count; i++)
		  for (int i=1; i<m_work_count; i++)
			core::workers[i%core::workers_count]->submit(m_works[i]);
		  
		  m_task->run(rgs[0]);
		  //rgs[0].debug();

		  //for (unsigned int i=1; i<core::workers_count; i++)
		  for (int i=1; i<m_work_count; i++)
			m_works[i]->wait();
	    }

         inline void run(range& r)
	    {
		  run();
	    }

         inline void run(int i)
	    {
		  run();
	    }

	   void detect_shared()
	   {
	   }

	   void set_shared(pointer p, lockable * l)
	   {
	   }

	   pointers get_pointers()
	   {
		 pointers ptrs;
		 return ptrs;
	   }

	   pointers get_in_data()
	   {
		 pointers ptrs;
		 return ptrs;
	   }

	   pointers get_out_data()
	   {
		 pointers ptrs;
		 return ptrs;
	   }

	 private:

	    //range * m_ranges; 
	    int      m_work_count;
	    work  ** m_works;
	    task  *  m_task;
            ranges   rgs;
	    //int     m_step;
   };




} // namespace xpu

#endif // __XPU_PARALLEL_FOR__

