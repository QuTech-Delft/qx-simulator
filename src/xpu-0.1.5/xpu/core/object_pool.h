/**
 * @file    object_pool.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @brief   object pool for memory allocation
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

#ifndef __XPU_OBJECT_POOL__
#define __XPU_OBJECT_POOL__

#include <iostream>
#include <cstdlib>

#include <xpu/core/list.h>

namespace xpu
{
   namespace core
   {
	 template<class T, int S> 
	 class object_pool
	 {
		private:

		__xpu_disallow_copy_and_assign__(object_pool);

		protected:

		    xpu::core::list<T*>  m_object_data_list;
		    
		    T**            m_object_free;
		    int            m_object_count;
		    int            m_top;
		    //int S;

		protected:

		    /**
			* free all
			*/
		    void free(void)
		    {
			   int i = (S-1);
			   int pool_size = m_object_data_list.size();
			   for (int j=0; j<pool_size; j++)
			   {
				 T* m_object_data = m_object_data_list.get(j);
				 for(m_top=0;m_top<S*j;m_top++)
				 {
				    m_object_free[m_top] = &m_object_data[i--];
				 }
			   }
			   return;
		    }

		public:
		    
		    void free(T* instance)
		    {
			   //if((m_top<m_object_count) && (instance>=&m_object_data[0]) && (instance<=&m_object_data[m_object_count-1]))
			   if((m_top<m_object_count))
			   {
				  m_object_free[m_top++] = instance;
			   }
			   return;
		    }

		    T* allocate(void)
		    {
			   if (m_top>0)
				  return (m_object_free[--m_top]);
			   
			   // there is no more memory, reallocate memory
			   m_object_count += S;
			   
			   ::free(m_object_free);
			   m_object_free = (T**)malloc(m_object_count*sizeof(T*));
			   
			   T * m_object_data = (T*)malloc(S*sizeof(T));
			   m_object_data_list.add(m_object_data);

			   // setup free data
			   int i = (S-1);
			   for (m_top=0; m_top<S-1; m_top++)
				  m_object_free[m_top] = &m_object_data[i--];

			   // allocate
			   return (m_object_free[--m_top]);
		    }

		    object_pool()
		    {
			   m_object_data_list.add((T*)malloc(S*sizeof(T)));
			   m_object_free = (T**)malloc(S*sizeof(T));
			   
			   m_object_count = S;

			   free();
		    }

		    virtual ~object_pool(void)
		    {
			  //debug();
			  int size = m_object_data_list.size();
			  for (int i=0; i<size; i++)
				 ::free(m_object_data_list[i]);
			   ::free(m_object_free);
		    }

		    void debug()
		    {
			  m_object_data_list.debug();
			  std::cout << " m_object_count " << m_object_count << std::endl;
			  std::cout << " m_object_free "  << m_object_free  << std::endl;
		    }
	 };
   } // core
} // xpu

#endif // __XPU_OBJECT_POOL__
