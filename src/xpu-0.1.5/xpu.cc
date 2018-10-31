/**
 * @file
 * @author  Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date        1-03-13
 * @copyright  
 *  
 *  Copyright (c) 2011 Nader Khammassi, http://xpu-project.net/
 *                                                                          
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *                                                                          
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *                                                                          
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  
 *
 * @brief 
 *    xpu runtime   
 */

// FIXME: In the current deployment environment, we don't ever use this file. -KKL


#include "config.h"
#include <xpu.h>

//internal components
//#include <xpu/core/os/thread.h>
#include <xpu/core/generic_worker.h>

namespace  xpu 
{
   namespace core
   {
     //class generic_worker;
     //class worker_queue;

     bool                   initialized   = false;
     
     u_int32_t              workers_count = 0;
     u_int32_t              lasy_workers_count;
      
     xpu::core::generic_worker ** workers;
      xpu::core::generic_worker ** lasy_workers;
     xpu::core::work_queue        dynamic_work_queue;

     namespace system  // hardware information
     {
        namespace processor  // processing units information
        {
          u_int32_t logical_processor_count;
          u_int32_t graphic_processor_count;
          u_int32_t l1_data_cache_line_size;
          u_int32_t l1_inst_cache_line_size;
          u_int32_t l1_data_cache_size;
          u_int32_t l1_inst_cache_size;
          // processor cache hierarchy : see "unified multicore model" paper
        }

        u_int32_t explore(); // extract information : dynamic hardware exloration
     } // system
   } // core
} // xpu

#ifdef inlined_section
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
     xpu::core::system::processor::logical_processor_count        = MPProcessorsScheduled();              // number of active/running cpus 
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
 *
 */
u_int32_t xpu::init()
{
    try{
   if (xpu::core::initialized)
     return 0;
   // exlore hardware architecture and capabilities
   xpu::core::system::explore();

   xpu::core::workers_count      = xpu::system::processor::logical_processors_count * 2;
   xpu::core::lasy_workers_count = xpu::system::processor::logical_processors_count;
   xpu::core::workers      = new generic_worker * [core::workers_count];
   xpu::core::lasy_workers = new generic_worker * [core::lasy_workers_count];

   printf("[+] initializing xpu...\n");

   for (int i=0; i<core::workers_count; i++)
   {
     core::workers[i] = new generic_worker(i%system::processor::logical_processors_count,new work_queue());
     core::workers[i]->start();
   }

   for (int i=0; i<core::lasy_workers_count; i++)
   {
     core::lasy_workers[i] = new generic_worker(i%system::processor::logical_processors_count, &core::dynamic_work_queue);
     core::lasy_workers[i]->start();
   }

   xpu::core::initialized = true;
   
   printf("[+] initialized.\n");
    }
    catch(...){
        std::cerr << "Init failed 0." << std::endl;
    }
   
   return 0;
}

/**
 *
 */
u_int32_t xpu::init(u_int32_t processor_count)
{
    try{
   if (xpu::core::initialized)
     return 0;
   // exlore hardware architecture and capabilities
   xpu::core::system::explore();

   // set workers count
   xpu::core::workers_count      = processors_count * 2;

   xpu::core::workers_count      = xpu::system::processor::logical_processors_count * 2;
   xpu::core::lasy_workers_count = xpu::system::processor::logical_processors_count;
   xpu::core::workers      = new generic_worker * [core::workers_count];
   xpu::core::lasy_workers = new generic_worker * [core::lasy_workers_count];

   printf("[+] initializing xpu...\n");

   for (int i=0; i<core::workers_count; i++)
   {
     core::workers[i] = new generic_worker(i%system::processor::logical_processors_count,new work_queue());
     core::workers[i]->start();
   }

   for (int i=0; i<core::lasy_workers_count; i++)
   {
     core::lasy_workers[i] = new generic_worker(i%system::processor::logical_processors_count, &core::dynamic_work_queue);
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
u_int32_t xpu::clean()
{
    try{
   // stop tam and cleanup memory
   //__debug("stopping workers...");
   for (u_int32_t i=0; i<xpu::core::workers_count; i++)
   {
     xpu::core::workers[i]->stop();
     try {xpu::core::workers[i]->join(); }
     catch(...) { continue; } 
   }

   for (u_int32_t i=0; i<xpu::core::workers_count; i++)
   {
     xpu::core::lazy_workers[i]->stop();
     try {xpu::core::lazy_workers[i]->join(); }
     catch(...) { continue; } 
   }
   //__debug("cleanup memory...");
   for (u_int32_t i=0; i<xpu::core::workers_count; i++)
   {
     delete xpu::core::workers[i];
     delete xpu::core::lazy_workers[i];
   }
    }
    catch(...){
        std::cerr << "Cleaning failed." << std::endl;
        continue;
    }
   //__debug("cleanup done.");
   return 0;
}

#endif // 0
