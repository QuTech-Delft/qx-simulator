/**
 * @file spinlock.h
 * @version 0.1
 * @author Nader KHAMMASSI
 * @date 01/05/2010
 * @description spinlock encapsulation
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


#ifndef __XPU_SPINLOCK_H__
#define __XPU_SPINLOCK_H__


#include <iostream>
#include <pthread.h>

#include <cerrno>

#include <xpu/lockable.h>
#include <xpu/utils.h>

namespace xpu 
{
   namespace core
   {
	 namespace os
	 {

#if !(defined(__linux) || defined(__osf__) || defined(__sun))

	    typedef int pthread_spinlock_t;

	    int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
	       __asm__ __volatile__ ("" ::: "memory");
	       *lock = 0;
	       return 0;
	    }

	    int pthread_spin_destroy(pthread_spinlock_t *lock) {
	       return 0;
	    }

	    int pthread_spin_lock(pthread_spinlock_t *lock) {
	       while (1) {
		  int i;
		  for (i=0; i < 10000; i++) {
		     if (__sync_bool_compare_and_swap(lock, 0, 1)) {
			return 0;
		     }
		  }
		  sched_yield();
	       }
	    }

	    int pthread_spin_trylock(pthread_spinlock_t *lock) {
	       if (__sync_bool_compare_and_swap(lock, 0, 1)) {
		  return 0;
	       }
	       return EBUSY;
	    }

	    int pthread_spin_unlock(pthread_spinlock_t *lock) {
	       __asm__ __volatile__ ("" ::: "memory");
	       *lock = 0;
	       return 0;
	    }

#endif 

	    class spinlock : public lockable
	    {
		  private:
			
			pthread_spinlock_t      _spinlock;

			//__xpu_disallow_copy_and_assign__(spinlock);

		  public:
			inline spinlock(void);
			
			//spinlock(const spinlock& r_spinlock);

			virtual inline ~spinlock(void);

			inline void lock();   

			inline void unlock(); 	   
	    };
	 } // namespace os
   } // namespace core
} // namespace xpu

  #include <xpu/core/os/spinlock.cc>


#endif

