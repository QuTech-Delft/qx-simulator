/**
 * @file		utils.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     utils 
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
 */

#ifndef __XPU_UTILS_79424B4E0A_H__
#define __XPU_UTILS_79424B4E0A_H__


#define __xpu_force_inline__ __attribute__((always_inline))
#define __xpu_disallow_copy_and_assign__(type) \
              type(const type&);             \
		    void operator=(const type&)

#include <xpu/pointers.h>

namespace xpu
{

   /**
    * __read_only
    */
   template<typename __T>
   static inline const __T * __read_only(__T * d)        __xpu_force_inline__ ; 

   template<typename __T>
   static inline const __T * __read_only(const __T * d)  __xpu_force_inline__ ;

   template<typename __T>
   static inline       __T   __read_only(__T d)          __xpu_force_inline__ ; 



   template<typename __T>
   static inline const __T * __read_only(__T * d)        { }

   template<typename __T>
   static inline const __T * __read_only(const __T * d)  { }

   template<typename __T>
   static inline       __T   __read_only(__T d)          { }



   /**
    * __safe_type
    */
   template<typename __T1> 
   static inline __T1 __safe_type(__T1 a, __T1 b)               __xpu_force_inline__ ;
   
   template<typename __T1, typename __T2> 
   static inline __T1 __safe_type(__T1 a, __T2 b)               __xpu_force_inline__ ;
   
   template<typename __T1> 
   static inline __T1 __safe_type(__T1 a, __T1 b)
   {
	 return b;
   }

   template<typename __T1, typename __T2> 
   static inline __T1 __safe_type(__T1 a, __T2 b)
   {
	 return a;
   }

   /**
    * __to_pointer()
    */
   template <typename __T>
   void * __to_pointer(__T val)
   {
	    __T * result = new __T;  // must be cleaned : use central memory manager
	       *result = val;
	    return (void*)result;
   }


   template<typename T>
	 void add_ptr(pointers& in_ptrs, pointers& out_ptrs, T * p)
	 {
	    out_ptrs.insert((pointer)p);
	 }

   template<typename T>
	 void add_ptr(pointers& in_ptrs, pointers& out_ptrs, const T * p)
	 {
	    in_ptrs.insert((pointer)p);
	 }  

   template<typename T>
	 void add_ptr(pointers& in_ptrs, pointers& out_ptrs, T p)
	 {
	 }  
   
   template<typename T>
      T __safe_cast(T x, T y)
	 {
	    return y;
	 }

   template<typename T1, typename T2>
      T1 __safe_cast(T1 x, T2 y)
	 {
	    return x;
	 }

} // namespace xpu

#endif // __XPU_UTILS_79424B4E0A_H__
