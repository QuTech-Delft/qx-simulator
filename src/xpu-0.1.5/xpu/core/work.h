/**
 * @file    work.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @brief   work request
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

#ifndef __XPU_WORK_H__
#define __XPU_WORK_H__

#include <xpu/task.h>
#include <xpu/range.h>

#include <xpu/core/os/event.h>

namespace xpu
{
   namespace core
   {
	 /**
	  * work interface
	  */
	 class work
	 {
	    public:
		  virtual void perform() = 0;  // perform requested work then fire an event
		  virtual void wait()  = 0;    // wait for work to be done
	 };


	 /**
	  * basic work
	  */
	 class basic_work : public work
	 {
	    public:

		  basic_work( ) : m_task(NULL), m_event(NULL)
	    {
	    }

		  basic_work(task * n_task) : m_task(n_task)
	    {
		  m_event = new event();
	    }

		  ~basic_work()
		  {
			delete m_event;
		  }

		  basic_work(const basic_work & wrq)
		  {
			m_task  = wrq.m_task;
			m_event = wrq.m_event;
		  }

		  void init(task * n_task)
		  {
			m_task  = n_task;
		  }

		  void perform()
		  {
			m_task->run();
			m_event->fire();
		  }

		  void wait()
		  {
			m_event->wait();
		  }

	    private:

		  task  * m_task;
		  event * m_event;

	 };


	 /**
	  * for work
	  */
	 class for_work : public work
	 {
	    public:

		  for_work( ) : m_task(NULL), m_event(NULL)
	    {
	    }

		  for_work(task * n_task, range& r) : m_task(n_task), m_range(r)
	    {
		  m_event = new event();
	    }

		  ~for_work()
		  {
			delete m_event;
		  }

		  for_work(const for_work & wrq)
		  {
			m_task  = wrq.m_task;
			m_event = wrq.m_event;
			m_range = wrq.m_range;
		  }

		  void init(task * n_task, range& r)
		  {
			m_task  = n_task;
			m_range = r;
		  }

		  void perform()
		  {
			m_task->run(m_range);
			m_event->fire();
		  }

		  void wait()
		  {
			m_event->wait();
		  }

	    private:

		  task  * m_task;
		  event * m_event;
		  range   m_range;

	 };




	 /**
	  * for_cowork
	  *   
	  *   cooperative parallel for work
	  */
	 class for_cowork : public work
	 {
	    public:

		  for_cowork( ) : m_task(NULL), m_range(0,0,0), m_shared_range(NULL), m_event(NULL)
	    {
	    }

		  for_cowork(task * m_task, range m_range, shared_range * m_shared_range) : m_task(m_task), m_range(m_range), m_shared_range(m_shared_range)
	    {
		  m_event = new xpu::core::os::event();
	    }

		  ~for_cowork()
		  {
			delete m_event;
		  }

		  for_cowork(const for_cowork & wrq)
		  {
			m_task         = wrq.m_task;
			m_range        = wrq.m_range;
			m_shared_range = wrq.m_shared_range;
			m_event        = wrq.m_event;
		  }

		  void init(task * n_task, range n_range, shared_range * n_shared_range)
		  {
			m_task         = n_task;
			m_range        = n_range;
			m_shared_range = n_shared_range;
		  }

		  void perform()
		  {
			m_task->run(m_range, m_shared_range);
			m_event->fire();
		  }

		  void wait()
		  {
			m_event->wait();
		  }

		  void debug()
		  {
			__debug("[-] for_work: " << this);
			__debug("     |- task  : " << m_task);
			__debug("     |- event : " << m_event);
			__debug("     |- range : " << this);
			m_range.debug();
			__debug("     |- shared range : " << this);
			m_shared_range->debug();
		  }

	    private:

		  xpu::task  *                  m_task;
		  xpu::range                    m_range;
		  xpu::shared_range *           m_shared_range;

		  xpu::core::os::event *        m_event;

	 };


	 /**
	  * pipe work
	  */
	 class pipe_work : public work
	 {
	    public:

		  pipe_work( ) : m_task(NULL), m_event(NULL)
	    {
	    }

		  pipe_work(task * n_task, int i) : m_task(n_task), m_idx(i)
	    {
		  m_event = new event();
	    }

		  ~pipe_work()
		  {
			delete m_event;
		  }

		  pipe_work(const pipe_work & wrq)
		  {
			m_task  = wrq.m_task;
			m_event = wrq.m_event;
			m_idx   = wrq.m_idx;
		  }

		  void init(task * n_task, int i)
		  {
			m_task  = n_task;
			m_idx   = i;
		  }

		  void perform()
		  {
			m_task->run(m_idx);
			m_event->fire();
		  }

		  void wait()
		  {
			m_event->wait();
		  }

	    private:

		  task  * m_task;
		  event * m_event;
		  int     m_idx;

	 };


	 /**
	  * async pipe work
	  */
	 class async_pipe_work : public work
	 {
	    public:

	    async_pipe_work( ) : m_task(NULL), m_event(NULL), m_next_stage_event(NULL)
	    {
	    }

	    async_pipe_work(task * n_task, event * next_stage_event, int i) : m_task(n_task), m_next_stage_event(next_stage_event), m_idx(i)
	    {
		  //m_event = new event();
	    }

		  ~async_pipe_work()
		  {
			//delete m_event;
		  }

		  async_pipe_work(const async_pipe_work & wrq)
		  {
			m_task  = wrq.m_task;
			m_event = wrq.m_event;
			m_next_stage_event = wrq.m_next_stage_event;
			m_idx   = wrq.m_idx;
		  }

		  void init(task * n_task, event * next_stage_event, int i)
		  {
			m_next_stage_event = next_stage_event;
			m_task  = n_task;
			m_idx   = i;
		  }

		  void perform()
		  {
			m_task->run(m_idx);
			if (m_next_stage_event != NULL)
			   m_next_stage_event->fire();
			//m_event->fire();
		  }

		  void wait()
		  {
			m_event->wait();
		  }

	    private:

		  task  * m_task;
		  event * m_event;
		  event * m_next_stage_event;
		  int     m_idx;
	 };


   } // namespace core
} // namespace xpu

#endif // __XPU_WORK_H__
