/**
 * @file spinlock.cc
 * @version 0.1
 * @author Nader KHAMMASSI
 * @date 08/02/2010
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


xpu::core::os::spinlock::spinlock(void)
{  
  pthread_spin_init(&_spinlock, 0); 
}

/*
xpu::core::os::spinlock::spinlock(const spinlock& r_spinlock)
{
   _spinlock = r_spinlock._spinlock;
}
*/ // disallowed

xpu::core::os::spinlock::~spinlock(void) 
{ 
}

void 
xpu::core::os::spinlock::lock() 
{ 
  int r; 
  if ((r = pthread_spin_lock(&_spinlock))) 
  {
    switch(r) {
	  case EINVAL:
	  std::cerr << "error: invalid or uninitialized spinlock." << std::endl;
	  break;
	  case EDEADLK:
	  std::cerr << "error: deadlock : spinlock can not be relocked by his owner." << std::endl;
	  break;
	  default:
	  std::cerr << "error: cannot lock spinlock." << std::endl;
    }
 }   
}

void xpu::core::os::spinlock::unlock() 
{ 
  int r; 
  if ((r = pthread_spin_unlock(&_spinlock))) 
  {
    switch(r) {
	  case EINVAL:
	  std::cerr << "error: invalid or uninitialized spinlock." << std::endl;
	  break;
	  case EPERM:
	  std::cerr << "error: spinlock can be locked only by his owner." << std::endl;
	  break;
	  default:
	  std::cerr << "error: cannot unlock spinlock." << std::endl;
    }
 }  
}

