/**
 * @file		stdout.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     print utils
 *
 * @copyright
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
 * @brief 
 *
 */

#ifndef __XPU_STDOUT_H__
#define __XPU_STDOUT_H__

#include <cstdio>
#include <iostream>
#include <cerrno>

#include <xpu/core/os/spinlock.h>
#include <xpu/core/os/mutex.h>

namespace xpu 
{

  #define __print(x) std::cout << x;
  #define __println(x) std::cout << x << std::endl;
 /*
  #define __debug(x) std::cout << "debug : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << std::endl;
  #define __trace(x) std::cout << "trace : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << std::endl;
  #define __error(x) std::cerr << "error : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << std::endl;
  #define __errno(x) std::cerr << "error : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << ":" << strerror(errno) << std::endl;
 */

  #define __debug(x) std::cout << "debug : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << std::endl;
  #define __trace(x) std::cout << "trace : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << std::endl;
  #define __error(x) std::cerr << "error : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << std::endl;
  #define __errno(x) std::cerr << "error : [thread " << pthread_self() << "] " << __FILE__ << " : " << __LINE__ << " : " << __func__ << "() : " << x << ":" << strerror(errno) << std::endl;  
  #ifdef __xpu_debug__
    #define __xpu_trace__(x) __trace(x)
  #else
    #define __xpu_trace__(x) // trace 
  #endif
} // xpu


#endif // __XPU_STDOUT_H__

