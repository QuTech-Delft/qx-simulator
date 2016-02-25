/**
 * @file mutex.h
 * @version 0.1
 * @author Nader KHAMMASSI
 * @date 08/02/2010
 * @description mutex encapsulation
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




#ifndef __XPU_MUTEX_79424B4E0A_H__
#define __XPU_MUTEX_79424B4E0A_H__

#include <xpu/lockable.h>

namespace xpu
{
   namespace core
   {
	 namespace os
	 {

	    class mutex : public lockable
	    {
		private:
		    pthread_mutex_t 	    m_mutex;
		    pthread_t 		    m_owner;
		    pthread_mutexattr_t  m_attr;

		public:
		    inline mutex(void);
		    inline ~mutex(void);

		    inline void lock();
		    inline void unlock();
	    };
	 } // namespace os
   } // namespace core
} // namespace xpu

   #include <xpu/core/os/mutex.cc>


#endif // __XPU_MUTEX_79424B4E0A_H__

