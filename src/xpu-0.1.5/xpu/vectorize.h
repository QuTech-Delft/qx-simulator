/**
 * @file		vectorize.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     vectorization utils (based on SSE)
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

#ifndef __XPU_VECTORIZE_H__
#define __XPU_VECTORIZE_H__

#include <xpu/vec8s.h>
#include <xpu/vec4f.h>
#include <xpu/vec3f.h>


namespace xpu
{


   namespace debug 
   {

#ifdef  __SSE__

	 typedef struct _v4f
	 {	 
	    union 
	    {
		  struct { float f1, f2, f3, f4; };
		  __m128 xmm;
	    };
	 } __v4f __attribute__ ((aligned (16)));

	 typedef struct _v2d
	 {	 
	    union 
	    {
		  struct { double d1, d2; };
		  __m128d xmm;
	    };
	} __v2d __attribute__ ((aligned (16)));


	 typedef struct _v4i
	 {	 
	    union 
	    {
		  struct { int i1, i2, i3, i4; };
		  __m128i xmm;
	    };
	 } __v4i __attribute__ ((aligned (16)));


	 typedef struct _v2i
	 {	 
	    union 
	    {
		  struct { int i1, i2; };
		  __m64 xmm;
	    };
	 } __v2i __attribute__ ((aligned (16)));


	 typedef struct _v8s
	 {	 
	    union 
	    {
		  struct { short s1, s2, s3, s4, s5, s6, s7, s8; };
		  __m128i xmm;
	    };
	 } __v8s __attribute__ ((aligned (16)));

	 
	 typedef struct _v4s
	 {	 
	    union 
	    {
		  struct { short s1, s2, s3, s4; };
		  __m64 xmm;
	    };
	 } __v4s __attribute__ ((aligned (16)));


	 typedef struct _v16c
	 {	 
	    union 
	    {
		  struct { char c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16; };
		  __m128i xmm;
	    };
	 } __v16c __attribute__ ((aligned (16)));



	 typedef struct _v8c
	 {	 
	    union 
	    {
		  struct { char c1, c2, c3, c4, c5, c6, c7, c8; };
		  __m64 xmm;
	    };
	 } __v8c __attribute__ ((aligned (16)));


      
	 /**
	  * dump
	  */

	 inline void dump_m128f(__m128 xmm)
	 {
	    __v4f x; 
	    x.xmm = xmm;
	    println("[ " << x.f1 << ", " << x.f2 << ", " << x.f3 << ", " << x.f4 << " ]");
	 }

	 inline void dump_m128d(__m128d xmm)
	 {
	    __v2d x; 
	    x.xmm = xmm;
	    println("[ " << x.d1 << ", " << x.d2 << " ]");
	 }
	 
	 
	 inline void dump_m128i(__m128i xmm)
	 {
	    __v4i x; 
	    x.xmm = xmm;
	    println("[ " << x.i1 << ", " << x.i2 << ", " << x.i3 << ", " << x.i4 << " ]");
	 }

	 inline void dump_m64i(__m64 xmm)
	 {
	    __v2i x; 
	    x.xmm = xmm;
	    println("[ " << x.i1 << ", " << x.i2 << " ]");
	 }

	 inline void dump_m128s(__m128i xmm)
	 {
	    __v8s x; 
	    x.xmm = xmm;
	    println("[ " << x.s1 << ", " << x.s2 << ", " << x.s3 << ", " << x.s4 << ", " << x.s5 << ", " << x.s6 << ", " << x.s7 << ", " << x.s8 << " ]");
	 }

	 inline void dump_m64s(__m64 xmm)
	 {
	    __v4s x; 
	    x.xmm = xmm;
	    println("[ " << x.s1 << ", " << x.s2 << ", " << x.s3 << ", " << x.s4 << " ]");
	 }

	 inline void dump_m128c(__m128i xmm)
	 {
	    __v16c x; 
	    x.xmm = xmm;
	    println("[ " << (int)x.c1 << ", " << (int)x.c2 << ", " << (int)x.c3 << ", " << (int)x.c4 << ", " << (int)x.c5 << ", " << (int)x.c6 << ", " << (int)x.c7 << ", " << (int)x.c8 << ", " << (int)x.c9 << ", " << (int)x.c10 << ", " << (int)x.c11 << ", " << (int)x.c12 << ", " << (int)x.c13 << ", " << (int)x.c14 << ", " << (int)x.c15 << ", "  << (int)x.c16 << " ]");
	 } 


	 inline void dump_m64c(__m64 xmm)
	 {
	    __v8c x; 
	    x.xmm = xmm;
	    println("[ " << (int)x.c1 << ", " << (int)x.c2 << ", " << (int)x.c3 << ", " << (int)x.c4 << ", " << (int)x.c5 << ", " << (int)x.c6 << ", " << (int)x.c7 << ", " << (int)x.c8 << " ]");
	 }

#endif

   } // debug
} // xpu

namespace xpu
{
   namespace dsp
   {
	 /**
	  * slide
	  */
      #define __slide(i,x1,x2,i1,i2,window) \
		  t1 = _mm_srli_si128(x1,i); \
		  t2 = _mm_slli_si128(x2,(16-i)); \
		  window = _mm_or_si128(t1,t2);  //\
		  //dump_m128c(window); 

	 /**
	  * convert
	  */
      #define __convert(window,i1,i2,i3,i4,f1,f2,f3,f4) \
		  i1 = _mm_cvtepu8_epi32(window); \
		  f1 = _mm_cvtepi32_ps(i1); \
		  window = _mm_srli_si128(window,4); \
		  i2 = _mm_cvtepu8_epi32(window); \
		  f2 = _mm_cvtepi32_ps(i2); \
		  window = _mm_srli_si128(window,4); \
		  i3 = _mm_cvtepu8_epi32(window); \
		  f3 = _mm_cvtepi32_ps(i3); \
		  window = _mm_srli_si128(window,4); \
		  i4 = _mm_cvtepu8_epi32(window); \
		  f4 = _mm_cvtepi32_ps(i4);

	 /**
	  * convolve
	  */
      #define __convolve(f,f1,f2,f3,f4)   \
		  f1 = _mm_dp_ps(f1,k1,0xff); \
		  f2 = _mm_dp_ps(f2,k2,0xff); \
		  f3 = _mm_dp_ps(f3,k3,0xff); \
		  f4 = _mm_dp_ps(f4,k4,0xff); \
		  f  = _mm_add_ps(f1,_mm_add_ps(f2,_mm_add_ps(f3,f4))); \
		  f  = _mm_div_ps(f,ks);



	 /**
	  * application : 1d convolution
	  * x      : input
	  * kernel : 16 float kernel
	  * kernel_size : < 16
	  * out    : output
	  * n      : input/output size 
	  */
	 inline void convolve(const unsigned char * x, float * kernel, int kernel_size, float * out, int n)
	 {
	    __m128 ones = _mm_set1_ps(1);  // for dp      
	    // load kernel
	    __m128 k1 = _mm_load_ps(kernel);      
	    __m128 k2 = _mm_load_ps(kernel+4);    
	    __m128 k3 = _mm_load_ps(kernel+8);    
	    __m128 k4 = _mm_load_ps(kernel+12);   
	    // kernel sum
	    __m128 ks = _mm_add_ps(k1,_mm_add_ps(k2,_mm_add_ps(k3,k4)));
	    ks = _mm_dp_ps(ks,ones,0xff);

	    int center = kernel_size/2;
	   
	    // handle left boudary 
	    for (int c=0; c<kernel_size/2; c++)
	    {
		  float dot = 0.0;
		  float sum = 0.0;
		  for(int cc=(-center);cc<=center;cc++)
		  {
			if((c+cc) >= 0)
			{
			   dot += (float)x[(c+cc)] * kernel[center+cc];
			   sum += kernel[center+cc];
			}
		  }
		  out[c] = dot/sum;
	    }

	    int j = kernel_size/2; // w offset
         
	    for (int i=0; i<n-16; i+=16)
	    {
		  __m128i x1 = _mm_load_si128((__m128i*)(&x[i]));
		  __m128i x2 = _mm_load_si128((__m128i*)(&x[i+16]));
		  __m128i t1,t2,window; // sliding window
		  __m128i i1, i2, i3, i4;
		  __m128  f, f1, f2, f3, f4;
		  
		  window = x1;
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(1,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(2,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(3,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(4,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(5,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(6,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(7,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(8,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(9,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(10,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(11,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(12,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(13,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(14,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  __slide(15,x1,x2,i1,i2,window);
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);

		  // 16
		  window = x2;
		  __convert(window,i1,i2,i3,i4,f1,f2,f3,f4);
		  __convolve(f,f1,f2,f3,f4);
		  _mm_store_ss(&out[j++], f);
	    }

	    // handle right boudary 
	    for (int c=(n-(kernel_size/2)); c<n; c++)
	    {
		  float dot = 0.0;
		  float sum = 0.0;
		  for(int cc=(-center);cc<=center;cc++)
		  {
			if((c+cc) >= 0)
			{
			   dot += (float)x[(c+cc)] * kernel[center+cc];
			   sum += kernel[center+cc];
			}
		  }
		  out[c] = dot/sum;
	    }
	 } // covolve 
   } // dsp
} // xpu



#endif // __XPU_VECTORIZE_H__

