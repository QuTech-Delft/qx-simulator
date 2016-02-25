/**
 * @file    event_notifier.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
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



#ifndef __XPU_EVENT_NOTIFIER_H__
#define __XPU_EVENT_NOTIFIER_H__

#include <xpu/core/os/event.h>
#include <xpu/core/os/thread.h>

#include <xpu/task.h>
#include <xpu/range.h>

#include <xpu/core/generic_worker.h>



namespace xpu
{
   using namespace xpu::core::os;
   
   namespace core
   {
	 class event_notifier : public thread
	 {
	    public:
		 
		 event_notifier(task * tsk, event * evt, int iterations) : thread(tsk) 
		 {
		    m_task       = tsk;
		    m_event      = evt;
		    m_iterations = iterations;
		 }

		 event * get_event()
		 {
		    return m_event;
		 }

		 void    set_event(event * evt) 
		 {
		    m_event = evt;
		 }

		 task_group * get_task()
		 {
		    return m_task;
		 }

		 void set_task(task * tsk)
		 {
		    m_task = tsk;
		 }


		 void start()
		 {
		    pthread_create(&m_id, &m_attr, reinterpret_cast<__xpu_task>(&run), (void*)this); 
		 }

	    protected:
		 
		 static
		   void * run(void * args)
		   {
			  event_notifier * t = (event_notifier*)args;
			  //t->m_cpuset.only(0);
			  t->m_cpuset.setup();
			  int iter = t->m_iterations;
			  
			  //std::cout << "[+] notifier : running on core " << sched_getcpu() << std::endl; 
			  
			  if (!iter)
			  {
				t->m_task->run();
				t->m_event->fire();
			  }
			  else if (iter<0)  // infinite loop
			  {
				iter = 0;
				while (true)
				{
				   
				   pipe_work w((task *)(t->m_task), iter);
				   core::workers[iter%core::workers_count]->submit(&w);
				   w.wait();
				   //t->m_task->run(iter);
				   iter++;
				   t->m_event->fire();
				}
			  }
			  else
			  {
				//while (iter--)
				for (int i=0; i<iter; i++)
				{
				  #ifdef PIPELINE_CACHE_AWARE 
				   pipe_work w((task *)(t->m_task), i);
				   core::workers[i%core::workers_count]->submit(&w);
				   w.wait();
				  #elif PIPELINE_LOAD_BALANCE
				   pipe_work w((task *)(t->m_task), i);
				   core::dynamic_work_queue.push(&w);
				   w.wait();
				  #else
				   t->m_task->run(i);
				  #endif // PIPELINE_V2
				   t->m_event->fire();
				}
			  }
			  return NULL;
		   }

	    private:

		 event * m_event;
		 int     m_iterations;
	 };
   } // core
} // xpu

#endif // __XPU_NOTIFIER_H__

