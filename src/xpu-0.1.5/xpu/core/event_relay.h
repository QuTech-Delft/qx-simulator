/**
 * @file    event_relay.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @date    15/03/2010
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

#ifndef __XPU_EVENT_RELAY_H__
#define __XPU_EVENT_RELAY_H__

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
	 class event_relay : public thread
	 {
	    public:
		 
		 event_relay(task * tsk, event * lst_evt, event * ntf_evt, int iterations) : thread(tsk) 
		 {
		    m_task       = tsk;
		    m_lst_event  = lst_evt;
		    m_ntf_event  = ntf_evt;
		    m_iterations = iterations;
		 }

		 event * get_listen_event()
		 {
		    return m_lst_event;
		 }

		 void    set_listen_event(event * evt)
		 {
		    m_lst_event = evt;
		 }
		 
		 event * get_notify_event()
		 {
		    return m_ntf_event;
		 }

		 void    set_notify_event(event * evt)
		 {
		    m_ntf_event = evt;
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
			  event_relay * t = (event_relay*)args;
			  //t->m_cpuset.only(0);
			  t->m_cpuset.setup();
			  int iter = t->m_iterations;

			  //std::cout << "[+] relay : running on core " << sched_getcpu() << std::endl; 
			  
			  if (!iter)
			  {
				t->m_lst_event->wait();
				t->m_task->run();
				t->m_ntf_event->fire();
			  }
			  else if (iter<0)  // infinite loop
			  {
				iter = 0;
				while (true)
				{
				   
				   pipe_work w((task *)(t->m_task), iter);
				   t->m_lst_event->wait();
				   core::workers[iter%core::workers_count]->submit(&w);
				   w.wait();
				   //t->m_task->run(iter);
				   t->m_ntf_event->fire();
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
				   t->m_lst_event->wait();
				  #ifdef PIPELINE_CACHE_AWARE
				   core::workers[i%core::workers_count]->submit(&w);
				   w.wait();
				  #elif PIPELINE_LOAD_BALANCE
				   core::dynamic_work_queue.push(&w);
				   w.wait();
				  #else
				   t->m_task->run(i);
                      #endif
				   t->m_ntf_event->fire();
				}
			  } 


			  return NULL;
		   }

	    private:

		 event * m_lst_event;
		 event * m_ntf_event;
		 int     m_iterations;
	 };

   } // core
} // xpu 

#endif // __XPU_EVENT_RELAY_H__


