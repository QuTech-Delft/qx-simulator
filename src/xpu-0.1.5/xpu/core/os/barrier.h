/**
 * @file barrier.h
 * @version 0.1
 * @author Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
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


#ifndef __XPU_BARRIER_H__
#define __XPU_BARRIER_H__

#include <pthread.h>

#include "mac_osx_pthread.h"

#include <xpu/types.h>

namespace xpu 
{   
   namespace core
   {
	 namespace os
	 {
	    class barrier
	    {
		private: 

		 pthread_barrier_t *     n_barrier_id;
		 pthread_barrierattr_t   n_attr;
		 u_int32_t            n_count;
		
		public:
		 
		 /**
		  * constructor
		  */ 
		 inline barrier(u_int32_t count); 

		 /**
		  * destructor
		  */ 
		 //virtual ~barrier();
		 inline ~barrier();
		 
		 /**
		  *  accessors
		  */ 
		 
		 inline pthread_barrier_t * id();

		 inline u_int32_t get_count();
		
		 inline void set_count(u_int32_t count);

		 /**
		  * init
		  */
		
		 inline void init();

		 /**
		  * collegues : )
		  */ 

		 friend void  wait();
		 friend void  run();
		 friend class thread;
		 friend class event_listener;
		 friend class event_notifier;
		 friend class event_handler;

	    };
	 } // namespace os
   } // namespace core
} // namespace xpu

#include "barrier.cc"

#endif // __XPU_BARRIER_H__  
