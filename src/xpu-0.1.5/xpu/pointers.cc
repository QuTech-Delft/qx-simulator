/**
 * @file		pointers.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief		smart pointer container
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

