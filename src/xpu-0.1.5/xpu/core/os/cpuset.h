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
#else
//  #error "xpu::cpu::set : platform not supported ! "
#endif // __unix__

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

