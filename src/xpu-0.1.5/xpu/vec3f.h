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
 *
 */

#ifndef __XPU_VEC4F_H__
#define __XPU_VEC4F_H__

#include <cmath>

#ifdef __SSE__
#include <xmmintrin.h>
#include <emmintrin.h>
#include <xpu/xxmmintrin.h> // log / exp extension...
#else
#include <cmath>
#endif


namespace xpu 
{
   struct vec3f
   {
	 union 
	 {
	    struct { float x, y, z, w; };
	    #ifdef  __SSE__
	    __m128 xmm;
	    #endif
	 };

	 #ifdef  __SSE__
	 vec3f (__m128 v) : xmm (v) {}
	 #endif

	 vec3f ( ) 
	 { 
	    #ifdef  __SSE__
	    xmm = _mm_set1_ps(1.0f); 
	    #else
	    x = 0;
	    y = 0;
	    z = 0;
	    //w = 1.0f; // to avoid div / 0
	    #endif
	    }
	 
	 vec3f (float v) 
	 { 
	    #ifdef  __SSE__
	    xmm = _mm_set1_ps(v); 
	    #else
	    x = v;
	    y = v;
	    z = v;
	    //w = 1.0f; // to avoid div / 0
	    #endif
	 }

	 vec3f (float x, float y, float z)
	 {
	    #ifdef __SSE__
	    xmm = _mm_set_ps(1.0f,z,y,x); 
	    #else
	    this->x = x;
	    this->y = y;
	    this->z = z;
	    #endif
	 }

	 vec3f (const float *v) 
	 { 
	    #ifdef __SSE__
	    xmm = _mm_load_ps(v);
	    #else
	    this->x = v[0];
	    this->y = v[1];
	    this->z = v[2];
	    #endif
	 }

	 vec3f operator* (const vec3f &v) const
	 { 
	    #ifdef __SSE__
	    return vec3f(_mm_mul_ps(xmm, v.xmm)); 
	    #else
	    return vec3f(x*v.x, y*v.y, z*v.z);
	    #endif
	 }

	 vec3f operator+ (const vec3f &v) const
	 { 
	    #ifdef __SSE__
	    return vec3f(_mm_add_ps(xmm, v.xmm)); 
	    #else
	    return vec3f(x+v.x, y+v.y, z+v.z);
	    #endif
	 }

	 vec3f operator- (const vec3f &v) const
	 { 
	    #ifdef __SSE__
	    return vec3f(_mm_sub_ps(xmm, v.xmm)); 
	    #else
	    return vec3f(x-v.x, y-v.y, z-v.z);
	    #endif
	 }

	 vec3f operator/ (const vec3f &v) const
	 { 
	    #ifdef __SSE__
	    return vec3f(_mm_div_ps(xmm, v.xmm)); 
	    #else
	    return vec3f(x/v.x, y/v.y, z/v.z);
	    #endif
	 }

	 void operator*= (const vec3f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_mul_ps(xmm, v.xmm); 
	    #else
	    x *= v.x;
	    y *= v.y; 
	    z *= v.z;
	    #endif
	 }

	 void operator+= (const vec3f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_add_ps(xmm, v.xmm); 
	    #else
	    x += v.x;
	    y += v.y; 
	    z += v.z;
	    #endif
	 }

	 void operator-= (const vec3f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_sub_ps(xmm, v.xmm); 
	    #else
	    x -= v.x;
	    y -= v.y; 
	    z -= v.z;
	    #endif
	 }

	 void operator/= (const vec3f &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_div_ps(xmm, v.xmm); 
	    #else
	    x /= v.x;
	    y /= v.y; 
	    z /= v.z;
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
	    #endif
	 }

	 vec3f sqrt()
	 { 
	    #ifdef __SSE__
	    return _mm_sqrt_ps(xmm); 
	    #else
	    return vec3f(::sqrt(x), ::sqrt(y), ::sqrt(z));
	    #endif
	 }

	 vec3f rcp()
	 { 
	    #ifdef __SSE__
	    return _mm_rcp_ps(xmm); 
	    #else
	    return vec3f(1/x, 1/y, 1/z);
	    #endif
	 }
	 
	 vec3f rsqrt()
	 { 
	    #ifdef __SSE__
	    return _mm_rsqrt_ps(xmm); 
	    #else
	    return vec3f(1/::sqrt(x), 1/::sqrt(y), 1/::sqrt(z));
	    #endif
	 }

	 vec3f exp()
	 { 
	    #ifdef __SSE__
	    return _mm_exp_ps(xmm); 
	    #else
	    return vec3f(::exp(x), ::exp(y), ::exp(z));
	    #endif
	 }

	 vec3f log()
	 { 
	    #ifdef __SSE__
	    return _mm_log_ps(xmm); 
		  #else
	    return vec3f(::log(x), ::log(y), ::log(z));
	    #endif
	 }
	 
	 vec3f sin()
	 { 
	    #ifdef __SSE__
		  return _mm_sin_ps(xmm); 
		  #else
	    return vec3f(::sin(x), ::sin(y), ::sin(z));
	    #endif
	 }
	 
	 vec3f cos()
	 { 
	    #ifdef __SSE__
		  return _mm_cos_ps(xmm); 
		  #else
	    return vec3f(::sin(x), ::sin(y), ::sin(z));
	    #endif
	 }

	 void sincos(vec3f * r_cos, vec3f * r_sin)
	 {
	    #ifdef __SSE__
	    _mm_sincos_ps(xmm, &(r_cos->xmm), &(r_sin->xmm));
		  #else
	    r_cos->x = ::cos(x);
	    r_cos->y = ::cos(y);
	    r_cos->z = ::cos(z);

	    r_sin->x = ::sin(x);
	    r_sin->y = ::sin(y);
	    r_sin->z = ::sin(z);
	    #endif
	 }
	 
	 /*
	 vec3f tan()
	 { return _mm_tan_ps(xmm); }

	 vec3f acos()
	 { return _mm_acos_ps(xmm); }

	 vec3f asin()
	 { return _mm_asin_ps(xmm); }

	 vec3f atan()
	 { return _mm_atan_ps(xmm); }
	 */

	 float len()
	 {
	    return std::sqrt(x*x + y*y + z*z); 
	 }
	 
	 float squared_len()
	 {
	    return (x*x + y*y + z*z); 
	 }

	 float scalar(vec3f& v)
	 {
	    #ifdef __SSE__
	    return (x*v.x + y*v.y +x*v.z); 
	    #else
	    return (x*v.x + y*v.y + x*v.z); 
	    #endif
	 }

	 vec3f normalize()
	 {
	    float f = 1/len();
	    #ifdef __SSE__
	    // xmm       = _mm_mul_ps(xmm,__extension__ (__m128){f,f,f,f});   // icc problem
	    xmm       = _mm_mul_ps(xmm,vec3f(f).xmm);   
	    //xmm       = _mm_mul_ps(xmm, vec3f(f));   
	    #else
	    x *= f; 
	    y *= f; 
	    z *= f; 
	    #endif
	    return *this;
	 }

	 vec3f normalized()
	 {
	    vec3f r(x,y,z);
	    return r.normalize();
	 }

   };

   static inline vec3f middle(vec3f& v1, vec3f& v2)
   {
	 return vec3f((v1.x+v2.x)/2, (v1.y+v2.y)/2, (v1.z+v2.z)/2);
   }
   
   static inline float distance(vec3f &v1, vec3f &v2) 
   {
	 return (v1-v2).len();
   }

} // namespace xpu

#endif // __XPU_VEC4F_H__
