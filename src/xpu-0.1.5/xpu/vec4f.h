/**
 * @file		vec4f.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     advanced vectorization support : 4 floats vector type
 *             (based on SSE)
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

#ifndef __XPU_VEC4F_H__
#define __XPU_VEC4F_H__

#ifdef __SSE__
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <xpu/xxmmintrin.h> // log / exp extension...
#else
#include <cmath>
#endif


namespace xpu 
{
   struct vec4f
   {
	 union 
	 {
	    struct { float x, y, z, w; };
	    #ifdef  __SSE__
	    __m128 xmm;
	    #endif
	 };

	 #ifdef  __SSE__
	 vec4f (__m128 v) : xmm (v) {}
	 #endif

	 vec4f ( ) 
	 { 
		  #ifdef  __SSE__
	    xmm = _mm_set1_ps(0); 
	    #else
	    x = 0;
	    y = 0;
	    z = 0;
	    w = 0;
	    #endif
	    }
	 
	 vec4f (float v) 
	 { 
	    #ifdef  __SSE__
	    xmm = _mm_set1_ps(v); 
	    #else
	    x = v;
	    y = v;
	    z = v;
	    w = v;
	    #endif
	 }

	 vec4f (float x, float y, float z, float w)
	 {
	    #ifdef __SSE__
	    xmm = _mm_set_ps(w,z,y,x); 
	    #else
	    this->x = x;
	    this->y = y;
	    this->z = z;
	    this->w = w;
	    #endif
	 }

	 vec4f (const float *v) 
	 { 
	    #ifdef __SSE__
	    xmm = _mm_loadu_ps(v);
	    #else
	    this->x = v[0];
	    this->y = v[1];
	    this->z = v[2];
	    this->w = v[3];
	    #endif
	 }

	 void operator = (const float *v)
	 {
	    #ifdef __SSE__
	    xmm = _mm_loadu_ps(v);
	    #else
	    this->x = v[0];
	    this->y = v[1];
	    this->z = v[2];
	    this->w = v[3];
	    #endif
	 }

	 vec4f operator* (const vec4f &v) const
	 { 
	    #ifdef __SSE__
	    return vec4f(_mm_mul_ps(xmm, v.xmm)); 
	    #else
	    return vec4f(x*v.x, y*v.y, z*v.z, w*v.w);
	    #endif
	 }

	 vec4f operator+ (const vec4f &v) const
	 { 
	    #ifdef __SSE__
	    return vec4f(_mm_add_ps(xmm, v.xmm)); 
	    #else
	    return vec4f(x+v.x, y+v.y, z+v.z, w+v.w);
	    #endif
	 }

	 vec4f operator- (const vec4f &v) const
	 { 
	    #ifdef __SSE__
	    return vec4f(_mm_sub_ps(xmm, v.xmm)); 
	    #else
	    return vec4f(x-v.x, y-v.y, z-v.z, w-v.w);
	    #endif
	 }

	 vec4f operator/ (const vec4f &v) const
	 { 
	    #ifdef __SSE__
	    return vec4f(_mm_div_ps(xmm, v.xmm)); 
	    #else
	    return vec4f(x/v.x, y/v.y, z/v.z, w/v.w);
	    #endif
	 }

	 void operator*= (const vec4f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_mul_ps(xmm, v.xmm); 
	    #else
	    x *= v.x;
	    y *= v.y; 
	    z *= v.z;
	    w *= v.w;
	    #endif
	 }

	 void operator+= (const vec4f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_add_ps(xmm, v.xmm); 
	    #else
	    x += v.x;
	    y += v.y; 
	    z += v.z;
	    w += v.w;
	    #endif
	 }

	 void operator-= (const vec4f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_sub_ps(xmm, v.xmm); 
	    #else
	    x -= v.x;
	    y -= v.y; 
	    z -= v.z;
	    w -= v.w;
	    #endif
	 }

	 void operator/= (const vec4f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_div_ps(xmm, v.xmm); 
	    #else
	    x /= v.x;
	    y /= v.y; 
	    z /= v.z;
	    w /= v.w;
	    #endif
	 }

	 void operator>> (float *v)
	 { 
	    #ifdef __SSE__
	    _mm_store_ps(v, xmm); 
	    #else
	    v[0] = x;
	    v[1] = y; 
	    v[2] = z;
	    v[3] = w;
	    #endif
	 }

	 vec4f sqrt()
	 { 
	    #ifdef __SSE__
	    return _mm_sqrt_ps(xmm); 
	    #else
	    return vec4f(::sqrt(x), ::sqrt(y), ::sqrt(z), ::sqrt(w));
	    #endif
	 }

	 vec4f rcp()
	 { 
	    #ifdef __SSE__
	    return _mm_rcp_ps(xmm); 
	    #else
	    return vec4f(1/x, 1/y, 1/z, 1/w);
	    #endif
	 }
	 
	 vec4f rsqrt()
	 { 
	    #ifdef __SSE__
	    return _mm_rsqrt_ps(xmm); 
	    #else
	    return vec4f(1/::sqrt(x), 1/::sqrt(y), 1/::sqrt(z), 1/::sqrt(w));
	    #endif
	 }

	 vec4f exp()
	 { 
	    #ifdef __SSE__
	    return _mm_exp_ps(xmm); 
	    #else
	    return vec4f(::exp(x), ::exp(y), ::exp(z), ::exp(w));
	    #endif
	 }

	 vec4f log()
	 { 
	    #ifdef __SSE__
	    return _mm_log_ps(xmm); 
		  #else
	    return vec4f(::log(x), ::log(y), ::log(z), ::log(w));
	    #endif
	 }
	 
	 vec4f sin()
	 { 
	    #ifdef __SSE__
		  return _mm_sin_ps(xmm); 
		  #else
	    return vec4f(::sin(x), ::sin(y), ::sin(z), ::sin(w));
	    #endif
	 }
	 
	 vec4f cos()
	 { 
	    #ifdef __SSE__
		  return _mm_cos_ps(xmm); 
		  #else
	    return vec4f(::sin(x), ::sin(y), ::sin(z), ::sin(w));
	    #endif
	 }

	 void sincos(vec4f * r_cos, vec4f * r_sin)
	 {
	    #ifdef __SSE__
	    _mm_sincos_ps(xmm, &(r_cos->xmm), &(r_sin->xmm));
		  #else
	    r_cos->x = ::cos(x);
	    r_cos->y = ::cos(y);
	    r_cos->z = ::cos(z);
	    r_cos->w = ::cos(w);

	    r_sin->x = ::sin(x);
	    r_sin->y = ::sin(y);
	    r_sin->z = ::sin(z);
	    r_sin->w = ::sin(w);
	    #endif
	 }
	 
	 /*
	 vec4f tan()
	 { return _mm_tan_ps(xmm); }

	 vec4f acos()
	 { return _mm_acos_ps(xmm); }

	 vec4f asin()
	 { return _mm_asin_ps(xmm); }

	 vec4f atan()
	 { return _mm_atan_ps(xmm); }
	 */
	 
	 inline
	 float sum()
	 {
	    //const __m128 t   = _mm_add_ps(xmm, _mm_movehl_ps(xmm,xmm));
	    //const __m128 sum = _mm_add_ss(t, _mm_shuffle_ps(t,t,1));
	    //const xpu::vec4f sum = _mm_add_ss(t, _mm_shuffle_ps(t,t,1));
	    //__m128 sum = _mm_hadd_ps(xmm,xmm);
	    //sum = _mm_hadd_ps(sum,sum);
	    //xpu::vec4f v = sum;
	    //return v.x;
	    //
	    ///*
	    #ifdef __SSE__
	    float s __attribute__ ((aligned (16)));
	    __m128 smm = _mm_hadd_ps(xmm, xmm);
	    smm = _mm_hadd_ps(smm, smm);
	    //const __m128 ones = _mm_set1_ps(1);
            //__m128 smm = _mm_dp_ps(xmm,ones,0xFF);
	    _mm_store_ss(&s, smm);
            return s;
            //return _mm_extract_ps(smm,0);
	    #else
	    //*/
	    return x+y+z+w;
            #endif
	 }


	 inline
	 void sum(float * v)
	 {
	    ///*
	    #ifdef __SSE__
	    //__m128 smm = _mm_hadd_ps(xmm, xmm);
	    //smm = _mm_hadd_ps(smm, smm);
	    const __m128 ones = _mm_set1_ps(1);
            __m128 smm = _mm_dp_ps(xmm,ones,0xFF);
	    _mm_storeu_ps(v, smm);
            //return _mm_extract_ps(smm,0);
	    #else
	    //*/
	    *v= x+y+z+w;
            #endif
	 }



   };

   
   /**
    * transpose 4x4 float matrix
    */
   void transpose_4x4f(float *a, float *b, const int lda, const int ldb) 
   {
	 __m128 row1 = _mm_load_ps(&a[0*lda]);
	 __m128 row2 = _mm_load_ps(&a[1*lda]);
	 __m128 row3 = _mm_load_ps(&a[2*lda]);
	 __m128 row4 = _mm_load_ps(&a[3*lda]);
	 _MM_TRANSPOSE4_PS(row1, row2, row3, row4);
	 _mm_store_ps(&b[0*ldb], row1);
	 _mm_store_ps(&b[1*ldb], row2);
	 _mm_store_ps(&b[2*ldb], row3);
	 _mm_store_ps(&b[3*ldb], row4);
   }

} // namespace xpu

#endif // __XPU_VEC4F_H__
