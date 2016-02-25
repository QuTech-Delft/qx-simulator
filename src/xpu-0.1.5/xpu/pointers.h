/**
 * @file    pointers.h 
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @date    14/06/2010
 * @brief   implentation of operations on pointers sets, 
 *          it's mainly used on the shared data detection 
 *          process.
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


#ifndef __XPU_POINTERS_79424B4E0A_H__
#define __XPU_POINTERS_79424B4E0A_H__



#include <algorithm>
#include <set>

#include <xpu/stdout.h>

namespace xpu
{
   typedef void * pointer;

   class pointers;

   class shareable
   {
	 public:
	    shareable()
	    { }

	    virtual ~shareable()
	    { }

	    virtual pointers get_pointers() = 0;
   };


   
   class pointers : public std::set<pointer>
   {
	 public:
	    
	    inline pointers();
	   
	    inline void add(shareable * p);
	   
	    template<typename __T> 
	    inline void add(__T * p);

	    template<typename __T> 
	    inline void add(const __T * p);

	    template<typename __T> 
	    inline void add(__T p);
     
	    inline void remove(shareable * p);

	    template<typename __T>
	    inline void remove(__T * p);

	    template<typename __T>
	    inline void remove(const __T * p);

	    template<typename __T>
	    inline void remove(__T p);
	    
	    template<typename __T>
	    inline bool contains(__T * p);
	   
	    template<typename __T>
	    inline bool contains(const __T * p);
	    
	    template<typename __T>
	    inline bool contains(__T p);

	    // operation between pointer sets

	    inline pointers & operator -= (const pointers & ptrs);
	    inline pointers & operator += (const pointers & ptrs);
	    inline pointers & operator *= (const pointers & ptrs);

	    inline pointers   operator + (const pointers & ptrs);
	    inline pointers   operator - (const pointers & ptrs);
	    inline pointers   operator * (const pointers & ptrs);

	    // fast pointer insert/erase

	    template<typename __T>
	    inline pointers & operator -= (__T * ptr);

	    template<typename __T>
	    inline pointers & operator += (__T * ptr);

	    template<typename __T>
	    inline pointers   operator + (__T * ptr);

	    template<typename __T>
	    inline pointers   operator - (__T * ptr);

	    // debug
	    /*
	    inline void dump()
	    {
		  __print("[ ");
		  for(pointers::iterator i = begin(); i!= end(); i++)
			__print(*i<< ", ");
		  __println("]");
	    }
	    */

	 };

	 // ----- implementation --------

	 pointers:: pointers()
	 {
	 }


	 void
	 pointers:: add(shareable * p)
	 {
	    *this += p->get_pointers();
	    insert((pointer)p);
	 }

	 template<typename __T> 
	 void
	 pointers:: add(__T * p)
	 {
	   insert((pointer)p);
	 }

	 template<typename __T> 
	 void
	 pointers:: add(const __T * p)
	 {
	    return;
	 }

	 template<typename __T> 
	 void
	 pointers:: add(__T p)
	 {
	   return; 
	 }


	 void
	 pointers:: remove(shareable * p)
	 {
	    *this -= p->get_pointers();
	    erase((pointer)p);
	 }

	 template<typename __T>
	 void 
	 pointers:: remove(__T * p)
	 { 
	    erase((pointer)p); 
	 }

	 template<typename __T>
	 void
	 pointers:: remove(const __T * p)
	 { 
	    return; 
	 }

	 template<typename __T>
	 void
	 pointers:: remove(__T p)
	 { 
	    return; 
	 }

	 template<typename __T>
	 bool
	 pointers:: contains(__T * p)
	 {
	    return (find((pointer)p)!=end());
	 }

	 template<typename __T>
	 bool
	 pointers:: contains(const __T * p)
	 {
	    return false;
	 }

	 template<typename __T>
	 bool
	 pointers:: contains(__T p)
	 {
	    return false;
	 }

	 // operation between pointer sets

	 pointers & 
	 pointers:: operator -= (const pointers & ptrs)
	 {
	    //erase(ptrs.begin(), ptrs.end());
	    pointers res;
	    set_difference(begin(), end(), 
					 ptrs.begin(), ptrs.end(), 
					 std::inserter(res, res.begin()) );
	    *this = res;
	    return *this;
	 }

	 pointers & 
	 pointers:: operator += (const pointers & ptrs)
	 {
	    insert(ptrs.begin(), ptrs.end());
	    return *this;
	 }

	 pointers & 
	 pointers:: operator *= (const pointers & ptrs)
	 {
	    pointers res;
	    set_intersection(begin(), end(), 
					 ptrs.begin(), ptrs.end(), 
					 std::inserter(res, res.begin()) );
	    *this = res;
	    return *this;
	 }

	 pointers 
	 pointers:: operator + (const pointers & ptrs)
	 {
	    pointers res = pointers(*this);
	    return ( res += ptrs);
	 }

	 pointers 
	 pointers:: operator - (const pointers & ptrs)
	 {
	    pointers res = pointers(*this);
	    return ( res -= ptrs);
	 }

	 pointers 
	 pointers:: operator * (const pointers & ptrs)
	 {
	    pointers res = pointers(*this);
	    return ( res *= ptrs);
	 }

	 // fast pointer insert/erase

	 template<typename __T>
	 pointers & 
	 pointers:: operator -= (__T * ptr)
	 {
	    remove(ptr);
	    return *this;
	 }

	 template<typename __T>
	 pointers & 
	 pointers:: operator += (__T * ptr)
	 {
	    add(ptr);
	    return *this;
	 }

	 template<typename __T>
	 pointers 
	 pointers:: operator + (__T * ptr)
	 {
	    pointers res = pointers(*this);
	    return ( res += ptr);
	 }

	 template<typename __T>
	 pointers 
	 pointers:: operator - (__T * ptr)
	 {
	    pointers res = pointers(*this);
	    return ( res -= ptr);
	 }

} // namespace 

#endif // __XPU_POINTERS_79424B4E0A_H__

