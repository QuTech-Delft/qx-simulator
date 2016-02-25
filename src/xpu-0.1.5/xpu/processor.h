/**
 * @file		processor.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     processor information extraction: core count, topology, cache information ...
 *             in this version we uses sysconf() service available on unix-like systems ...
 *             a more advanced implementation uses 'cpuid' instruction to get more accurate and detailed 
 *             information on x86 architectures (intel/amd)
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

#ifndef __XPU_PROCESSOR_H__
#define __XPU_PROCESSOR_H__

#if defined(_AIX) || defined(_CRAY) || defined(__irix) || defined(__linux) || defined(__osf__) || defined(__sun)
#include <unistd.h>         //  sysconf() used by most unix systems 
#elif defined(__APPLE__) && defined(THR)
#include <Carbon/Carbon.h>  // carbon apis for multiprocessing 
#elif defined(__hpux)
#include <sys/mpctl.h>      // hp-ux multiprocessing headers 
#else                       // windows not supported yet
#error "xpu::system::processor : platform not supported ! (this version support only unix-like platform)"
#endif

/**
 * if compiling on Linux, enable the GNU CPU affinity functions in both
 * libc and the libpthreads
 **/
#if defined(__linux)
#define _GNU_SOURCE 1
#include <sched.h>
#endif



namespace xpu
{
   namespace system
   {
	 namespace processor
	 {

	    unsigned int    logical_processors_count        = 0;     // logical processor count (HT/SMT included)  

	    unsigned int    l1_data_cache_size              = 0;     // l1 data cache size
	    unsigned int    l1_data_cache_line_size         = 0;     // l1 data cache line size

	    unsigned int    l1_instruction_cache_size       = 0;     // l1 instruction cache size
	    unsigned int    l1_instruction_cache_line_size  = 0;     // l1 instruction cache line size

	    /**
	     * explore
	     *   get information on available processors, their toplogy, capabilities...
	     */
	    void explore();
	 }
   }
}
	 
#endif // __XPU_PROCESSOR_H__

