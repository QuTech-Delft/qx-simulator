/**
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
  
   lockable_factory * lockable_factory::m_instance = NULL;

   lockable_factory * lockable_factory::instance() 
   {
     if (m_instance) 
	   return m_instance; 
     else 
	   return (m_instance=new lockable_factory()); 
   }
  

   lockable_factory::~lockable_factory() 
   { 
      m_instance = NULL; 
      // destroy mutexes ...
      for (std::map<pointer, lockable*>::iterator it = m_shared.begin(); it!= m_shared.end(); it++)
      {
        delete (*it).second;
      }
   }
 
   

   lockable * 
   lockable_factory::getlockable(pointer p)   
   {
     std::map<pointer, lockable*>::iterator it;
     if ((it = m_shared.find(p)) != m_shared.end())
     {
       return (*it).second;
     }
     else 
     {
       #ifdef __XPU_USE_SPINLOCK__
         lockable * l = new spinlock();
       #else
         lockable * l = new xpu::core::os::mutex();
       #endif
       m_shared[p] = l;

       return l;
     } 
   } 

