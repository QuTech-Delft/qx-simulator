/**
 * @file     thread.h
 * @version  0.2
 * @author   Nader KHAMMASSI
 * @contact  nader.khammassi@gmail.com
 * @date     05/02/2010
 * 
 * @copyright
 *
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


#ifndef __XPU_THREAD_79424B4E0A_H__
#define __XPU_THREAD_79424B4E0A_H__


#include <pthread.h>

// internal components
#include <xpu/core/os/barrier.h>
#include <xpu/core/os/cpuset.h>

// public components
#include <xpu/exception.h>
#include <xpu/task_group.h>



namespace xpu
{
   namespace core
   {
	 namespace os
	 {
	    class cpuset;
	    /**
		* @class thread
		* @brief thread support implementation for c++98 and c++0x
		*/
	    class thread 
	    {
		  protected:

			typedef pthread_t __xpu_handle; 
			typedef void *  (*__xpu_task)(void *);
			//typedef int task;

		  protected:

			// =========== thread attr ===========

			pthread_attr_t    m_attr;
			pthread_t         m_id;
			cpuset            m_cpuset;

			bool              m_running;

			void *            m_args;
			void *            m_exit_status;
			int               m_core;

			// ========= join th info ============

			pthread_t         m_join_id;
			void **           m_join_result;

			// ===== Synchronization tools =======

			barrier *         m_barrier;
			//vector<mutex *> __mutexes;


		  protected:

			/**
			 * execute task on witch thread is spawn.
			 * @param args arguments
			 * @return offset of task result
			 */ 
			static 
			   inline void * run(void * arg); 

			task_group *             m_task;

		  public:

			// void * (*_function)(void *);

			/**
			 * constructor
			 */ 
			explicit 
			   inline thread(task_group * tsk);

			/**
			 * destructor
			 */
			inline
			   ~thread();

			inline
			   task_group *  get_task() { return m_task; }  

			inline
			   void        set_task(task_group * tsk);  

			inline  
			   pthread_t  id();

			inline  
			   pthread_attr_t get_attributes();

			inline 
			   void * get_exit_status();

			inline 
			   void   set_exit_status(void * status);


			inline 
			   barrier * get_barrier();

			inline 
			   void set_barrier(barrier * br);

			inline 
			   void remove_barrier();


			inline 
			   pthread_t  get_join_id();

			inline 
			   void **    get_join_result();

			/*
			   inline 
			   void       add_mutex(mutex * m);

			   inline 
			   vector<mutex *> * get_mutexes();
			   */

			/**
			 * force thread to run on core n° <cpu>
			 * @cpu core index
			 */ 
			inline 
			   void set_core(int core); 

			inline 
			   void set_cpuset(cpuset cs)
			   {
				 m_cpuset = cs;
			   }


			inline
			   static int get_core()
			   {
				 //return sched_getcpu(); 
				 return 0; 
			   }

			/**
			 * create thread on run function on core n° <cpu>
			 */ 
			inline 
			   void start() throw (xpu::exception);

			/*
			   inline 
			   void stop()
			   {
			   pthread_cancel(m_id);
			   }
			   */

			/**
			 * join thread identified by <id>
			 * @id joinable thread id
			 * @join_result join task result
			 */ 
			inline 
			   void join(pthread_t id, void ** join_result=NULL); 

			inline 
			   void join() throw (xpu::exception); 

#if defined(__linux) || defined(__osf__) || defined(__sun)
			inline 
			   int try_join(long timout);
#endif


			// =========== static utils functions ===============

			// Applyed to calling thread ...

			//static void wait(barrier * barrier); 

			// wait for all no detached threads then exit..
			static inline 
			   void wait_for_all_threads();


			static inline
			pthread_t self() { return pthread_self(); }

			// join thread t
			static inline 
			   void join(thread *t) throw (xpu::exception);

			// TO DO: add accessors to attributes: joinable, detached ...
	    };

	 } // namespace os
   } // namespace core
} // namespace xpu

#include "thread.cc"

#endif // __XPU_THREAD_79424B4E0A_H__ 

