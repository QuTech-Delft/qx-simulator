/**
 * @file       vec8s.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		21-02-15
 * @copyright  none - confidential
 * @brief	     advanced vectorization support : 4 int16 (short) vector type
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
 */

#ifndef __XPU_VEC8S_H__
#define __XPU_VEC8S_H__

#ifdef __SSE__
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#else
#include <cmath>
#endif

#include <xpu/vec4f.h>

namespace xpu 
{
   struct vec8s
   {
	 union 
	 {
	    struct { short x0, x1, x2, x3, x4, x5, x6, x7; };
	    #ifdef  __SSE__
	    __m128i xmm;
	    #endif
	 };

	 #ifdef  __SSE__
	 vec8s (__m128i v) : xmm (v) {}
	 #endif

	 vec8s ( ) 
	 { 
	    #ifdef  __SSE__
	    xmm = _mm_set1_epi16(0); 
	    #else
	    x0 = 0;
	    x1 = 0;
	    x2 = 0;
	    x3 = 0;
	    x4 = 0;
	    x5 = 0;
	    x6 = 0;
	    x7 = 0;
	    #endif
	    }
	 
	 vec8s (short v) 
	 { 
	    #ifdef  __SSE__
	    xmm = _mm_set1_epi16(v); 
	    #else
	    x0 = v;
	    x1 = v;
	    x2 = v;
	    x3 = v;
	    x4 = v;
	    x5 = v;
	    x6 = v;
	    x7 = v;
	    #endif
	 }

	 vec8s (short x0, short x1, short x2, short x3,
	        short x4, short x5, short x6, short x7)
	 {
	    #ifdef __SSE__
	    xmm = _mm_set_epi16(x0, x1, x2, x3, x4, x5, x6, x7); 
	    #else
	    this->x0 = x0; 
	    this->x1 = x1; 
	    this->x2 = x2; 
	    this->x3 = x3; 
	    this->x4 = x4; 
	    this->x5 = x5; 
	    this->x6 = x6; 
	    this->x7 = x7; 
	    #endif
	 }

	 vec8s (const short *v) 
	 { 
	    #ifdef __SSE__
	    xmm = _mm_loadu_si128((__m128i*)v);
	    #else
	    this->x0 = v[0];
	    this->x1 = v[1];
	    this->x2 = v[2];
	    this->x3 = v[3];
	    this->x4 = v[4];
	    this->x5 = v[5];
	    this->x6 = v[6];
	    this->x7 = v[7];
	    #endif
	 }
	 
	 
	 vec8s (vec4f v1, vec4f v2) 
	 { 
	    #ifdef __SSE__
	    __m128  m1 = v1.xmm; // first  4 float
	    __m128  m2 = v2.xmm; // second 4 float
	    __m128i mi1 = _mm_cvtps_epi32(m1);  // convert float to int and round
	    __m128i mi2 = _mm_cvtps_epi32(m2);  // ..
	 
	    xmm = _mm_packs_epi32(mi1, mi2); 
	    //_mm_store_si128((__m128i *)(image+i), r);

	    #else
	    this->x0 = (short int)v1[0];
	    this->x1 = (short int)v1[1];
	    this->x2 = (short int)v1[2];
	    this->x3 = (short int)v1[3];
	    this->x4 = (short int)v2[0];
	    this->x5 = (short int)v2[1];
	    this->x6 = (short int)v2[2];
	    this->x7 = (short int)v2[3];
	    #endif
	 }
	 
	 
	 vec8s (vec4f v1, vec4f v2, float scale) 
	 { 
	    // scale before conversion
	    vec4f sc(scale);
	    v1 *= sc;
	    v2 *= sc;
	    #ifdef __SSE__
	    __m128  m1 = v1.xmm; // first  4 float
	    __m128  m2 = v2.xmm; // second 4 float
	    __m128i mi1 = _mm_cvtps_epi32(m1);  // convert float to int and round
	    __m128i mi2 = _mm_cvtps_epi32(m2);  // ..
	 
	    xmm = _mm_packs_epi32(mi1, mi2); 
	    //_mm_store_si128((__m128i *)(image+i), r);

	    #else
	    this->x0 = (short int)v1[0];
	    this->x1 = (short int)v1[1];
	    this->x2 = (short int)v1[2];
	    this->x3 = (short int)v1[3];
	    this->x4 = (short int)v2[0];
	    this->x5 = (short int)v2[1];
	    this->x6 = (short int)v2[2];
	    this->x7 = (short int)v2[3];
	    #endif
	 }


	 void operator = (const short *v)
	 {
	    #ifdef __SSE__
	    xmm = _mm_loadu_si128((__m128i*)v);
	    #else
	    this->x0 = v[0];
	    this->x1 = v[1];
	    this->x2 = v[2];
	    this->x3 = v[3];
	    this->x4 = v[4];
	    this->x5 = v[5];
	    this->x6 = v[6];
	    this->x7 = v[7];
	    #endif
	 }

	 vec8s operator* (const vec8s &v) const
	 { 
	    #ifdef __SSE__
	    //return vec8s(_mm_mul_epi16(xmm, v.xmm)); 
	    __m128i i1 = _mm_mullo_epi16(xmm,v.xmm);
	    __m128i i2 = _mm_mulhi_epi16(xmm,v.xmm);
	    return vec8s(_mm_packs_epi32(i1, i2));
	    #else
	    return vec8s(x0*v.x0, x1*v.x1, x2*v.x2, x3*v.x3, x4*v.x4, x5*v.x5, x6*v.x6, x7*v.x7);
	    #endif
	 }

	 vec8s operator+ (const vec8s &v) const
	 { 
	    #ifdef __SSE__
	    return vec8s(_mm_add_epi16(xmm, v.xmm)); 
	    #else
	    return vec8s(x0+v.x0, x1+v.x1, x2+v.x2, x3+v.x3, x4+v.x4, x5+v.x5, x6+v.x6, x7+v.x7);
	    #endif
	 }

	 vec8s operator- (const vec8s &v) const
	 { 
	    #ifdef __SSE__
	    return vec8s(_mm_sub_epi16(xmm, v.xmm)); 
	    #else
	    return vec8s(x0-v.x0, x1-v.x1, x2-v.x2, x3-v.x3, x4-v.x4, x5-v.x5, x6-v.x6, x7-v.x7);
	    #endif
	 }

	 vec8s operator/ (const vec8s &v) const
	 { 
	    //#ifdef __SSE__
	    //return vec8s(_mm_div_epi16(xmm, v.xmm)); 
	    //#else
	    return vec8s(x0/v.x0, x1/v.x1, x2/v.x2, x3/v.x3, x4/v.x4, x5/v.x5, x6/v.x6, x7/v.x7);
	    //#endif
	 }

	 void operator*= (const vec8s &v)
	 { 
	    #ifdef __SSE__
	    __m128i i1 = _mm_mullo_epi16(xmm,v.xmm);
	    __m128i i2 = _mm_mulhi_epi16(xmm,v.xmm);
	    xmm = _mm_packs_epi32(i1, i2);
	    #else
	    x0 *= v.x0;
	    x1 *= v.x1; 
	    x2 *= v.x2;
	    x3 *= v.x3;
	    x4 *= v.x4;
	    x5 *= v.x5; 
	    x6 *= v.x6;
	    x7 *= v.x7;
	    #endif
	 }

	 void operator+= (const vec8s &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_add_epi16(xmm, v.xmm); 
	    #else
	    x0 += v.x0;
	    x1 += v.x1; 
	    x2 += v.x2;
	    x3 += v.x3;
	    x4 += v.x4;
	    x5 += v.x5; 
	    x6 += v.x6;
	    x7 += v.x7;
	    #endif
	 }

	 void operator-= (const vec8s &v)
	 { 
	    #ifdef __SSE__
	    xmm = _mm_sub_epi16(xmm, v.xmm); 
	    #else
	    x0 -= v.x0;
	    x1 -= v.x1; 
	    x2 -= v.x2;
	    x3 -= v.x3;
	    x4 -= v.x4;
	    x5 -= v.x5; 
	    x6 -= v.x6;
	    x7 -= v.x7;
	    #endif
	 }

	 void operator/= (const vec8s &v)
	 { 
	    //#ifdef __SSE__
	    //xmm = _mm_div_epi16(xmm, v.xmm); 
	    //#else
	    x0 /= v.x0;
	    x1 /= v.x1; 
	    x2 /= v.x2;
	    x3 /= v.x3;
	    x4 /= v.x4;
	    x5 /= v.x5; 
	    x6 /= v.x6;
	    x7 /= v.x7;
	    //#endif
	 }

	 void operator>> (short *v)
	 { 
	    #ifdef __SSE__
	    //_mm_store_epi16(v, xmm); 
	    _mm_store_si128((__m128i *)v, xmm);
	    #else
	    v[0] = x0; 
	    v[1] = x1;
	    v[2] = x2;
	    v[3] = x3;
	    v[4] = x4;
	    v[5] = x5; 
	    v[6] = x6;
	    v[7] = x7;
	    #endif
	 }

   }; // vec4s

   /**
    * in-place transpose 8x8 16bits integers (short) matrix
    * --------------------------------------------
    * input/output  : m1
    */
   void transpose_8x8s(short int * m, const int lda, const int ldb) 
   {
	 __m128i a = _mm_load_si128((__m128i*)&m[0*lda]);
	 __m128i b = _mm_load_si128((__m128i*)&m[1*lda]);
	 __m128i c = _mm_load_si128((__m128i*)&m[2*lda]);
	 __m128i d = _mm_load_si128((__m128i*)&m[3*lda]);
	 __m128i e = _mm_load_si128((__m128i*)&m[4*lda]);
	 __m128i f = _mm_load_si128((__m128i*)&m[5*lda]);
	 __m128i g = _mm_load_si128((__m128i*)&m[6*lda]);
	 __m128i h = _mm_load_si128((__m128i*)&m[7*lda]);

/*
	 __m128i *p_input  = (__m128i*)array;
	 __m128i *p_output = (__m128i*)array;
	 __m128i a = _mm_load_si128(p_input++);
	 __m128i b = _mm_load_si128(p_input++);
	 __m128i c = _mm_load_si128(p_input++);
	 __m128i d = _mm_load_si128(p_input++);
	 __m128i e = _mm_load_si128(p_input++);
	 __m128i f = _mm_load_si128(p_input++);
	 __m128i g = _mm_load_si128(p_input++);
	 __m128i h = _mm_load_si128(p_input);
*/
	 __m128i a03b03 = _mm_unpacklo_epi16(a, b);
	 __m128i c03d03 = _mm_unpacklo_epi16(c, d);
	 __m128i e03f03 = _mm_unpacklo_epi16(e, f);
	 __m128i g03h03 = _mm_unpacklo_epi16(g, h);
	 __m128i a47b47 = _mm_unpackhi_epi16(a, b);
	 __m128i c47d47 = _mm_unpackhi_epi16(c, d);
	 __m128i e47f47 = _mm_unpackhi_epi16(e, f);
	 __m128i g47h47 = _mm_unpackhi_epi16(g, h);

	 __m128i a01b01c01d01 = _mm_unpacklo_epi32(a03b03, c03d03);
	 __m128i a23b23c23d23 = _mm_unpackhi_epi32(a03b03, c03d03);
	 __m128i e01f01g01h01 = _mm_unpacklo_epi32(e03f03, g03h03);
	 __m128i e23f23g23h23 = _mm_unpackhi_epi32(e03f03, g03h03);
	 __m128i a45b45c45d45 = _mm_unpacklo_epi32(a47b47, c47d47);
	 __m128i a67b67c67d67 = _mm_unpackhi_epi32(a47b47, c47d47);
	 __m128i e45f45g45h45 = _mm_unpacklo_epi32(e47f47, g47h47);
	 __m128i e67f67g67h67 = _mm_unpackhi_epi32(e47f47, g47h47);

	 __m128i a0b0c0d0e0f0g0h0 = _mm_unpacklo_epi64(a01b01c01d01, e01f01g01h01);
	 __m128i a1b1c1d1e1f1g1h1 = _mm_unpackhi_epi64(a01b01c01d01, e01f01g01h01);
	 __m128i a2b2c2d2e2f2g2h2 = _mm_unpacklo_epi64(a23b23c23d23, e23f23g23h23);
	 __m128i a3b3c3d3e3f3g3h3 = _mm_unpackhi_epi64(a23b23c23d23, e23f23g23h23);
	 __m128i a4b4c4d4e4f4g4h4 = _mm_unpacklo_epi64(a45b45c45d45, e45f45g45h45);
	 __m128i a5b5c5d5e5f5g5h5 = _mm_unpackhi_epi64(a45b45c45d45, e45f45g45h45);
	 __m128i a6b6c6d6e6f6g6h6 = _mm_unpacklo_epi64(a67b67c67d67, e67f67g67h67);
	 __m128i a7b7c7d7e7f7g7h7 = _mm_unpackhi_epi64(a67b67c67d67, e67f67g67h67);

	 _mm_store_si128((__m128i*)&m[0*ldb], a0b0c0d0e0f0g0h0);
	 _mm_store_si128((__m128i*)&m[1*ldb], a1b1c1d1e1f1g1h1);
	 _mm_store_si128((__m128i*)&m[2*ldb], a2b2c2d2e2f2g2h2);
	 _mm_store_si128((__m128i*)&m[3*ldb], a3b3c3d3e3f3g3h3);
	 _mm_store_si128((__m128i*)&m[4*ldb], a4b4c4d4e4f4g4h4);
	 _mm_store_si128((__m128i*)&m[5*ldb], a5b5c5d5e5f5g5h5);
	 _mm_store_si128((__m128i*)&m[6*ldb], a6b6c6d6e6f6g6h6);
	 _mm_store_si128((__m128i*)&m[7*ldb], a7b7c7d7e7f7g7h7);

/*
	 _mm_store_si128(p_output++, a0b0c0d0e0f0g0h0);
	 _mm_store_si128(p_output++, a1b1c1d1e1f1g1h1);
	 _mm_store_si128(p_output++, a2b2c2d2e2f2g2h2);
	 _mm_store_si128(p_output++, a3b3c3d3e3f3g3h3);
	 _mm_store_si128(p_output++, a4b4c4d4e4f4g4h4);
	 _mm_store_si128(p_output++, a5b5c5d5e5f5g5h5);
	 _mm_store_si128(p_output++, a6b6c6d6e6f6g6h6);
	 _mm_store_si128(p_output, a7b7c7d7e7f7g7h7);
*/
   }


   /**
    * transpose 8x8 16bits integers (short) matrix
    * --------------------------------------------
    * input  : m1
    * output : m2
    */
   void transpose_8x8s(const short int * m1, short int * m2, const int lda, const int ldb) 
   {
	 __m128i a = _mm_load_si128((__m128i*)&m1[0*lda]);
	 __m128i b = _mm_load_si128((__m128i*)&m1[1*lda]);
	 __m128i c = _mm_load_si128((__m128i*)&m1[2*lda]);
	 __m128i d = _mm_load_si128((__m128i*)&m1[3*lda]);
	 __m128i e = _mm_load_si128((__m128i*)&m1[4*lda]);
	 __m128i f = _mm_load_si128((__m128i*)&m1[5*lda]);
	 __m128i g = _mm_load_si128((__m128i*)&m1[6*lda]);
	 __m128i h = _mm_load_si128((__m128i*)&m1[7*lda]);

/*
	 __m128i *p_input  = (__m128i*)array;
	 __m128i *p_output = (__m128i*)array;
	 __m128i a = _mm_load_si128(p_input++);
	 __m128i b = _mm_load_si128(p_input++);
	 __m128i c = _mm_load_si128(p_input++);
	 __m128i d = _mm_load_si128(p_input++);
	 __m128i e = _mm_load_si128(p_input++);
	 __m128i f = _mm_load_si128(p_input++);
	 __m128i g = _mm_load_si128(p_input++);
	 __m128i h = _mm_load_si128(p_input);
*/
	 __m128i a03b03 = _mm_unpacklo_epi16(a, b);
	 __m128i c03d03 = _mm_unpacklo_epi16(c, d);
	 __m128i e03f03 = _mm_unpacklo_epi16(e, f);
	 __m128i g03h03 = _mm_unpacklo_epi16(g, h);
	 __m128i a47b47 = _mm_unpackhi_epi16(a, b);
	 __m128i c47d47 = _mm_unpackhi_epi16(c, d);
	 __m128i e47f47 = _mm_unpackhi_epi16(e, f);
	 __m128i g47h47 = _mm_unpackhi_epi16(g, h);

	 __m128i a01b01c01d01 = _mm_unpacklo_epi32(a03b03, c03d03);
	 __m128i a23b23c23d23 = _mm_unpackhi_epi32(a03b03, c03d03);
	 __m128i e01f01g01h01 = _mm_unpacklo_epi32(e03f03, g03h03);
	 __m128i e23f23g23h23 = _mm_unpackhi_epi32(e03f03, g03h03);
	 __m128i a45b45c45d45 = _mm_unpacklo_epi32(a47b47, c47d47);
	 __m128i a67b67c67d67 = _mm_unpackhi_epi32(a47b47, c47d47);
	 __m128i e45f45g45h45 = _mm_unpacklo_epi32(e47f47, g47h47);
	 __m128i e67f67g67h67 = _mm_unpackhi_epi32(e47f47, g47h47);

	 __m128i a0b0c0d0e0f0g0h0 = _mm_unpacklo_epi64(a01b01c01d01, e01f01g01h01);
	 __m128i a1b1c1d1e1f1g1h1 = _mm_unpackhi_epi64(a01b01c01d01, e01f01g01h01);
	 __m128i a2b2c2d2e2f2g2h2 = _mm_unpacklo_epi64(a23b23c23d23, e23f23g23h23);
	 __m128i a3b3c3d3e3f3g3h3 = _mm_unpackhi_epi64(a23b23c23d23, e23f23g23h23);
	 __m128i a4b4c4d4e4f4g4h4 = _mm_unpacklo_epi64(a45b45c45d45, e45f45g45h45);
	 __m128i a5b5c5d5e5f5g5h5 = _mm_unpackhi_epi64(a45b45c45d45, e45f45g45h45);
	 __m128i a6b6c6d6e6f6g6h6 = _mm_unpacklo_epi64(a67b67c67d67, e67f67g67h67);
	 __m128i a7b7c7d7e7f7g7h7 = _mm_unpackhi_epi64(a67b67c67d67, e67f67g67h67);

	 _mm_store_si128((__m128i*)&m2[0*ldb], a0b0c0d0e0f0g0h0);
	 _mm_store_si128((__m128i*)&m2[1*ldb], a1b1c1d1e1f1g1h1);
	 _mm_store_si128((__m128i*)&m2[2*ldb], a2b2c2d2e2f2g2h2);
	 _mm_store_si128((__m128i*)&m2[3*ldb], a3b3c3d3e3f3g3h3);
	 _mm_store_si128((__m128i*)&m2[4*ldb], a4b4c4d4e4f4g4h4);
	 _mm_store_si128((__m128i*)&m2[5*ldb], a5b5c5d5e5f5g5h5);
	 _mm_store_si128((__m128i*)&m2[6*ldb], a6b6c6d6e6f6g6h6);
	 _mm_store_si128((__m128i*)&m2[7*ldb], a7b7c7d7e7f7g7h7);

/*
	 _mm_store_si128(p_output++, a0b0c0d0e0f0g0h0);
	 _mm_store_si128(p_output++, a1b1c1d1e1f1g1h1);
	 _mm_store_si128(p_output++, a2b2c2d2e2f2g2h2);
	 _mm_store_si128(p_output++, a3b3c3d3e3f3g3h3);
	 _mm_store_si128(p_output++, a4b4c4d4e4f4g4h4);
	 _mm_store_si128(p_output++, a5b5c5d5e5f5g5h5);
	 _mm_store_si128(p_output++, a6b6c6d6e6f6g6h6);
	 _mm_store_si128(p_output, a7b7c7d7e7f7g7h7);
*/
   }
   


   void convert_float_to_short(const float * xf, short int * xs, size_t size)
   {
	 for (int i=0; i<size; i+=8)
	 {
	    __m128  f1 = _mm_load_ps(&xf[i]  ); 
	    __m128  f2 = _mm_load_ps(&xf[i+4]); 
	    __m128i i1 = _mm_cvtps_epi32(f1); 
	    __m128i i2 = _mm_cvtps_epi32(f2);

	    __m128i rs = _mm_packs_epi32(i1, i2); 
	    _mm_store_si128((__m128i *)(&xs[i]), rs);
	 }
   }


} // xpu


#endif


