/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		17-03-13
 * @brief		
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

#ifndef __XPU_TASK__
#define __XPU_TASK__

#include <xpu/task_group.h>
#include <xpu/static_callback.h>
#include <xpu/dynamic_callback.h>


#ifndef println
#define println(x) std::cout << x << std::endl
#endif



namespace xpu
{
      class task : public task_group
	 {
	    private:

		  task_group * m_callable;

	    public:

		    
		/*
		 * dynamic callback
		 */

		 template < class __C, typename __R>
		 task(__C * inst, __R(__C::* m)())
		 {
		    m_callable = new dynamic_callback<__C, __R>(inst, m);
		 }

		 template < class __C, typename __R, typename __1>
		 task(__C * inst, __R(__C::* m)(__1), __1 prm)
		 {
		    m_callable = new dynamic_callback<__C, __R, __1>(inst, m, prm);
		 }

		 template < class __C, typename __R, typename __1 ,
									  typename __2 >
		 task(__C * inst, __R(__C::* m)(__1,__2),__1 prm_1,__2 prm_2 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2>(inst, m, prm_1, prm_2);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3),__1 prm_1,__2 prm_2,__3 prm_3 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3>(inst, m, prm_1, prm_2, prm_3);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4>(inst, m, prm_1, prm_2, prm_3, prm_4);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 ,
									  typename __9 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8,__9),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8,__9 prm_9 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8,__9>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8, prm_9);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 ,
									  typename __9 ,
									  typename __10 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8,__9,__10),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8,__9 prm_9,__10 prm_10 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8,__9,__10>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8, prm_9, prm_10);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 ,
									  typename __9 ,
									  typename __10 ,
									  typename __11 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8,__9 prm_9,__10 prm_10,__11 prm_11 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8, prm_9, prm_10, prm_11);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 ,
									  typename __9 ,
									  typename __10 ,
									  typename __11 ,
									  typename __12 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8,__9 prm_9,__10 prm_10,__11 prm_11,__12 prm_12 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8, prm_9, prm_10, prm_11, prm_12);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 ,
									  typename __9 ,
									  typename __10 ,
									  typename __11 ,
									  typename __12 ,
									  typename __13 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12,__13),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8,__9 prm_9,__10 prm_10,__11 prm_11,__12 prm_12,__13 prm_13 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12,__13>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8, prm_9, prm_10, prm_11, prm_12, prm_13);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 ,
									  typename __9 ,
									  typename __10 ,
									  typename __11 ,
									  typename __12 ,
									  typename __13 ,
									  typename __14 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12,__13,__14),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8,__9 prm_9,__10 prm_10,__11 prm_11,__12 prm_12,__13 prm_13,__14 prm_14 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12,__13,__14>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8, prm_9, prm_10, prm_11, prm_12, prm_13, prm_14);
		 }


		    
		 template < class __C, typename __R, typename __1 ,
									  typename __2 ,
									  typename __3 ,
									  typename __4 ,
									  typename __5 ,
									  typename __6 ,
									  typename __7 ,
									  typename __8 ,
									  typename __9 ,
									  typename __10 ,
									  typename __11 ,
									  typename __12 ,
									  typename __13 ,
									  typename __14 ,
									  typename __15 >
		 task(__C * inst, __R(__C::* m)(__1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12,__13,__14,__15),__1 prm_1,__2 prm_2,__3 prm_3,__4 prm_4,__5 prm_5,__6 prm_6,__7 prm_7,__8 prm_8,__9 prm_9,__10 prm_10,__11 prm_11,__12 prm_12,__13 prm_13,__14 prm_14,__15 prm_15 )
		 {
		    m_callable = new dynamic_callback<__C, __R, __1,__2,__3,__4,__5,__6,__7,__8,__9,__10,__11,__12,__13,__14,__15>(inst, m, prm_1, prm_2, prm_3, prm_4, prm_5, prm_6, prm_7, prm_8, prm_9, prm_10, prm_11, prm_12, prm_13, prm_14, prm_15);
		 }


		  /**
		   * static callbacks
		   */
		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9, typename __10, typename __11, typename __12, typename __13, typename __14, typename __15, typename __16>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14, __15, __16), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9, __10 m_prm_10, __11 m_prm_11, __12 m_prm_12, __13 m_prm_13, __14 m_prm_14, __15 m_prm_15, __16 m_prm_16)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14, __15, __16>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9, m_prm_10, m_prm_11, m_prm_12, m_prm_13, m_prm_14, m_prm_15, m_prm_16);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9, typename __10, typename __11, typename __12, typename __13, typename __14, typename __15>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14, __15), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9, __10 m_prm_10, __11 m_prm_11, __12 m_prm_12, __13 m_prm_13, __14 m_prm_14, __15 m_prm_15)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14, __15>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9, m_prm_10, m_prm_11, m_prm_12, m_prm_13, m_prm_14, m_prm_15);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9, typename __10, typename __11, typename __12, typename __13, typename __14>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9, __10 m_prm_10, __11 m_prm_11, __12 m_prm_12, __13 m_prm_13, __14 m_prm_14)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9, m_prm_10, m_prm_11, m_prm_12, m_prm_13, m_prm_14);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9, typename __10, typename __11, typename __12, typename __13>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9, __10 m_prm_10, __11 m_prm_11, __12 m_prm_12, __13 m_prm_13)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9, m_prm_10, m_prm_11, m_prm_12, m_prm_13);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9, typename __10, typename __11, typename __12>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9, __10 m_prm_10, __11 m_prm_11, __12 m_prm_12)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9, m_prm_10, m_prm_11, m_prm_12);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9, typename __10, typename __11>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9, __10 m_prm_10, __11 m_prm_11)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9, m_prm_10, m_prm_11);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9, typename __10>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9, __10 m_prm_10)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9, m_prm_10);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8, typename __9>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8, __9), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8, __9 m_prm_9)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8, __9>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8, m_prm_9);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7, typename __8>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7, __8), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7,  __8 m_prm_8)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7, __8>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7, m_prm_8);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6, typename __7>
			task(R(*f)(__1, __2, __3, __4, __5, __6, __7), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6, __7 m_prm_7)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6, __7>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6, m_prm_7);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5, typename __6>
			task(R(*f)(__1, __2, __3, __4, __5, __6), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5, __6 m_prm_6)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5, __6>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5, m_prm_6);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4, typename __5>
			task(R(*f)(__1, __2, __3, __4, __5), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4, __5 m_prm_5)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4, __5>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4, m_prm_5);
			}

		  template <typename R, typename __1, typename __2, typename __3, typename __4>
			task(R(*f)(__1, __2, __3, __4), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3, __4 m_prm_4)
			{
			   m_callable = new static_callback<R, __1, __2, __3, __4>(f, m_prm_1, m_prm_2, m_prm_3, m_prm_4);
			}

		  template <typename R, typename __1, typename __2, typename __3>
			task(R(*f)(__1, __2, __3), __1 m_prm_1, __2 m_prm_2, __3 m_prm_3)
			{
			   m_callable = new static_callback<R, __1, __2, __3>(f, m_prm_1, m_prm_2, m_prm_3);
			}

		  template <typename R, typename __1, typename __2>
			task(R(*f)(__1, __2), __1 m_prm_1, __2 m_prm_2)
			{
			   m_callable = new static_callback<R, __1, __2>(f, m_prm_1, m_prm_2);
			}

		  template <typename R, typename __1>
			task(R(*f)(__1), __1 m_prm_1)
			{
			   m_callable = new static_callback<R, __1>(f, m_prm_1);
			}

		  void run(range& r)
		  { 
			m_callable->run(r);
		  }

		  void run()
		  {
			m_callable->run();
		  }

		  void run(int i)
		  {
			m_callable->run(i);
		  }

		  /**
		   * to do
		   */
		  void run(range& r, shared_range * sr)
		  {
			m_callable->run();
		  }

		  
		  void detect_shared() { }

		  void set_shared(pointer p, lockable * l)
		  {
			m_callable->set_shared(p,l);
		  }

		  pointers get_in_data()
		  {
			return m_callable->get_in_data();
		  }

		  pointers get_out_data()
		  {
			return m_callable->get_out_data();
		  }

	 };
} // namespace xpu

#endif  // __XPU_TASK__


