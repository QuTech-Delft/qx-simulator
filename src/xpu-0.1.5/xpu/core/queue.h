/**
 * @file    queue.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @brief   memory pool-based queue implementation
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

#ifndef __XPU_QUEUE_H__
#define __XPU_QUEUE_H__

#include <xpu/stdout.h>
#include <xpu/exception.h>

#include <xpu/core/object_pool.h>

namespace xpu
{
   namespace core
   {
	 template <typename T>
	 struct queue_node
	 {
	    queue_node * next;
	    T            data;
	 };

	 template<typename T>
	 class queue
	 {
	    public:
		 
		 queue()
		 {
		    m_head = m_tail = NULL;
		    m_size  = 0;
		 }

		 ~queue()
		 {
		    //clear();
		 }

		 void push(T item)
		 {
		    //struct queue_node<T> * node = new struct queue_node<T>;
		    struct queue_node<T> * node = m_allocator.allocate();
		    node->data = item;
		    node->next = NULL;
		    if (m_size)
		    {
			  m_tail->next = node;
			  m_tail       = node;
		    }
		    else
		    {
			  m_head = m_tail = node;
		    }
		    m_size++;
		 }

		 T pop() throw (exception)
		 {
		    if (m_size)
		    {
			  T data  = m_head->data;
			  struct queue_node<T> * node =  m_head;
			  m_head = m_head->next;
			  //delete node;
			  m_allocator.free(node);
			  m_size--;
			  return data;
		    }
		    else
		    throw exception(" xpu::queue : cannot perform pop() operation on empty queue.");
		 }


		 T top()
		 {
		    return m_head->data;
		 }

		 void clear()
		 {
		    if (m_size)
		    {
			  struct queue_node<T> * node = m_head;
			  while (node)
			  {
				struct queue_node<T> * tmp = node;
				node = node->next;
				m_allocator.free(tmp);
				//delete tmp;
			  }
			  m_size = 0;
		    }
		 }

		 int size()
		 {
		    return m_size;
		 }

		 bool empty()
		 {
		    return (m_size==0);
		 }

		 void debug()
		 {
		    __debug("queue " << (void *)this);
		    __debug("  |- size: " << m_size);
		    if (m_size)
		    {
			 __debug("  |- items: " );
			 struct queue_node<T> * node = m_head;
			 __print("      [");
			 while (node)
			 {
			   __print(node->data << ", ");
			   node = node->next;
			 }
			 __println("]");
		    }
		 }

	    private:

		 struct queue_node<T> * m_head;
		 struct queue_node<T> * m_tail;
		 int                    m_size;

		 object_pool<queue_node<T>,1024> m_allocator;

	 };
   } // core
} // xpu
 
#endif // __XPU_QUEUE_H__

