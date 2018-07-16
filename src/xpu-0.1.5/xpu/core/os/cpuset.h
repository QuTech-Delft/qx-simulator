/**
 * @file cpuset.h
 * @version 0.1
 * @author Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @description cpu_set is high-level implementation of processors mask, it provides
 *              flexible task/processor mapping
 * @date 11/02/2010
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



#ifndef __XPU_CPU_SET_H__
#define __XPU_CPU_SET_H__

#include <cstdio>
//#include <xpu.h>
#include <xpu/types.h>
#include <xpu/core/generic_worker.h>

//#ifdef __unix__
#if defined(__linux) || defined(__osf__) || defined(__sun) 
  //#define _GNU_SOURCE  
  #include <sched.h>
  #define xpu_cpu_set(cpu, cs)\
  	CPU_SET(cpu, cs)
  #define xpu_cpu_init(cs)\
  	CPU_ZERO(cs)

#elif __APPLE__

#include <mach/thread_policy.h>
#include <mach/task_info.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#include <pthread.h>

int get_thread_id();
int numphyscpus();
int pthread_create_with_cpu_affinity(pthread_t *__restrict thread, int cpu,
const pthread_attr_t *__restrict attr, void *(*start_routine)(void *), void *__restrict arg);

int get_thread_id() {
   return pthread_mach_thread_np(pthread_self());
}

int numphyscpus() {
   int numcpus;
   size_t sizeofint = sizeof(numcpus);
   if (-1 == sysctlbyname("hw.physicalcpu", &numcpus, &sizeofint, NULL, 0)) perror("sysctl");
   return numcpus;
}

int pthread_create_with_cpu_affinity(pthread_t * thread, int cpu, const pthread_attr_t * attr, void *(*start_routine)(void *), void * arg) 
{
   thread_affinity_policy_data_t policy_data = { cpu };
   int rv = pthread_create_suspended_np(thread, attr, start_routine, arg);
   mach_port_t mach_thread = pthread_mach_thread_np(*thread);
   if (rv != 0) {
      return rv;
   }
   thread_policy_set(*(thread_t*)thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy_data, THREAD_AFFINITY_POLICY_COUNT);
   thread_resume(mach_thread);
   return 0;
}

int set_realtime(int period, int computation, int constraint) {
   struct thread_time_constraint_policy ttcpolicy;
   int ret;
   thread_port_t threadport = pthread_mach_thread_np(pthread_self());

   ttcpolicy.period=period; // HZ/160
   ttcpolicy.computation=computation; // HZ/3300;
   ttcpolicy.constraint=constraint; // HZ/2200;
   ttcpolicy.preemptible=1;

   if ((ret=thread_policy_set(threadport,
	       THREAD_TIME_CONSTRAINT_POLICY, (thread_policy_t)&ttcpolicy,
	       THREAD_TIME_CONSTRAINT_POLICY_COUNT)) != KERN_SUCCESS) {
      fprintf(stderr, "set_realtime() failed.\n");
      return 0;
   }
   return 1;
}


#else

   #error "xpu::cpu::set : platform not supported ! "

#endif

namespace xpu
{
   namespace core
   {
	 //class generic_worker;
	 //class worker_queue;

	 extern u_int32_t              workers_count;

	 extern u_int32_t              lasy_workers_count;

	 namespace system  // hardware information
	 {
	    namespace processor  // processing units information
	    {
		  extern u_int32_t logical_processor_count;
		  extern u_int32_t graphic_processor_count;
		  extern u_int32_t l1_data_cache_line_size;
		  extern u_int32_t l1_inst_cache_line_size;
		  extern u_int32_t l1_data_cache_size;
		  extern u_int32_t l1_inst_cache_size;
		  // processor cache hierarchy : see "unified multicore model" paper
	    }
	 }

	 namespace os
	 {
	    class cpuset
	    {
		  public:

		  cpuset()
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			xpu_cpu_init(&m_cpu_set);
#endif
		  }

		  cpuset(int cpu)
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			xpu_cpu_init(&m_cpu_set);
			xpu_cpu_set(cpu%(xpu::core::system::processor::logical_processor_count), &m_cpu_set);
#endif
		  }

		  inline void allow(int cpu)
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			xpu_cpu_set(cpu%(xpu::core::system::processor::logical_processor_count), &m_cpu_set);
#endif
		  }

		  inline void allow_all()
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			for (unsigned int i=0; i<xpu::core::system::processor::logical_processor_count; i++)
			{
			   xpu_cpu_set(i, &m_cpu_set);
			}
#endif
		  }

		  inline void only(int cpu)
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			xpu_cpu_init(&m_cpu_set);
			xpu_cpu_set(cpu%(xpu::core::system::processor::logical_processor_count), &m_cpu_set);
#endif 
		  }

		  inline void clear()
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			xpu_cpu_init(&m_cpu_set);
#endif
		  }

		  inline void setup()
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &m_cpu_set);
#endif
		  }

		  inline void setup(pthread_t id, cpuset cs)
		  {
#if defined(__linux) || defined(__osf__) || defined(__sun) 
			pthread_setaffinity_np(id, sizeof(cpu_set_t), &cs.m_cpu_set);
#endif
		  }

		  private:

#if defined(__linux) || defined(__osf__) || defined(__sun) 
		  cpu_set_t m_cpu_set;
#endif

	    };
	 } // namespace os
   } // namespace core
} // namespace xpu

#endif // __XPU_CPU_SET_H__

