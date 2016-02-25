/**
 * @file		types.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     for portability across platform (not really used : to be reviewed)
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

#ifndef __XPU_TYPES__
#define __XPU_TYPES__

#if defined(__sun) || defined(__linux) || defined(__osf__) || defined(_AIX) // this must be verified
#include <sys/types.h>
#else

// ---- convenience types ---- //
typedef unsigned char u_char;
typedef unsigned short int u_short;
typedef unsigned int u_int;
typedef unsigned long int u_long;

// ---- fixed-size types, underlying types : ---- //
// ---- depend on word size and compiler     ---- //
typedef signed char int8_t;
typedef unsigned char u_int8_t;
typedef signed short int int16_t;
typedef unsigned short int u_int16_t;
typedef signed int int32_t;
typedef unsigned int u_int32_t;

#if __WORDSIZE == 64

#elif defined __GLIBC_HAVE_LONG_LONG                          // for gnu compiler
__extension__ typedef signed long long int int64_t;
__extension__ typedef unsigned long long int u_int64_t;
#else
typedef signed long int int64_t;
typedef unsigned long int u_int64_t;
#endif // __GLIBC_HAVE_LONG_LONG
#endif // __unix__


typedef u_int32_t error_code_t; // 0 is success

#endif // __XPU_TYPES__

