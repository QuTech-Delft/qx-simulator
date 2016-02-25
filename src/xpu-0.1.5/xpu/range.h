/**
 * @file		range.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     data partionning algorithms
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

#ifndef __XPU_RANGE_H__
#define __XPU_RANGE_H__

#include <vector>

#include <xpu/stdout.h>
#include <xpu/utils.h>
#include <xpu/lockable.h>

#ifndef __xpu_use_hard_lock__
#ifdef  __xpu_use_spinlock__
  #include <xpu/core/os/spinlock.h>
#else
  #include <xpu/core/os/mutex.h>
#endif
#endif

using namespace xpu::core::os;

namespace xpu
{
   namespace core 
   {
	 extern unsigned int           workers_count ;
   }

   class range;
   class shared_range; 

   typedef std::vector<range> ranges;
   typedef std::vector<shared_range *> shared_ranges;


  
   class shared_range 
   {
      protected:
#ifndef __xpu_use_hard_lock__
	 lockable * m;
#endif
	 int min;
	 int max;
	 int index; 
	 int step;
	 int subrange_length;

	 private:

	   __xpu_disallow_copy_and_assign__(shared_range);

      public:

	 shared_range(int min, int max, int step): min(min),
						   max(max),
						   index(min),
						   step(step)
         {
#ifndef __xpu_use_hard_lock__
#ifdef  __xpu_use_spinlock__
	    m = new spinlock(); 
#else // use mutex
	    m = new mutex(); 
#endif
#endif
	    if ((max-min) >= core::workers_count) 
		  subrange_length = (max-min)/core::workers_count;
	    else 
		  subrange_length = step;
         }

	 inline int begin() { return min; }
	 inline int end()   { return max; }
	 inline int init()  
	 {  
	    if ((max-min) > core::workers_count) 
		  subrange_length = (max-min)/core::workers_count;
	    else 
		  subrange_length = step;
	    return (index=min); 
	 }
	 inline int get_step()
	 {
	    return step;
	 }

         shared_range & operator = (shared_range & sr)
	 {
	    index = sr.index;
	    min   = sr.min;
	    max   = sr.max;
	    step  = sr.step;

	    if ((max-min) > core::workers_count) 
		  subrange_length = (max-min)/core::workers_count;
	    else 
		  subrange_length = step;
	    return *this;
	 }

	 inline int steal() 
	 {
#ifdef __xpu_use_hard_lock__
	    int i = __sync_fetch_and_add(&index, step);
#else
	    m->lock();
	    int i = index;
	    index += step;
	    m->unlock();
#endif
	    return i; 
	 }

      inline int steal_subrange()
	 {
#ifdef __xpu_use_hard_lock__
	    int i = __sync_fetch_and_add(&index, subrange_length);
#else
	    m->lock();
	    int i = index;
	    index += subrange_length;
	    m->unlock();
#endif
	    return i; 
	 }

	 int get_subrange_length()
	 {
	    return subrange_length;
	 }

	 void debug()
	 {
	    __debug("[-] shared range  " << this);
	    __debug(" |- min      " << min);
	    __debug(" |- max      " << max);
	    __debug(" |- step     " << step);
	    __debug(" |- index    " << index);
	    __debug(" |- subrange " << subrange_length);
	 }



	 friend class range;
   };

   class range
   {
      protected:

	 int min;
	 int max;
	 int index; 
	 int step;

      public:

	 range( ):  min(0),
			  max(0),
			  index(0),
			  step(0)
         {
         }
	 
	 range(int min, int max, int step):  min(min),
					     max(max),
					     index(min),
					     step(step)
         {
         }
	 
	 range(const range& r)
	 {
	    min   = r.min;
	    max   = r.max;
	    index = r.index;
	    step  = r.step;
	 }

	 inline int begin() 
	 { 
	    return min; 
	 }

	 inline int end()   
	 { 
	    return max; 
	 }

	 inline int next() 
	 { 
	    int i =  index;
	    index+=step; 
	    return i;
	 }

	 inline int init() 
	 { 
	    return (index=min);   
	 }

	 inline int get_step()
	 {
	    return step;
	 }


      /**
	  * quasi-equitative distribution (decreasing order)
	  */
	 inline ranges split_dec(int m)
	 {
	    ranges res;
	    int n = max - min; // size
	    int b = min;
	    int q,r,a;
	    for (int k=0; k<m; k++)
	    {
	       q = (n-k)/m;
	       r = (n-k)%m;
	       a = b;
	       b += q + (r!=0 ? 1 : 0);
	       if (k==0) 
			if (step==1) 
			   res.push_back(range(a,b,step));
			else
			   res.push_back(range(a,b+1,step));
	       else    
	       {
		  if (step != 1) 
		     res.push_back(range(a+step-(a%step),b+(k==(m-1) ? 0 : 1),step));
		  else    
		     res.push_back(range(a+step-(a%step)-1,b,step));
	       }
	    }
	    return res;
	 }

	 inline ranges split_inc(int m)
	 {
	    std::vector<range> res;
	    int n = max - min; // size
	    int b = min;
	    int a = b;
	    //int r;
	    for (int k=0; k<m; k++)
	    {
	       a = b;
	       //r = (n+k)%m;
	       b += (n+k)/m;
	       if (k==0) 
			if (step==1) 
			   res.push_back(range(a,b,step));
			else
			   res.push_back(range(a,b+1,step));
	       else     
	       {
		  if (step != 1)
		     res.push_back(range(a+step-(a%step),b+(k==(m-1) ? 0 : 1),step));
	       else     
		     res.push_back(range(a+step-(a%step)-1,b,step));
	       }
	    }
	    return res;
	 }

	 inline ranges split_dec(int m, shared_range * sr)
	 {
	    ranges res = split_dec(m+1);
	    sr->min   = res.back().min;
	    sr->max   = res.back().max;
	    sr->index = res.back().index;
	    sr->step  = res.back().step;
	    res.pop_back();
	    sr->init();
	    return res;
	 }


	 inline ranges split_inc(int m, shared_range * sr)
	 {
	    ranges res = split_dec(m+1);
	    sr->min   = res.back().min;
	    sr->max   = res.back().max;
	    sr->index = res.back().index;
	    sr->step  = res.back().step;
	    res.pop_back();
	    sr->init();
	    return res;
	 }


	 inline void split_dec(int mp, int ms, ranges& p_res, shared_ranges& s_res)
	 {
	    //ranges        p_res;
	    //shared_ranges s_res;
	    p_res.clear();
	    s_res.clear();

	    int m = mp+ms;
	    int n = max - min; // size
	    int b = min;
	    int q,r,a;
	    
	    for (int k=0; k<m; k++)
	    {
	       q = (n-k)/m;
	       r = (n-k)%m;
	       a = b;
	       b += q + (r!=0 ? 1 : 0);
	       //std::cout << a << ".." << b << std::endl;
	       if (k==0) 
			p_res.push_back(range(a,b,step));
	       else    
	       {
		  if (step != 1) 
		     p_res.push_back(range(a+step-(a%step),b,step));
		  else    
		     p_res.push_back(range(a+step-(a%step)-1,b,step));
	       }
	    }

	    for (int i=mp; i<m; i++)
	    {
		  shared_range * sr = new shared_range(0,0,0);
		  sr->min   = p_res.back().min;
		  sr->max   = p_res.back().max;
		  sr->index = sr->min; //p_res.back().index;
		  sr->step  = p_res.back().step;
		  p_res.pop_back();
		  sr->init();
		  s_res.push_back(sr);
	    }
	 }

	 void debug()
	 {
	    printf("[+] range (min=%d, max=%d, step=%d)\n",min,max,step);
	 }

   };


}

#endif // __XPU_RANGE_H__

