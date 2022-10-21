#pragma once

#include <iostream>

#include "qx/compat.h"
#include "qx/core/printhelpers.h"

#ifndef __SSE__
#error "SSE not available !"
#endif

#ifdef __AVX__
#include <immintrin.h>
#endif

#include <emmintrin.h>
#include <pmmintrin.h>
#include <xmmintrin.h>
#ifdef __SSE4_1__
#include <smmintrin.h>
#endif

namespace xpu {
struct complex_d {
    union {
        struct {
            double im, re;
        };
        __m128d xmm;
    };


    complex_d(__m128d v) : xmm(v) {}

    complex_d() = default;

    complex_d(double v);

    complex_d(double re, double im);

    complex_d(const double *v);

    void operator=(const complex_d &v);

    void operator=(const double *v);

    void operator=(const double v);

    void operator=(const int v);

    complex_d operator*(const complex_d &v) const;

    complex_d operator+(const complex_d &v) const;

    complex_d operator-(const complex_d &v) const;

    complex_d operator/(const double &v) const;

    void operator*=(const complex_d &v);

    void operator+=(const complex_d &v);

    void operator-=(const complex_d &v);

    void operator/=(const double &v);

    void operator/=(const complex_d &v);

    void operator>>(double *v);

    double norm() const;

    friend std::ostream &operator<<(std::ostream &os, const complex_d &c);

    friend bool operator==(const complex_d &l, const complex_d &r);

    friend bool operator!=(const complex_d &l, const complex_d &r);

    complex_d conj();
};

void dump_m128d(__m128d x);

__m128d _mm128_mul_add_pc(__m128d x1, __m128d x2, __m128d y1,
                                        __m128d y2);

__m128d _mm_mulc_pd(__m128d mx, __m128d my);

__m128d mul_ci(__m128d x, __m128d y);

/**
 * mul(x,y) where :
 * x : complex
 * y : real
 */
__m128d mul_cr(__m128d x, __m128d y);

#ifdef __FMA__
/**
 * mul(x1,y1)+mul(x1,y2) where :
 * x1,x2 : complex
 * y1,y2 : real
 */
__m128d _mm_cr_mul_add_pd(__m128d x1, __m128d y1, __m128d x2,
                                        __m128d y2);
#endif // __FMA__

#ifdef __AVX__
void dump_m256d(__m256d m);

__m256d _mm256_cmul_pd(__m256d a, __m256d b);

void _mm_cmul_add_pd(const complex_d &x1, const complex_d &x2,
                                   const complex_d &y1, const complex_d &y2,
                                   complex_d &res);
                                
/**
 * mul(x1,y1)+mul(x1,y2) where :
 * x1,x2 : complex
 * y1,y2 : real
 */
__m128d _mm256_cr_mul_add_pd(__m128d x1, __m128d y1,
                                            __m128d x2, __m128d y2); 
#endif

} // namespace xpu