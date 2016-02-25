/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		17-03-13
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
#ifndef __XPU_STATIC_CALLBACK_79424B4E0A_H__
#define __XPU_STATIC_CALLBACK_79424B4E0A_H__


#include <xpu/task_group.h>
#include <xpu/utils.h>

namespace xpu
{

   /**
    * static callback
    */

   template < typename __R, typename __1 = void ,
		  typename __2  = void ,
		  typename __3  = void ,
		  typename __4  = void ,
		  typename __5  = void ,
		  typename __6  = void ,
		  typename __7  = void ,
		  typename __8  = void ,
		  typename __9  = void ,
		  typename __10 = void ,
		  typename __11 = void ,
		  typename __12 = void ,
		  typename __13 = void ,
		  typename __14 = void ,
		  typename __15 = void ,
		  typename __16 = void >
			class static_callback : public task_group
		  {
			typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14, __15, __16);

			private:

			__F         m_func;
			__R         m_retn;

               lockable *  m_locks[16];
         int         m_nb_locks;

			__1         m_prm_1;
			__2         m_prm_2;
			__3         m_prm_3;
			__4         m_prm_4;
			__5         m_prm_5;
			__6         m_prm_6;
			__7         m_prm_7;
			__8         m_prm_8;
			__9         m_prm_9;
			__10        m_prm_10;
			__11        m_prm_11;
			__12        m_prm_12;
			__13        m_prm_13;
			__14        m_prm_14;
			__15        m_prm_15;
			__16        m_prm_16;

			pointers    m_in_ptrs;
			pointers    m_out_ptrs;

			public:

			static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9, __10 prm_10, __11 prm_11, __12 prm_12, __13 prm_13, __14 prm_14, __15 prm_15,  __16 prm_16) : m_func(fnct), 
			m_prm_1(prm_1),
			m_prm_2(prm_2), 
			m_prm_3(prm_3), 
			m_prm_4(prm_4), 
			m_prm_5(prm_5), 
			m_prm_6(prm_6), 
			m_prm_7(prm_7), 
			m_prm_8(prm_8), 
			m_prm_9(prm_9), 
			m_prm_10(prm_10), 
			m_prm_11(prm_11), 
			m_prm_12(prm_12), 
			m_prm_13(prm_13), 
			m_prm_14(prm_14), 
			m_prm_15(prm_15), 
			m_prm_16(prm_16) 
			{
                  m_nb_locks = 0;
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_10);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_11);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_12);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_13);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_14);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_15);
			   add_ptr(m_in_ptrs, m_out_ptrs, prm_16);
			}

			void run(range& r)
			{ 
			   m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14,m_prm_15,m_prm_16);
			}
               
			void run(int i)
			{ 
			   m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14,m_prm_15,m_prm_16);
			}

			void run()
			{
			   for (int i=0; i<m_nb_locks; i++)
				 m_locks[i]->lock();
			   m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14,m_prm_15,m_prm_16);
			   for (int i=0; i<m_nb_locks; i++)
				 m_locks[i]->unlock();
			}

			pointers get_in_data()
			{
			   return m_in_ptrs;
			}

			pointers get_out_data()
			{
			   return m_out_ptrs;
			}
			
			
	          void set_shared(pointer p, lockable * l)
			{
			   pointers::iterator it = m_in_ptrs.find(p);
			   
			   if (it != m_in_ptrs.end())
			   {
				 set_lock(l);
				 return;
			   }

			   it = m_out_ptrs.find(p);

			   if (it != m_in_ptrs.end())
			   {
				 set_lock(l);
				 return;
			   }
			}
			
			void detect_shared() { }

			
			void set_lock(lockable* l)
			{
			   if (m_nb_locks <16)
			   {
				 m_locks[m_nb_locks++] = l;
			   }
			}

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 ,
		  typename __9 ,
		  typename __10,
		  typename __11,
		  typename __12,
		  typename __13,
		  typename __14,
		  typename __15>
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14, __15> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14, __15);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;
			   __9         m_prm_9;
			   __10        m_prm_10;
			   __11        m_prm_11;
			   __12        m_prm_12;
			   __13        m_prm_13;
			   __14        m_prm_14;
			   __15        m_prm_15;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9, __10 prm_10, __11 prm_11, __12 prm_12, __13 prm_13, __14 prm_14, __15 prm_15) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8), 
			   m_prm_9(prm_9), 
			   m_prm_10(prm_10), 
			   m_prm_11(prm_11), 
			   m_prm_12(prm_12), 
			   m_prm_13(prm_13), 
			   m_prm_14(prm_14), 
			   m_prm_15(prm_15)
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_10);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_11);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_12);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_13);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_14);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_15);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14,m_prm_15);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14,m_prm_15);
			   }



			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14,m_prm_15);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback



   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 ,
		  typename __9 ,
		  typename __10,
		  typename __11,
		  typename __12,
		  typename __13,
		  typename __14>
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13, __14);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;
			   __9         m_prm_9;
			   __10        m_prm_10;
			   __11        m_prm_11;
			   __12        m_prm_12;
			   __13        m_prm_13;
			   __14        m_prm_14;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9, __10 prm_10, __11 prm_11, __12 prm_12, __13 prm_13, __14 prm_14) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8), 
			   m_prm_9(prm_9), 
			   m_prm_10(prm_10), 
			   m_prm_11(prm_11), 
			   m_prm_12(prm_12), 
			   m_prm_13(prm_13), 
			   m_prm_14(prm_14)
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_10);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_11);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_12);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_13);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_14);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13,m_prm_14);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }


			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 ,
		  typename __9 ,
		  typename __10,
		  typename __11,
		  typename __12,
		  typename __13>
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12, __13);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;
			   __9         m_prm_9;
			   __10        m_prm_10;
			   __11        m_prm_11;
			   __12        m_prm_12;
			   __13        m_prm_13;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9, __10 prm_10, __11 prm_11, __12 prm_12, __13 prm_13) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8), 
			   m_prm_9(prm_9), 
			   m_prm_10(prm_10), 
			   m_prm_11(prm_11), 
			   m_prm_12(prm_12), 
			   m_prm_13(prm_13)
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_10);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_11);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_12);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_13);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12,m_prm_13);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback



   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 ,
		  typename __9 ,
		  typename __10,
		  typename __11,
		  typename __12>
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11, __12);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;
			   __9         m_prm_9;
			   __10        m_prm_10;
			   __11        m_prm_11;
			   __12        m_prm_12;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9, __10 prm_10, __11 prm_11, __12 prm_12) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8), 
			   m_prm_9(prm_9), 
			   m_prm_10(prm_10), 
			   m_prm_11(prm_11), 
			   m_prm_12(prm_12) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_10);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_11);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_12);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11,m_prm_12);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 ,
		  typename __9 ,
		  typename __10,
		  typename __11>
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __11);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;
			   __9         m_prm_9;
			   __10        m_prm_10;
			   __11        m_prm_11;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9, __10 prm_10, __11 prm_11) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8), 
			   m_prm_9(prm_9), 
			   m_prm_10(prm_10), 
			   m_prm_11(prm_11) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_10);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_11);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10,m_prm_11);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback



   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 ,
		  typename __9 ,
		  typename __10>
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;
			   __9         m_prm_9;
			   __10        m_prm_10;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9, __10 prm_10) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8), 
			   m_prm_9(prm_9), 
			   m_prm_10(prm_10) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_10);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9,m_prm_10);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 ,
		  typename __9 >
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8, __9> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8, __9);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;
			   __9         m_prm_9;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8, __9 prm_9) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8), 
			   m_prm_9(prm_9) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_9);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8,m_prm_9);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }

			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 ,
		  typename __8 >
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7, __8> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7, __8);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;
			   __8         m_prm_8;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7,  __8 prm_8) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7), 
			   m_prm_8(prm_8)
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_8);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7,m_prm_8);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2 ,
		  typename __3 ,
		  typename __4 ,
		  typename __5 ,
		  typename __6 ,
		  typename __7 >
			class static_callback<__R, __1, __2, __3, __4, __5, __6, __7> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6, __7);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;
			   __7         m_prm_7;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6, __7 prm_7) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6), 
			   m_prm_7(prm_7)
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_7);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6,m_prm_7);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6,m_prm_7);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback



   template < typename __R, typename __1,
		  typename __2,
		  typename __3,
		  typename __4,
		  typename __5,
		  typename __6 >
			class static_callback<__R, __1, __2, __3, __4, __5, __6> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5, __6);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;
			   __6         m_prm_6;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5, __6 prm_6) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5), 
			   m_prm_6(prm_6) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_6);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5,m_prm_6);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5,m_prm_6);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2,
		  typename __3,
		  typename __4,
		  typename __5 >
			class static_callback<__R, __1, __2, __3, __4, __5> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4, __5);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;
			   __5         m_prm_5;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4, __5 prm_5) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4), 
			   m_prm_5(prm_5) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_5);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4,m_prm_5);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4,m_prm_5);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4,m_prm_5);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback


   template < typename __R, typename __1,
		  typename __2,
		  typename __3,
		  typename __4 >
			class static_callback<__R, __1, __2, __3, __4> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3, __4);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;
			   __4         m_prm_4;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3, __4 prm_4) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3), 
			   m_prm_4(prm_4) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_4);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()),m_prm_4);
			   }
			   
			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3,m_prm_4);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3,m_prm_4);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback




   template < typename __R, typename __1,
		  typename __2,
		  typename __3 >
			class static_callback<__R, __1, __2, __3> : public task_group
			{
			   typedef __R (*__F)(__1, __2, __3);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;
			   __3         m_prm_3;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2, __3 prm_3) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2), 
			   m_prm_3(prm_3) 
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_3);
			   }

			   void run(range& r)
			   { 
				 m_func(__safe_cast(m_prm_1,r.begin()),__safe_cast(m_prm_2,r.end()),__safe_cast(m_prm_3,r.get_step()));
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2,m_prm_3);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2,m_prm_3);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }


			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback

   template < typename __R, typename __1,
		  typename __2 >
			class static_callback<__R, __1, __2> : public task_group
			{
			   typedef __R (*__F)(__1, __2);

			   private:

			   __F         m_func;
			   __R         m_retn;

                  lockable *  m_locks[16];
         int         m_nb_locks;

			   __1         m_prm_1;
			   __2         m_prm_2;

			   pointers    m_in_ptrs;
			   pointers    m_out_ptrs;

			   public:

			   static_callback(__F fnct, __1 prm_1, __2 prm_2) : m_func(fnct), 
			   m_prm_1(prm_1),
			   m_prm_2(prm_2)
			   {
                     m_nb_locks = 0;
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
				 add_ptr(m_in_ptrs, m_out_ptrs, prm_2);
			   }

			   void run(range& r)
			   { 
				 m_func(m_prm_1,m_prm_2);
			   }

			   void run(int i)
			   { 
				 m_func(__safe_cast(m_prm_1,i),m_prm_2);
			   }

			   void run()
			   {
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->lock();
				 m_func(m_prm_1,m_prm_2);
			      for (int i=0; i<m_nb_locks; i++)
				   m_locks[i]->unlock();
			   }

			   pointers get_in_data()
			   {
				 return m_in_ptrs;
			   }

			   pointers get_out_data()
			   {
				 return m_out_ptrs;
			   }

			   void set_shared(pointer p, lockable * l)
			   {
				 pointers::iterator it = m_in_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }

				 it = m_out_ptrs.find(p);

				 if (it != m_in_ptrs.end())
				 {
				    set_lock(l);
				    return;
				 }
			   }

			   void set_lock(lockable* l)
			   {
				 if (m_nb_locks <16)
				 {
				    m_locks[m_nb_locks++] = l;
				 }
			   }

			  void detect_shared() { }

     }; // class static_callback

   template < typename __R, typename __1>
	 class static_callback<__R, __1> : public task_group
	 {
	    typedef __R (*__F)(__1);

	    private:

	    __F         m_func;
	    __R         m_retn;

         lockable *  m_locks[16];
         int         m_nb_locks;

	    __1         m_prm_1;

	    pointers    m_in_ptrs;
	    pointers    m_out_ptrs;

	    public:

	    static_callback(__F fnct, __1 prm_1) : m_func(fnct), 
	    m_prm_1(prm_1)
	    {
            m_nb_locks = 0;
		  add_ptr(m_in_ptrs, m_out_ptrs, prm_1);
	    }

	    void run(range& r)
	    { 
		  m_func(m_prm_1);
	    }

	    void run(int i)
	    { 
		  m_func(__safe_cast(m_prm_1,i));
	    }

	    void run()
	    {
		  for (int i=0; i<m_nb_locks; i++)
			m_locks[i]->lock();
		  m_func(m_prm_1);
		  for (int i=0; i<m_nb_locks; i++)
			m_locks[i]->unlock();
	    }

	    pointers get_in_data()
	    {
		  return m_in_ptrs;
	    }

	    pointers get_out_data()
	    {
		  return m_out_ptrs;
	    }

	    void set_shared(pointer p, lockable * l)
	    {
		  pointers::iterator it = m_in_ptrs.find(p);

		  if (it != m_in_ptrs.end())
		  {
			set_lock(l);
			return;
		  }

		  it = m_out_ptrs.find(p);

		  if (it != m_in_ptrs.end())
		  {
			set_lock(l);
			return;
		  }
	    }

	    void set_lock(lockable* l)
	    {
		  if (m_nb_locks <16)
		  {
			m_locks[m_nb_locks++] = l;
		  }
	    }

	   void detect_shared() { }

     }; // class static_callback

} // namespace xpu

#endif // __XPU_STATIC_CALLBACK_79424B4E0A_H__
