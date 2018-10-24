/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		20-01-12
 * @copyright  
 * 
 *  XPU - C++ Parallel  Programming Library for Multicore Architectures
 *
 *  Copyright (C) 2014 Nader Khammassi, All Rights Reserved.
 *
 * This file is part of XPU and has been downloaded from 
 * http://ww.xpu-project.net/.
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
 *     main xpu header file	
 */

#ifndef __XPU_MAIN__
#define __XPU_MAIN__

/*
#if defined(_AIX) || defined(_CRAY) || defined(__irix) || defined(__linux) || defined(__osf__) || defined(__sun) || (defined(__APPLE__) && defined(THR)) || defined(__hpux)
#else                       // windows not supported yet
#error "xpu : platform not supported !"
#endif
*/
#include <unistd.h>
#include <xpu/types.h>
#include <xpu/core/generic_worker.h>

namespace  xpu 
{
   
   /**
    * return thread id
    */ 
   u_int64_t thread_id() { return (u_int64_t)core::os::thread::self(); }

   typedef xpu::core::work * sync;

   namespace core
   {
	 //class xpu::core::generic_worker;
	 //class xpu::core::worker_queue;

	 extern bool                   initialized;
	 extern u_int32_t              workers_count;
	 extern generic_worker **      workers;

	 extern u_int32_t              lasy_workers_count;
      extern bool                   initialized;

      extern xpu::core::generic_worker ** workers;
      extern xpu::core::generic_worker ** lasy_workers;
	 extern xpu::core::work_queue        dynamic_work_queue;

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

	    /**
		* explore
		*    
		*    explore the underlying system to extract hardware information including processors 
		*   count, types (execution capabilities) and processor cache topology... then store hardware 
		*   architecture descriptions to be used by the intelligent run-time system.
		*   
		*    These information are exploited to provide dynamically efficient execution on 
		*   the underlying architecture through intelligent data ppartitionning and cache-aware
		*   task scheduling. Dynamic hardware exploration offer also good forward scalability 
		*   since program scaleup dynamically at run-time without need to re-compile it (very suited
		*   for dynamic hardware systems). 
		*/
	    extern u_int32_t explore(); // extract information : dynamic hardware exloration
	 } // system

	 namespace os
	 {
	    class thread;
	 }

   } // core


   u_int32_t init();
   u_int32_t init(u_int32_t processor_count);
   u_int32_t clean();

   inline bool 
	 initialized() { return xpu::core::initialized; }

} // xpu


inline 
void wait(xpu::sync s)
{
   s->wait();
}

#include <xpu/core/generic_worker.h>

inline
xpu::sync spawn(xpu::task t, u_int32_t processor_id)
{
   processor_id        = processor_id % xpu::core::workers_count;
   xpu::core::work * w = new xpu::core::basic_work(&t);
   xpu::core::workers[processor_id]->submit(w);
   return w;
}

inline
xpu::sync spawn(xpu::task t)
{
   xpu::core::work * w = new xpu::core::basic_work(&t);
   xpu::core::dynamic_work_queue.push(w);
   return w;
}

inline
u_int32_t xpu::core::system::explore()
{
   //xpu::core::system::processor::logical_processor_count = 8;
   //xpu::core::system::processor::graphic_processor_count = 0;
   #if defined(_AIX) || defined(_CRAY) || defined(__irix) || defined(__linux) || defined(__osf__) || defined(__sun)
	 #if defined(__sun) || defined(__linux) || defined(__osf__) || defined(_AIX)
	 xpu::core::system::processor::logical_processor_count       = sysconf(_SC_NPROCESSORS_ONLN);
	 #endif // linux
	 #if defined(_CRAY)
	 xpu::core::system::processor::logical_processor_count       = sysconf(_SC_CRAY_NCPU);        // total number of cpus
	 #endif // cray
	 #if defined(__irix)
	 xpu::core::system::processor::logical_processor_count       = sysconf(_SC_NPROC_ONLN);       // number of active/running cpus 
	 #endif // irix
	 #if defined(__hpux)
	 xpu::core::system::processor::logical_processor_count       = mpctl(MPC_GETNUMSPUS, 0, 0);   // total number of cpus 
	 xpu::core::system::processor::l1_data_cache_line_size       = 64;
	 xpu::core::system::processor::l1_data_cache_size            = 32768;
	 xpu::core::system::processor::l1_inst_cache_line_size       = 32;
	 xpu::core::system::processor::l1_inst_cache_size            = 32768;
	 #endif /* hpux */
	 xpu::core::system::processor::l1_data_cache_line_size        = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);  
	 xpu::core::system::processor::l1_data_cache_size             = sysconf(_SC_LEVEL1_DCACHE_SIZE);      // _SC_LEVEL1_DCACHE_SIZE not in POSIX.x standard ?
	 xpu::core::system::processor::l1_inst_cache_line_size        = sysconf(_SC_LEVEL1_ICACHE_LINESIZE);  // _SC_LEVEL1_ICACHE_LINESIZE not in POSIX.x standard ?
	 xpu::core::system::processor::l1_inst_cache_size             = sysconf(_SC_LEVEL1_ICACHE_SIZE);      // _SC_LEVEL1_ICACHE_SIZE not in POSIX.x standard ?
   #elif defined(__APPLE__)
	 xpu::core::system::processor::logical_processor_count        = sysconf( _SC_NPROCESSORS_ONLN ); // MPProcessorsScheduled();              // number of active/running cpus 
	 xpu::core::system::processor::l1_data_cache_line_size        = 64;
	 xpu::core::system::processor::l1_data_cache_size             = 32768;
	 xpu::core::system::processor::l1_inst_cache_line_size        = 32;
	 xpu::core::system::processor::l1_inst_cache_size             = 32768;
   #else
	 xpu::core::system::processor::logical_processor_count        = 2;
	 xpu::core::system::processor::l1_data_cache_line_size        = 64;
	 xpu::core::system::processor::l1_data_cache_size             = 32768;
	 xpu::core::system::processor::l1_inst_cache_line_size        = 32;
	 xpu::core::system::processor::l1_inst_cache_size             = 32768;
   #endif

   return xpu::core::system::processor::logical_processor_count;
}

/**
 * init
 */
inline
u_int32_t xpu::init()
{
    try{
   if (xpu::core::initialized)
	 return 0;
   // exlore hardware architecture and capabilities
   xpu::core::system::explore();

   xpu::core::workers_count      = xpu::core::system::processor::logical_processor_count * 2;
   xpu::core::lasy_workers_count = xpu::core::system::processor::logical_processor_count;
   xpu::core::workers      = new xpu::core::generic_worker * [core::workers_count];
   xpu::core::lasy_workers = new xpu::core::generic_worker * [core::lasy_workers_count];

   //printf("[+] initializing xpu...\n");

   for (int i=0; i<core::workers_count; i++)
   {
	 xpu::core::workers[i] = new xpu::core::generic_worker(i%xpu::core::system::processor::logical_processor_count,new xpu::core::work_queue());
	 xpu::core::workers[i]->start();
   }

   for (int i=0; i<core::lasy_workers_count; i++)
   {
	 xpu::core::lasy_workers[i] = new xpu::core::generic_worker(i%xpu::core::system::processor::logical_processor_count, &core::dynamic_work_queue);
	 xpu::core::lasy_workers[i]->start();
   }

   xpu::core::initialized = true;
   }
   catch(...){
        std::cerr << "Init failed 0." << std::endl;
    }
   //printf("[+] initialized.\n");
   
   return 0;
}

/**
 * init
 */
inline
u_int32_t xpu::init(u_int32_t processor_count)
{
    try{
   if (xpu::core::initialized)
	 return 0;
   // exlore hardware architecture and capabilities
   xpu::core::system::explore();

   // set workers count
   xpu::core::workers_count      = processor_count * 2;
   xpu::core::lasy_workers_count = processor_count * 2; //xpu::system::processor::logical_processor_count;
   xpu::core::workers      = new xpu::core::generic_worker * [core::workers_count];
   xpu::core::lasy_workers = new xpu::core::generic_worker * [core::lasy_workers_count];

   printf("[+] initializing xpu...\n");

   for (int i=0; i<core::workers_count; i++)
   {
	 core::workers[i] = new xpu::core::generic_worker(i%processor_count,new xpu::core::work_queue());
	 core::workers[i]->start();
   }

   for (int i=0; i<core::lasy_workers_count; i++)
   {
	 core::lasy_workers[i] = new xpu::core::generic_worker(i%processor_count, &core::dynamic_work_queue);
	 core::lasy_workers[i]->start();
   }

   xpu::core::initialized = true;
   
   printf("[+] initialized.\n");
   }
   catch(...){
        std::cerr << "Init failed 1." << std::endl;
    }
   return 0;
}

/**
 * stop runtime
 */
inline
u_int32_t xpu::clean()
{
    try{
   // stop tam and cleanup memory
   //__debug("stopping workers...");
   for (u_int32_t i=0; i<xpu::core::workers_count; i++)
   {
     try{
	 xpu::core::workers[i]->stop();
	 xpu::core::workers[i]->join(); }
	 catch(...) { continue; } 
   }

   for (u_int32_t i=0; i<xpu::core::workers_count; i++)
   {
     try{
	 xpu::core::lasy_workers[i]->stop();
	 xpu::core::dynamic_work_queue.deactivate();
	 xpu::core::lasy_workers[i]->join(); }
	 catch(...) { continue; } 
   }
   //__debug("cleanup memory...");
   for (u_int32_t i=0; i<xpu::core::workers_count; i++)
   {
     try{
	 delete xpu::core::workers[i];
	 delete xpu::core::lasy_workers[i];
    }
    catch(...)
    {continue;}
   }
   //__debug("cleanup done.");
    }
    catch(...){
        std::cerr << "Cleaning failed." << std::endl;
        return 0;
    }
   return 0;
}

#include <xpu/parallel_tasks.h>
#include <xpu/sequential_tasks.h>
#include <xpu/pipeline.h>
#include <xpu/parallel_for.h>


#endif // __XPU_MAIN__
