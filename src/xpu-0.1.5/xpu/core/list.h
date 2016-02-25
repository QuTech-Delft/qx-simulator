/**
 * @file    list.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @brief   light list implementation
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


#ifndef __XPU_LIST_H__
#define __XPU_LIST_H__

#include <xpu/stdout.h>
#include <xpu/exception.h>

namespace xpu
{
   namespace core
   {
	 template <typename T>
	 struct list_node
	 {
	    list_node *  next;
	    T            data;
	 };

	 template<typename T>
	 class list
	 {
	    public:
		 
		 list()
		 {
		    m_head = m_tail = NULL;
		    m_size  = 0;
		 }

		 ~list()
		 {
		    clear();
		 }

		 void add(T item)
		 {
		    struct list_node<T> * node = new struct list_node<T>;
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

		 /**
		  * delete element e
		  */
		 void remove(unsigned int e) throw (exception)
		 {
		    if (e < m_size)
		    {
			  struct list_node<T> * node, * prev;
			  if (!e)
			  {
				node = m_head->next;
				delete m_head;
				m_head = node;
				m_size--;
				return;
			  }
			  node = m_head;
			  while (e--)
			  {
				prev = node;
				node = prev->next;
			  }
			  prev->next = node->next;
			  delete node;
			  m_size--;
		    }
		    else
			  throw exception(" xpu::list : cannot perform remove() index out of range.");
		 }

		 /**
		  * return element e
		  */
		 T get(unsigned int e) throw (exception)
		 {
		    if ((int)e < m_size)
		    {
			  struct list_node<T> * node; 
			  struct list_node<T> * prev;
			  node = m_head;
			  while (e)
			  {
				prev = node;
				node = prev->next;
				e--;
			  };
			  return node->data;
		    }
		    else
			  throw exception(" xpu::list : cannot perform remove() out of range index.");
		 }

		 /**
		  * return element e
		  */
		 T operator [](unsigned int e) throw(exception)
		 {
		    return get(e);
		 }

		 /**
		  * last
		  */
		 T last()
		 {
		    return m_tail->data;
		 }

		 T first()
		 {
		    return m_head->data;
		 }

		 /**
		  * clear list
		  */
		 void clear()
		 {
		    if (m_size)
		    {
			  struct list_node<T> * node = m_head;
			  while (node)
			  {
				struct list_node<T> * tmp = node;
				node = node->next;
				delete tmp;
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
		    __debug("list " << (void *)this);
		    __debug("  |- size: " << m_size);
		    if (m_size)
		    {
			 __debug("  |- items: " );
			 struct list_node<T> * node = m_head;
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

		 struct list_node<T> * m_head;
		 struct list_node<T> * m_tail;
		 int                    m_size;

	 };
   } // core
} // xpu


#endif // __XPU_LIST_H__

