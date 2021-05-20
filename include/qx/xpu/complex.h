/**
 * @file		complex_d.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		29-10-11
 * @brief	     advanced vectorization support : double precision complex
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

#ifndef __XPU_COMPLEX_D_H__
#define __XPU_COMPLEX_D_H__

#include <iostream>

#include <qx/compat.h>

#ifdef __AVX__
#include <immintrin.h> // avx
#endif

#ifdef __SSE__
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#ifdef __SSE4_1__
#include <smmintrin.h>
#endif
#else
#include <cmath>
#endif


namespace xpu 
{
   // typedef

   typedef QX_ALIGNED(16) struct _v2d
   {	 
      union 
      {
	 struct { double d1, d2; };
#ifdef __SSE__
	 __m128d xmm;
#endif 
      };
   } __v2d;



#ifdef __SSE__
   void dump_m128d(__m128d x)
   {
      __v2d xx;
      xx.xmm = x;
      std::cout << "[" << (xx.d1) << "," << (xx.d2) << "]" << std::endl;
   }
#endif

   typedef QX_ALIGNED(32) struct _v4d
   {	 
      union 
      {
	 struct { double d1, d2, d3, d4; };
#ifdef __AVX__
	 __m256d m;
#endif
      };
   } __v4d;


#ifdef __AVX__
   inline void dump_m256d(__m256d m)
   {
      __v4d x; 
      x.m = m;
      println("[ " << x.d1 << ", " << x.d2 << ", " << x.d3 << ", " << x.d4 << " ]");
   }
#endif 





   struct complex_d
   {
	 union 
	 {
	    struct { double im,re; };
#ifdef  __SSE__
	    __m128d xmm;
#endif
	 };

#ifdef  __SSE__
	 complex_d (__m128d v) : xmm (v) {}
#endif

	 complex_d ( ) 
	 { 
		 // Do not initialize as it may corrupt the `first touch`
// #ifdef  __SSE__
// 	    xmm = _mm_set1_pd(0); 
// #else
// 	    re = 0;
// 	    im = 0;
// #endif
	 }

	 complex_d (double v) 
	 { 
#ifdef  __SSE__
	    xmm = _mm_set_pd(v,0); 
#else
	    re = v;
	    im = 0;
#endif
	 }

	 complex_d (double re, double im)
	 {
#ifdef __SSE__
	    xmm = _mm_set_pd(re,im);
#else
	    this->re = re;
	    this->im = im;
#endif
	 }

	 complex_d (const double *v) 
	 { 
#ifdef __SSE__
	    xmm = _mm_loadu_pd(v);
#else
	    this->re = v[0];
	    this->im = v[1];
#endif
	 }

	 inline void operator = (const complex_d& v)
	 {
#ifdef __SSE__
	    xmm = v.xmm;
#else
	    this->re = v.re;
	    this->im = v.im;
#endif
	 }


	 inline void operator = (const double *v)
	 {
#ifdef __SSE__
	    xmm = _mm_loadu_pd(v);
#else
	    this->re = v[0];
	    this->im = v[1];
#endif
	 }

	 inline void operator = (const double v)
	 {
#ifdef __SSE__
	    // xmm = _mm_set1_pd(v);
	    this->re = v;
	    this->im = 0.; //(double)v;
#else
	    this->re = v;
	    this->im = 0.; //(double)v;
#endif
	 }


	 inline void operator = (const int v)
	 {
#ifdef __SSE__
	    // xmm = _mm_set1_pd((double)v);
	    this->re = (double)v;
	    this->im = 0.; //(double)v;
#else
	    this->re = (double)v;
	    this->im = 0.; //(double)v;
#endif
	 }


	 inline complex_d operator* (const complex_d &v) const
	 { 
	    #ifdef __SSE__
	    /*
	    __m128d mx, my; 
	    __m128d xr_xi, yr_yr;
	    __m128d xi_yi, yi_xr;
	    __m128d a,b;
	    mx    = xmm;   // _mm_load_pd((double *)x);
	    my    = v.xmm; // _mm_load_pd((double *)y);
	    xr_xi = mx;
	    yr_yr = _mm_shuffle_pd(my, my, 0); //_mm_broadcastsd_pd(my);
	    xi_yi = _mm_shuffle_pd(mx, my, 3);  // 11b = 3
	    yi_xr = _mm_shuffle_pd(my, mx, 1);  // 01b = 1
	    _mm_shuffle_pd(mx, mx, 1); 

	    a = _mm_mul_pd(xr_xi,yr_yr);
	    b = _mm_mul_pd(xi_yi,yi_xr);
	    a = _mm_addsub_pd(b,a);
	    // print(" result : "); dump_m128d(a);
	    // _mm_store_pd((double *)z,a);
	    return complex_d(a);
	    */
	    __m128d mx, my; 
	    // __m128d xr_xi, yr_yr; // v1, v2
	    // __m128d xi_yi, yi_xr; // v1, v2
	    __m128d v1, v2;
	    __m128d a,b;
	    mx    = xmm;   // _mm_load_pd((double *)x);
	    my    = v.xmm; // _mm_load_pd((double *)y);
	    /* xr_xi */ v1 = mx;
	    /* yr_yr */ v2 = _mm_shuffle_pd(my, my, 0); //_mm_broadcastsd_pd(my);
	    a = _mm_mul_pd(v1,v2);
	    // a = _mm_mul_pd(xr_xi,yr_yr);

	    /* xi_yi */ v1 = _mm_shuffle_pd(mx, my, 3);  // 11b = 3
	    /* yi_xr */ v2 = _mm_shuffle_pd(my, mx, 1);  // 01b = 1
	    // b = _mm_mul_pd(xi_yi,yi_xr);
	    b = _mm_mul_pd(v1,v2);
	    a = _mm_addsub_pd(b,a);
	    a = _mm_shuffle_pd(a, a, 1); 
	    // print(" result : "); dump_m128d(a);
	    // _mm_store_pd((double *)z,a);
	    return complex_d(a);

	    #else
	    return complex_d((re*v.re-im*v.im), 
	                     (im*v.re+v.im*re));
	    #endif
	 }

	 inline complex_d operator+ (const complex_d &v) const
	 { 
#ifdef __SSE__
	    return complex_d(_mm_add_pd(xmm, v.xmm)); 
#else
	    return complex_d(re+v.re, im+v.im);
#endif
	 }

	 inline complex_d operator- (const complex_d &v) const
	 { 
#ifdef __SSE__
	    return complex_d(_mm_sub_pd(xmm, v.xmm)); 
#else
	    return complex_d(re-v.re, im-v.im);
#endif
	 }

	 inline complex_d operator/ (const double &v) const
	 { 
	    #ifdef __SSE__
	    __m128d d = _mm_set1_pd(v);
	    return complex_d(_mm_div_pd(xmm, d)); 
	    #else
	    return complex_d(re/v, im/v);
	    #endif
	 }

	 inline void operator*= (const complex_d &v)
	 { 
	    #ifdef __SSE__DISABLE
	    /*
	    __m128d mx, my; 
	    __m128d xr_xi, yr_yr, xi_yi, yi_xr;
	    __m128d a,b;
	    mx    = xmm;   // _mm_load_pd((double *)x);
	    my    = v.xmm; // _mm_load_pd((double *)y);
	    xr_xi = mx;
	    yr_yr = _mm_shuffle_pd(my, my, 0); //_mm_broadcastsd_pd(my);
	    xi_yi = _mm_shuffle_pd(mx, my, 3);  // 11b = 3
	    yi_xr = _mm_shuffle_pd(my, mx, 1);  // 01b = 1
	    // _mm_shuffle_pd(mx, mx, 1); 

	    a = _mm_mul_pd(xr_xi,yr_yr);
	    b = _mm_mul_pd(xi_yi,yi_xr);
	    a = _mm_addsub_pd(b,a);
	    
	    xmm = a;
	    */
	    
	    __m128d mx, my; 
	    __m128d v1, v2;
	    __m128d a,b;
	    mx    = xmm;   // _mm_load_pd((double *)x);
	    my    = v.xmm; // _mm_load_pd((double *)y);
	    /* xr_xi */ v1 = mx;
	    /* yr_yr */ v2 = _mm_shuffle_pd(my, my, 0); //_mm_broadcastsd_pd(my);
	    a = _mm_mul_pd(v1,v2);
	    /* xi_yi */ v1 = _mm_shuffle_pd(mx, my, 3);  // 11b = 3
	    /* yi_xr */ v2 = _mm_shuffle_pd(my, mx, 1);  // 01b = 1
	    b = _mm_mul_pd(v1,v2);
	    xmm = _mm_addsub_pd(b,a);
	    // dump_m128d(a);
	    // xmm = a; // _mm_shuffle_pd(a, a, 1); 
	    #else
       double rt = re;  
	    re = (re*v.re-im*v.im); 
	    im = (im*v.re+v.im*rt);
	    #endif
	 }

	 inline void operator+= (const complex_d &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_add_pd(xmm, v.xmm); 
	    #else
	    re += v.re;
	    im += v.im; 
	    #endif
	 }

	 inline void operator-= (const complex_d &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_sub_pd(xmm, v.xmm); 
	    #else
	    re -= v.re;
	    im -= v.im; 
	    #endif
	 }

	 inline void operator/= (const double &v)
	 { 	    
	    #ifdef __SSE__
	    __m128d d = _mm_set1_pd(v);
	    xmm = _mm_div_pd(xmm, d); 
	    #else
	    re/=v; 
	    im/=v;
	    #endif
	 }

	 inline void operator/= (const complex_d &v)
	 { 	    
	    // #ifdef __SSE__
	    // __m128d d = _mm_set1_pd(v);
	    // xmm = _mm_div_pd(xmm, d); 
	    // #else
       double a = re, b = im, c = v.re, d = v.im;
	    re = (a*c+b*d)/(c*c+d*d); 
	    im = (b*c-a*d)/(c*c+d*d);
	    // #endif
    }
	

	 void operator>> (double *v)
	 { 
	    #ifdef __SSE__
	    _mm_store_pd(v, xmm); 
	    #else
	    v[0] = re;
	    v[1] = im; 
	    #endif
	 }
	
	 inline double norm()
	 {
		__m128d c = xmm; 
 	    c = _mm_mul_pd(c,c);
 	    c = _mm_hadd_pd(c,c);
 	    __v2d x; 
 	    x.xmm = c;
 	    return x.d1;
		// return _mm_cvtsd_f64(_mm_hadd_pd(_mm_mul_pd(xmm,xmm),_mm_set1_pd(0.)));
		// __m128d tmp = _mm_mul_pd(xmm,xmm);
		// return _mm_cvtsd_f64(_mm_hadd_pd(tmp, tmp));
	 }

	 friend std::ostream &operator<<(std::ostream &os, const complex_d& c)
	 //std::ostream& operator<<(std::ostream& os) const
	 {
	        os << "(" << c.re << "," << c.im << ")";
		//os << "tes";
		return os;
	 }

	 friend bool operator==(const complex_d& l, const complex_d& r)
	 {
	    return ((l.re==r.re) && (l.im==r.im));
	 }


	 friend bool operator!=(const complex_d& l, const complex_d& r)
	 {
	    return ((l.re!=r.re) || (l.im!=r.im));
	    //return true;
	 }
	 

	 complex_d conj()
	 {
	    return complex_d(re,-im);
	 }


         #ifdef __SSE__
	 /**
	  * mul(x,y) where :
	  * x : complex
	  * y : imaginary
	  */
	 inline static __m128d mul_ci(__m128d x, __m128d y)
	 {
	    // xy : | (-x.i*y.i) | (x.r*y.i) |  
	    // x  : | x.r | x.i |
	    // y  : | y.r |  0  |
	    // 
	    // t  : | - y.i | y.i | 
	    // xy : x*t
	    // __m128d t = _mm_broadcastsd_pd(y);
	    __m128d t = _mm_movedup_pd(y);              // 1 cyc
	    // dump_m128d(t);
	    t = _mm_xor_pd(t, _mm_set_pd(0.f,-0.0f));   // 1 cyc
	    // dump_m128d(t);
	    __m128d res = _mm_mul_pd(x,t);               // 5 cyc
	    res = _mm_shuffle_pd(res,res,1);            // 1 cyc
	    // dump_m128d(res);
	    return res;
	 }
	 
	 /**
	  * mul(x,y) where :
	  * x : complex
	  * y : real 
	  */
	 inline static __m128d mul_cr(__m128d x, __m128d y)
	 {
	    // xy : | (-x.i*y.i) | (x.r*y.i) |  
	    // x  : | x.r | x.i |
	    // y  : | y.r |  0  |
	    // 
	    // t  : | - y.i | y.i | 
	    // xy : x*t
	    __m128d t = _mm_shuffle_pd(y,y,3);
	    // dump_m128d(t);
	    return _mm_mul_pd(x,t);               // 5 cyc
	    // dump_m128d(res);
	    // return res;
	 }
	 
	 #ifdef __FMA__	 
	 /**
	  * mul(x1,y1)+mul(x1,y2) where :
	  * x1,x2 : complex
	  * y1,y2 : real 
	  */
         inline static __m128d _mm_cr_mul_add_pd(__m128d x1, __m128d y1, __m128d x2, __m128d y2)
	 {
	    // xy : | (-x.i*y.i) | (x.r*y.i) |  
	    // x  : | x.r | x.i |
	    // y  : | y.r |  0  |
	    // 
	    // t  : | - y.i | y.i | 
	    // xy : x*t
	    __m128d t1 = _mm_shuffle_pd(y1,y1,3);             // 1 cyc
	    __m128d t2 = _mm_shuffle_pd(y2,y2,3);             // 1 cyc
	    // dump_m128d(t);
	    t1 = _mm_mul_pd(x1,t1);                           // 5 cyc
	    // t2 = _mm_mul_pd(x2,t2);
	    // return _mm_add_pd(t1,t2);
	    return _mm_fmadd_pd (x2, t2, t1);  // x2*t2+t1    // 5 cyc
	    // dump_m128d(res);
	    // return res;
	 }
	 #endif // __FMA__

	 #endif // __SSE__
	 
	 #ifdef __AVX__
	 
	 /**
	  * mul(x1,y1)+mul(x1,y2) where :
	  * x1,x2 : complex
	  * y1,y2 : real 
	  */
         inline static __m128d _mm256_cr_mul_add_pd(__m128d x1, __m128d y1, __m128d x2, __m128d y2)
	 {
	    // xy : | (-x.i*y.i) | (x.r*y.i) |  
	    // x  : | x.r | x.i |
	    // y  : | y.r |  0  |
	    // 
	    // t  : | - y.i | y.i | 
	    // xy : x*t
	    __m128d t1 = _mm_shuffle_pd(y1,y1,3);             // 1 cyc
	    __m128d t2 = _mm_shuffle_pd(y2,y2,3);             // 1 cyc
	    
	    // __m256d t1t2 = _mm256_setr_m128d(t1,t2);          // 3
	    __m256d t1t2; 
	    t1t2 = _mm256_insertf128_pd(t1t2,t1,0);              // 3
	    t1t2 = _mm256_insertf128_pd(t1t2,t2,1);              // 3
	    // __m256d x1x2 = _mm256_setr_m128d(x1,x2);          // 3
	    __m256d x1x2;
	    x1x2 = _mm256_insertf128_pd(x1x2,x1,0);              // 3
	    x1x2 = _mm256_insertf128_pd(x1x2,x2,1);              // 3

	    t1t2 = _mm256_mul_pd(t1t2,x1x2);                  // 5
	    x1x2 = _mm256_permute2f128_pd (t1t2, t1t2, 1);    // 3 
	    t1t2 = _mm256_add_pd(t1t2,x1x2);                  // 3
	    // dump_m256d(t1t2);

	    return _mm256_extractf128_pd(t1t2, 0);            // 1
	    // dump_m128d(res);
	    // return res;
	 }
	 #endif // __AVX__
	 





   };

   #ifdef __AVX__

   static inline __m256d _mm256_cmul_pd(__m256d a, __m256d b) 
   {
      __m256d t1 = _mm256_movedup_pd(b);
      __m256d t2 = _mm256_shuffle_pd(b, b, 0xF);
      t1 = _mm256_mul_pd(a, t1);
      t2 = _mm256_mul_pd(a, t2);
      t2 = _mm256_shuffle_pd(t2, t2, 0x5);
      return _mm256_addsub_pd(t1, t2);
   }

   // static inline void _mm_cmul_add_pd(const complex_d &x1, const complex_d &x2, const complex_d &y1, const complex_d &y2, complex_d &res) 
   static inline void _mm_cmul_add_pd(const complex_d &x1, const complex_d &x2, const complex_d &y1, const complex_d &y2, complex_d &res) 
   {
      /*
       * __m256d _mm256_castpd128_pd256 (__m128d a)  // 0 cyc.
       * __m256d _mm256_shuffle_pd (__m256d a, __m256d b, const int imm8) 
       *
       */
      __m256d a,b,t; 
      __m128d ir;
      /*
      a  = _mm256_insertf128_pd(a,_mm_permute_pd(x1.xmm,1), 0); // 3 + 1 cyc.
      a  = _mm256_insertf128_pd(a,_mm_permute_pd(x2.xmm,1), 1); // 4
      b  = _mm256_insertf128_pd(b,_mm_permute_pd(y1.xmm,1), 0); // 4
      b  = _mm256_insertf128_pd(b,_mm_permute_pd(y2.xmm,1), 1); // 4
      */
      a = _mm256_castpd128_pd256(_mm_permute_pd(x1.xmm,1));                       // 0
      ir = _mm_permute_pd(x2.xmm,1);
      t = _mm256_broadcast_pd(&ir);                         // 1
      a = _mm256_blend_pd(a,t,12);  // 1100                    // 1

      b = _mm256_castpd128_pd256(_mm_permute_pd(y1.xmm,1));                       // 0
      ir = _mm_permute_pd(y2.xmm,1);
      t = _mm256_broadcast_pd(&ir);                         // 1
      b = _mm256_blend_pd(b,t,12);  // 1100                    // 1

      // a = _mm256_shuffle_f64x2(a,b,1);  // AVX-512 !!

      // std::cout << " a : "; dump_m256d(a);
      // std::cout << " b : "; dump_m256d(b);
      // mul
      __m256d t1 = _mm256_movedup_pd(b);                        // 1
      __m256d t2 = _mm256_shuffle_pd(b, b, 0xF);                // 1
      t1 = _mm256_mul_pd(a, t1);                                // 5
      t2 = _mm256_mul_pd(a, t2);                                // 5
      t2 = _mm256_shuffle_pd(t2, t2, 0x5);                      // 1
      a = _mm256_addsub_pd(t1, t2);                             // 3
      // add
      __m256d ar = _mm256_permute2f128_pd(a, a, 1);             // 3
      a = _mm256_add_pd(a,ar);                                  // 3
      res.xmm = _mm_permute_pd(_mm256_extractf128_pd(a,0),1);   // 1 + 1  -> total: 39 cyc.
   }

   /**
    * non-intrleaved complex
    */
   typedef struct {
      __m256d real;
      __m256d imag;
   } __m256c;

   // mul
   __m256c _mm256_mul_pc(__m256c a, __m256c b)
   {
      __m256c vec;
      __m256d t1 = _mm256_mul_pd(a.real, b.real);
      __m256d t2 = _mm256_mul_pd(a.imag, b.imag);
      vec.real = _mm256_sub_pd(t1, t2);
      t1 = _mm256_mul_pd(a.real, b.imag);
      t2 = _mm256_mul_pd(a.imag, b.real);
      vec.imag = _mm256_add_pd(t1, t2);
      return vec;
   }

   #endif
   
   #ifdef __SSE__
   
   typedef struct {
      __m128d real;
      __m128d imag;
   } __m128c;

   // mul
   __m128c _mm128_mul_pc(__m128c a, __m128c b)
   {
      __m128c vec;
      __m128d t1 = _mm_mul_pd(a.real, b.real);
      __m128d t2 = _mm_mul_pd(a.imag, b.imag);
      vec.real = _mm_sub_pd(t1, t2);
      t1 = _mm_mul_pd(a.real, b.imag);
      t2 = _mm_mul_pd(a.imag, b.real);
      vec.imag = _mm_add_pd(t1, t2);
      return vec;
   }

   static inline __m128d _mm128_mul_add_pc(__m128d x1, __m128d x2, __m128d y1, __m128d y2)
   {
      __m128c a;
      __m128c b;
      a.real = _mm_shuffle_pd(x1, y1, 3); // 00b
      a.imag = _mm_shuffle_pd(x1, y1, 0); // 11b
      b.real = _mm_shuffle_pd(x2, y2, 3); // 00b
      b.imag = _mm_shuffle_pd(x2, y2, 0); // 11b
      __m128d r1 = _mm_mul_pd(a.real, b.real);
      __m128d r2 = _mm_mul_pd(a.imag, b.imag);
      __m128d i1 = _mm_mul_pd(a.real, b.imag);
      __m128d i2 = _mm_mul_pd(a.imag, b.real);
      return _mm_hadd_pd(_mm_add_pd(i1, i2),_mm_sub_pd(r1,r2));
   }

   inline __m128d _mm_mulc_pd(__m128d mx, __m128d my)
   {
      __m128d v1, v2;
      __m128d a,b;
      v1 = mx;
      v2 = _mm_shuffle_pd(my, my, 0); //_mm_broadcastsd_pd(my);
      a = _mm_mul_pd(v1,v2);
      v1 = _mm_shuffle_pd(mx, my, 3);  // 11b = 3
      v2 = _mm_shuffle_pd(my, mx, 1);  // 01b = 1
      b = _mm_mul_pd(v1,v2);
      return _mm_addsub_pd(b,a);
   }


   #endif


} // namespace xpu

#endif // __XPU_COMPLEX_D_H__
