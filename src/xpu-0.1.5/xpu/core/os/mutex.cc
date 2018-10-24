/**
 * @file mutex.cc
 * @version 0.1
 * @author Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @description mutex encapsulation
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


xpu::core::os::mutex::mutex(void):m_owner(0)
{
   pthread_mutexattr_init	(&m_attr);
   pthread_mutex_init	     (&m_mutex, &m_attr);
}


xpu::core::os::mutex::~mutex(void)
{
   pthread_mutex_lock	     (&m_mutex);
   pthread_mutex_unlock	(&m_mutex);
   pthread_mutex_destroy	(&m_mutex);
}



void xpu::core::os::mutex::lock() 
{
   pthread_t id =  pthread_self();
   try {
	 if ( pthread_equal(id, m_owner) )
	    throw " thread cannot lock same mutex twice !";
	 pthread_mutex_lock (&m_mutex);
	 m_owner = id;
   }
   catch (char const * e)
   {
	  cerr << " (x) " <<__FILE__ << ":" << __LINE__ << ":" << __func__ << ": fatal exception : " << e << endl;
	  throw e;
   } 
}


void xpu::core::os::mutex::unlock()
{
   pthread_t id =  pthread_self();
   try {
	 if ( !pthread_equal(id, m_owner) )
	    throw "only thread witch locked the mutex can release it !";
	 m_owner = 0;
	 pthread_mutex_unlock (&m_mutex);
   }
   catch (char const * e)
   {
	  cerr << " (x) " << __FILE__ << ":" << __LINE__ << ":" << __func__ << ": fatal exception : " << e << endl;
     throw e;
   } 
}


