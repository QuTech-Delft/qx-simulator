/**
 * @file event_listener.h
 * @version 0.1
 * @author Nader KHAMMASSI
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


#ifndef __XPU_EVENT_LISTENER_H__
#define __XPU_EVENT_LISTENER_H__

#include <xpu/core/os/event.h>
#include <xpu/core/os/thread.h>

#include <xpu/task.h>
#include <xpu/range.h>
#include <xpu/core/generic_worker.h>


using namespace xpu::core::os;

namespace xpu
{
   namespace core
   {
	 class event_listener : public thread
	 {
	    public:
		 
		 event_listener(task * tsk, event * evt, int iterations) : thread(tsk) 
		 {
		    m_task       = tsk;
		    m_iterations = iterations;
		    m_event      = evt;
			
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
			  event_listener * t = (event_listener*)args;
			  //t->m_cpuset.only(0);
			  t->m_cpuset.setup();
			  int iter = t->m_iterations;
			  
			  //std::cout << "[+] listener : running on core " << sched_getcpu() << std::endl; 
			  
			  if (!iter)
			  {
				t->m_event->wait();
				t->m_task->run();
			  }
			  else if (iter<0)  // infinite loop
			  {
				iter = 0;
				while (true)
				{
				   
				   pipe_work w((task *)(t->m_task), iter);
				   t->m_event->wait();
				   core::workers[iter%core::workers_count]->submit(&w);
				   w.wait();
				   //t->m_task->run(iter);
				   iter++;
				}
			  }
			  else
			  {
				//while (iter--)
				for (int i=0; i<iter; i++)
				{
				   
				   #if (defined(PIPELINE_CACHE_AWARE) || defined(PIPELINE_LOAD_BALANCE)) 
				   pipe_work w((task *)(t->m_task), i);
				   #endif
				   t->m_event->wait();
				   #ifdef PIPELINE_CACHE_AWARE
				   core::workers[i%core::workers_count]->submit(&w);
				   w.wait();
				   #elif PIPELINE_LOAD_BALANCE
				   core::dynamic_work_queue.push(&w);
				   w.wait();
				   #else
				   t->m_task->run(i);
                       #endif
				}
			  }
			  return NULL;
		   }

	    private:
		 
		 int     m_iterations;
		 event * m_event;
	 };
   } // core
} // xpu


#endif // __XPU_LISTENER_H__

