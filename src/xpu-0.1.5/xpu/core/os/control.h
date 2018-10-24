/**
 * @file control.h
 * @version 0.1
 * @author Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
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

#ifndef __XPU_DATA_CONTROL_H__
#define __XPU_DATA_CONTROL_H__

#include <pthread.h>

namespace xpu
{
   namespace core
   {
     namespace os
     {
        class data_control
        {
          public:
             
            /**
             * ctor
             */
            data_control()
            {
               pthread_mutex_init(&m_mutex,NULL);
               pthread_cond_init(&m_cond,NULL);
               m_active = 1;
            }

            /**
             * dtor
             */
            ~data_control()
            {
               pthread_mutex_destroy(&m_mutex);
               pthread_cond_destroy(&m_cond);
               m_active = 0;
            }

            /**
             * reset data_control
             */
            inline void reset()
            {
               pthread_mutex_init(&m_mutex,NULL);
               pthread_cond_init(&m_cond,NULL);
               m_active = 1;
            }

            /**
             * activate
             */
            inline void activate()
            {
               pthread_mutex_lock(&m_mutex);
               m_active = 1;
               pthread_mutex_unlock(&m_mutex);
               pthread_cond_broadcast(&m_cond);
            }

            /**
             * deactivate
             */
            inline void deactivate()
            {
                if ( pthread_equal(pthread_self(), m_owner) )
                    throw " thread cannot lock same mutex twice !";
                pthread_mutex_lock (&m_mutex);
                m_active = 0;
                if ( !pthread_equal(pthread_self(), m_owner) )
                    throw "only thread witch locked the mutex can release it !";
                pthread_mutex_unlock(&m_mutex);
                pthread_cond_broadcast(&m_cond);
            }

            inline int active()
            {
               return m_active;
            }

            inline void lock()
            {
               pthread_mutex_lock(&m_mutex);
            }

            inline void unlock()
            {
               pthread_mutex_unlock(&m_mutex);
            }

            inline void wait()
            {
               pthread_cond_wait(&m_cond, &m_mutex);
            }

            inline void signal()
            {
               pthread_cond_signal(&m_cond);
            }

            inline void broadcast()
            {
               pthread_cond_broadcast(&m_cond);
            }

          private:

            pthread_mutex_t m_mutex;
            pthread_cond_t  m_cond;
            int             m_active;
            pthread_t       m_owner = pthread_self();
        };
     } // os
   } // core
} // xpu

#endif // __XPU_DATA_CONTROL_H__

