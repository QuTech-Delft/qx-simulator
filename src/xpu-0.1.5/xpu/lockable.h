/**
 * @file    lockable.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @date    15/03/2010
 * @brief   common lockable interface 
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
 */

#ifndef __XPU_LOCKABLE_79424B4E0A_H__
#define __XPU_LOCKABLE_79424B4E0A_H__

#include <pthread.h>

namespace xpu
{
   class lockable
   {
	 public:
	  lockable()              {} ;
	  virtual ~lockable()     {} ;
	  virtual void lock()     = 0;
	  virtual void unlock()   = 0;
	  //virtual bool try_lock() const = 0;
   };

} // namespace xpu 

#endif // __XPU_LOCKABLE_79424B4E0A_H__
