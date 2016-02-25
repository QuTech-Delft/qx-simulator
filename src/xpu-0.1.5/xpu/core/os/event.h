/**
 * @file event.h
 * @version 0.1
 * @author Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @description event encapsulation (lossless fifo-based event implementation)
 *              
 * @date 11/02/2010
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

#ifndef __XPU_EVENT_H__
#define __XPU_EVENT_H__

//#include <queue.h>
#include <xpu/stdout.h>
#include <xpu/core/os/control.h>

//typedef int work;


namespace xpu
{
   namespace core
   {
	 namespace os
	 {
	    class event
	    {
		  public:

		    event()
		    {
			  m_control = new data_control;
			  m_queue   = new int;
			  *m_queue   = 0;
		    }

		    ~event()
		    {
			  delete m_control;
			  delete m_queue;
		    }

		    inline void reset()
		    {
			  m_control->reset();
		    }
		    
		    inline void fire( )
		    {
			  m_control->lock();
			  (*m_queue)++;
			  m_control->unlock();
			  m_control->broadcast();
		    }
		    

		    inline int notifications()
		    {
			  return *m_queue;
		    }

		    inline void wait()
		    {
			  m_control->lock();
			  if (*m_queue == 0) 
			  {
				   m_control->wait();
			  }
			  // there is work
			  (*m_queue)--;
			  m_control->signal();
			  m_control->unlock();
		    }


		    inline void multi_wait()
		    {
			  m_control->lock();
			  if (*m_queue == 0) 
			  {
				   m_control->wait();
			  }
			  // (*m_queue)--;
			  m_control->signal();
			  m_control->unlock();
		    }



		  private:
		    
		    data_control  * m_control;
		    int           * m_queue;
	    };
	 } // namespace os
   } // namespace core
} // namespace xpu

//#include <xpu/core/os/event.cc>

#endif // __XPU_WORK_QUEUE_H__

