/**
 * @file    barrier.cc
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @brief   barrier 
 * @date    11/02/2010
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



/**
 * constructor
 */ 
xpu::core::os::barrier::barrier(u_int32_t count) : n_count(count) 
{ 
 n_barrier_id = new pthread_barrier_t;
 pthread_barrierattr_init(&n_attr);
 pthread_barrier_init(n_barrier_id, &n_attr, n_count);
}


/**
 * destructor
 */ 
xpu::core::os::barrier::~barrier() 
{
  delete n_barrier_id;
}


pthread_barrier_t * 
xpu::core::os::barrier::id() 
{ 
  return n_barrier_id; 
}

u_int32_t 
xpu::core::os::barrier::get_count() 
{ 
  return n_count; 
}

void 
xpu::core::os::barrier::set_count(u_int32_t count) 
{ 
 n_count = count;
 pthread_barrierattr_init(&n_attr);
 pthread_barrier_init(n_barrier_id, &n_attr, n_count);
}


void 
xpu::core::os::barrier::init()
{
 pthread_barrierattr_init(&n_attr);
 pthread_barrier_init(n_barrier_id, &n_attr, n_count);
}


