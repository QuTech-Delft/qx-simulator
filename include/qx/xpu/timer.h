/**
 * @file		timer.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     accurate timer
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

#ifndef __XPU_TIMER_H__
#define __XPU_TIMER_H__

#include <time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

/**
#ifdef __MACH__
#include <sys/time.h>
//clock_gettime is not implemented on OSX
int clock_gettime(int clk_id, struct timespec* t) {
   struct timeval now;
   int rv = gettimeofday(&now, NULL);
   if (rv) return rv;
   t->tv_sec  = now.tv_sec;
   t->tv_nsec = now.tv_usec * 1000;
   return 0;
}
#endif
*/

namespace xpu 
{
   class timer
   {
    public:

	 timer():__elapsed(0)
	 { 
	   __start_t.tv_nsec = 0; 
	   __start_t.tv_sec = 0;
	   __end_t.tv_nsec = 0;  
	   __end_t.tv_sec = 0;
	 }
    
	 inline void start()
	 {
	  __elapsed = 0;
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
	  clock_serv_t cclock;
	  mach_timespec_t mts;
	  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	  clock_get_time(cclock, &mts);
	  mach_port_deallocate(mach_task_self(), cclock);
	  __start_t.tv_sec  = mts.tv_sec;
	  __start_t.tv_nsec = mts.tv_nsec;
#else
	  clock_gettime(CLOCK_REALTIME, &__start_t);
#endif
	 }

	 inline void stop()
	 {
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
	  clock_serv_t cclock;
	  mach_timespec_t mts;
	  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	  clock_get_time(cclock, &mts);
	  mach_port_deallocate(mach_task_self(), cclock);
	  __end_t.tv_sec  = mts.tv_sec;
	  __end_t.tv_nsec = mts.tv_nsec;
#else
	  clock_gettime(CLOCK_REALTIME, &__end_t);
#endif
	  __elapsed =   difftime( __end_t.tv_sec,   __start_t.tv_sec)  
				+ (double)((__end_t.tv_nsec - __start_t.tv_nsec)*1e-9);
	 }
	
	 inline double 
	 elapsed() { return __elapsed; };
	 
	 inline 
	 double current() // current tine in seconds
	 { 
	    struct timespec t;
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
	  clock_serv_t cclock;
	  mach_timespec_t mts;
	  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	  clock_get_time(cclock, &mts);
	  mach_port_deallocate(mach_task_self(), cclock);
	  t.tv_sec  = mts.tv_sec;
	  t.tv_nsec = mts.tv_nsec;
#else
	    clock_gettime(CLOCK_REALTIME, &t);
#endif
	    return  ( t.tv_sec + (double)((t.tv_nsec)*1e-9)); 
	 }
	 
    private:

	 struct timespec __start_t;
	 struct timespec __end_t;
	 double          __elapsed;
   };

   // #include "timer.cc"
}

#endif // __XPU_TIMER_H__

   /*
	 CLOCK_REALTIME   	
	 CLOCK_MONOTONIC  
	 CLOCK_PROCESS_CPUTIME_ID
	 CLOCK_THREAD_CPUTIME_ID
   */

   /*
   int main(int argc, char **argv)
   {
    #define size 1024*20
    long int v[size];
    timer t;
    t.start();
    //usleep(1000);
    for (int i=0;i<size;++i) v[i] = i;
    t.stop();
    cout << "time: " <<  t.elapsed() << endl;
   };
   */
