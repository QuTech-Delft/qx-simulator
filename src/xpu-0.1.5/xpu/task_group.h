/**
 * @file		task_group.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     abstract task group interface
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

#ifndef __XPU_TASK_GROUP_H__
#define __XPU_TASK_GROUP_H__

#include <xpu/range.h>
#include <xpu/pointers.h>
#include <xpu/lockable.h>

namespace xpu
{
   /**
    * task_group : common interface
    */

   class task_group
   {
	 public:

	    virtual void run()  = 0;
	    virtual void run(int i)  = 0;
	    virtual void run(range& r)  = 0;

	    virtual pointers get_in_data() = 0;
	    virtual pointers get_out_data() = 0;

	    virtual void set_shared(pointer,lockable*) = 0;
	    virtual void detect_shared() = 0;
   };
}

#endif // __TASK_GROUP_H__

