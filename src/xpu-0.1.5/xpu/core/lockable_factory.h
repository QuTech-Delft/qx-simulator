/**
 * @file    lockable_factory.h
 * @author  Nader KHAMMASSI
 * @date    08/04/10
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

#ifndef __XPU_LOCKABLE_FACTORY_H__
#define __XPU_LOCKABLE_FACTORY_H__

#include <map>

#include <xpu/lockable.h>
#ifdef __XPU_USE_SPINLOCK__
 #include <xpu/core/os/spinlock.h>
#else
 #include <xpu/core/os/mutex.h>
#endif

using namespace xpu::core::os;

/**
 * @class lockable_factory 
 * @brief
 *   lockable_factory must be used as the unique (spin or mutex) factory, 
 *  it's implemented as a singleton to ensure unique association of a lock 
 *  to each given pointer. this factory is designed to avoid instanciation 
 *  of multiples mutexes for a same resource to ensure data protection efficency.
 */
namespace xpu
{
   namespace core
   {
   class lockable_factory
   {
    typedef void *  pointer;

    private:

	 static lockable_factory *     m_instance;
	 std::map<pointer, lockable*>  m_shared;

	 lockable_factory() { }

    public:
	 static lockable_factory * instance();
	
	 ~lockable_factory();

	 // get unique mutex for pointer if exists else create new one 
	 lockable * getlockable(pointer p);   

   };

 #include <xpu/core/lockable_factory.cc>

   } // core
} // namespace xpu

#endif // __XPU_LOCKABLE_FACTORY_H__

