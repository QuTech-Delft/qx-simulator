/**
 * @file    thread.cc 
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @date    
 * @brief   thread implementation
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

/**
 * constructor
 */ 
thread::thread(task_group *cb): m_id(0),
   m_running(false),
   m_core(-1), // not defined, let the kernel choose it for us... (default)
   m_join_id(0),
   m_join_result(0),
   //m_barrier(0),
   m_task(cb)
{
   pthread_attr_init(&m_attr);
   //CPU_ZERO(&m_cpuset);
   /*
	 for (int i=0; i<__xpu_processor__.count(); i++)
	 {
	 CPU_SET(i, &m_cpuset);
	 }
	 */
   m_cpuset.allow_all();
} 


/**
 * destructor
 */
thread::~thread()
{
   return;
}

/*
   task * 
   thread::get_task()
   {
   return m_task;
   }
   */

   void
thread::set_task(task_group * cb)
{
   m_task = cb;
}

/**
 * id
 *   return thread id if running else -1
 */
   pthread_t  
thread::id()
{
   if (m_running) 
	 return pthread_self();
   else 
	 return 0;
}

   pthread_attr_t    
thread::get_attributes()
{
   return m_attr;
}

   void * 
thread::get_exit_status()
{
   return m_exit_status;
}

   void
thread::set_exit_status(void * exit_status)
{
   m_exit_status = exit_status;
}


   barrier * 
thread::get_barrier()
{
   return m_barrier;
}

   void 
thread::set_barrier(barrier * _barrier )
{
   m_barrier = _barrier;
}

   void 
thread::remove_barrier()
{
   m_barrier = NULL;
}


   pthread_t
thread::get_join_id()
{
   return m_id;
}

   void **
thread::get_join_result()
{
   return m_join_result;
}



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
   void 
thread::set_core(int core)
{
   m_core = core;
   m_cpuset.only(core);
}


/**
 * create thread on run function on core n° <cpu>
 */ 
   void 
thread::start() throw (xpu::exception)
{
   if (pthread_create(&m_id, &m_attr, reinterpret_cast<__xpu_task>(&run), (void*)this) != 0)
	 throw (xpu::exception("thread::start() : pthread_create() failed ",true)); 

}

/**
 * join thread identified by <id>
 * @id joinable thread id
 * @join_result join task result
 */ 
   void 
thread::join(pthread_t id, void ** join_result)
{
   m_join_id = id; 
   m_join_result = join_result; 
}


   void 
thread::join() throw (xpu::exception)
{
   if (pthread_join(m_id, NULL) != 0)
	 throw (xpu::exception("thread::join() : pthread_join() failed ",true)); 
}


#if defined(__linux) || defined(__osf__) || defined(__sun)
   int
thread::try_join(long timeout) 
{
   struct timespec ts;
   int s;
   if (clock_gettime(CLOCK_REALTIME, &ts) == -1) 
   {  /* Handle error */ }

   //ts.tv_sec += 5;
   ts.tv_nsec += timeout;
   s = pthread_timedjoin_np(m_id, NULL, &ts);
   return s;
}
#endif

// =========== static utils functions ===============

// Applyed to calling thread ...

//void 
//thread::wait(barrier * barrier); 

// wait for all no detached threads then exit..
   void 
thread::wait_for_all_threads()
{
   pthread_exit(NULL);
}

// join thread t
   void 
thread::join(thread *t) throw (xpu::exception)
{
   if (pthread_join(t->m_id, t->m_join_result) != 0) 
	 throw (xpu::exception("thread::join() : pthread_join() failed ",true)); 
}

   void * 
thread::run(void *args) 
{
   //cpu_set_t cpuset;
   //CPU_ZERO(&cpuset);
   //CPU_SET(j&cpuset);

   thread * t = (thread*)args;
   t->m_cpuset.setup();
   t->m_task->run();
   return NULL;
} 

