#include "qx/xpu/complex.h"

namespace xpu {
complex_d::complex_d(double v) {
    xmm = _mm_set_pd(v, 0);
}

complex_d::complex_d(double re, double im) {
    xmm = _mm_set_pd(re, im);
}

complex_d::complex_d(const double *v) {
    xmm = _mm_loadu_pd(v);
}

void complex_d::operator=(const complex_d &v) {
    xmm = v.xmm;
}

void complex_d::operator=(const double *v) {
    xmm = _mm_loadu_pd(v);
}

void complex_d::operator=(const double v) {
    this->re = v;
    this->im = 0.;
}

void complex_d::operator=(const int v) {
    this->re = (double)v;
    this->im = 0.;
}

complex_d complex_d::operator*(const complex_d &v) const {
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
    __m128d a, b;
    mx = xmm;   // _mm_load_pd((double *)x);
    my = v.xmm; // _mm_load_pd((double *)y);
    /* xr_xi */ v1 = mx;
    /* yr_yr */ v2 = _mm_shuffle_pd(my, my, 0); //_mm_broadcastsd_pd(my);
    a = _mm_mul_pd(v1, v2);
    // a = _mm_mul_pd(xr_xi,yr_yr);

    /* xi_yi */ v1 = _mm_shuffle_pd(mx, my, 3); // 11b = 3
    /* yi_xr */ v2 = _mm_shuffle_pd(my, mx, 1); // 01b = 1
    // b = _mm_mul_pd(xi_yi,yi_xr);
    b = _mm_mul_pd(v1, v2);
    a = _mm_addsub_pd(b, a);
    a = _mm_shuffle_pd(a, a, 1);
    // print(" result : "); dump_m128d(a);
    // _mm_store_pd((double *)z,a);
    return complex_d(a);
}

complex_d complex_d::operator+(const complex_d &v) const {
    return complex_d(_mm_add_pd(xmm, v.xmm));
}

complex_d complex_d::operator-(const complex_d &v) const {
    return complex_d(_mm_sub_pd(xmm, v.xmm));
}

complex_d complex_d::operator/(const double &v) const {
    __m128d d = _mm_set1_pd(v);
    return complex_d(_mm_div_pd(xmm, d));
}

void complex_d::operator*=(const complex_d &v) {
    double rt = re;
    re = (re * v.re - im * v.im);
    im = (im * v.re + v.im * rt);
}

void complex_d::operator+=(const complex_d &v) {
    xmm = _mm_add_pd(xmm, v.xmm);
}

void complex_d::operator-=(const complex_d &v) {
    xmm = _mm_sub_pd(xmm, v.xmm);
}

void complex_d::operator/=(const double &v) {
    __m128d d = _mm_set1_pd(v);
    xmm = _mm_div_pd(xmm, d);
}

void complex_d::operator/=(const complex_d &v) {
    double a = re, b = im, c = v.re, d = v.im;
    re = (a * c + b * d) / (c * c + d * d);
    im = (b * c - a * d) / (c * c + d * d);
}

void complex_d::operator>>(double *v) {
    _mm_store_pd(v, xmm);
}

struct QX_ALIGNED(16) __v2d {
    union {
        struct {
            double d1, d2;
        };
        __m128d xmm;
    };
};

double complex_d::norm() const {
    __m128d c = xmm;
    c = _mm_mul_pd(c, c);
    c = _mm_hadd_pd(c, c);
    __v2d x;
    x.xmm = c;
    return x.d1;
}

std::ostream &operator<<(std::ostream &os, const complex_d &c)
{
    os << c.re << " + " << c.im << " * i";
    return os;
}

bool operator==(const complex_d &l, const complex_d &r) {
    return ((l.re == r.re) && (l.im == r.im));
}

bool operator!=(const complex_d &l, const complex_d &r) {
    return ((l.re != r.re) || (l.im != r.im));
}

complex_d complex_d::conj() { return complex_d(re, -im); }

void dump_m128d(__m128d x) {
    complex_d xx;
    xx.xmm = x;
    std::cout << "[" << (xx.im) << "," << (xx.re) << "]" << std::endl;
}

struct __m128c {
    __m128d real;
    __m128d imag;
};

__m128d _mm128_mul_add_pc(__m128d x1, __m128d x2, __m128d y1,
                                        __m128d y2) {
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
    return _mm_hadd_pd(_mm_add_pd(i1, i2), _mm_sub_pd(r1, r2));
}

__m128d _mm_mulc_pd(__m128d mx, __m128d my) {
    __m128d v1, v2;
    __m128d a, b;
    v1 = mx;
    v2 = _mm_shuffle_pd(my, my, 0); //_mm_broadcastsd_pd(my);
    a = _mm_mul_pd(v1, v2);
    v1 = _mm_shuffle_pd(mx, my, 3); // 11b = 3
    v2 = _mm_shuffle_pd(my, mx, 1); // 01b = 1
    b = _mm_mul_pd(v1, v2);
    return _mm_addsub_pd(b, a);
}

__m128d mul_ci(__m128d x, __m128d y) {
    // xy : | (-x.i*y.i) | (x.r*y.i) |
    // x  : | x.r | x.i |
    // y  : | y.r |  0  |
    //
    // t  : | - y.i | y.i |
    // xy : x*t
    // __m128d t = _mm_broadcastsd_pd(y);
    __m128d t = _mm_movedup_pd(y); // 1 cyc
    // dump_m128d(t);
    t = _mm_xor_pd(t, _mm_set_pd(0.f, -0.0f)); // 1 cyc
    // dump_m128d(t);
    __m128d res = _mm_mul_pd(x, t);    // 5 cyc
    res = _mm_shuffle_pd(res, res, 1); // 1 cyc
    // dump_m128d(res);
    return res;
}

/**
 * mul(x,y) where :
 * x : complex
 * y : real
 */
__m128d mul_cr(__m128d x, __m128d y) {
    // xy : | (-x.i*y.i) | (x.r*y.i) |
    // x  : | x.r | x.i |
    // y  : | y.r |  0  |
    //
    // t  : | - y.i | y.i |
    // xy : x*t
    __m128d t = _mm_shuffle_pd(y, y, 3);
    // dump_m128d(t);
    return _mm_mul_pd(x, t); // 5 cyc
                                // dump_m128d(res);
                                // return res;
}

#ifdef __FMA__
/**
 * mul(x1,y1)+mul(x1,y2) where :
 * x1,x2 : complex
 * y1,y2 : real
 */
__m128d _mm_cr_mul_add_pd(__m128d x1, __m128d y1, __m128d x2,
                                        __m128d y2) {
    // xy : | (-x.i*y.i) | (x.r*y.i) |
    // x  : | x.r | x.i |
    // y  : | y.r |  0  |
    //
    // t  : | - y.i | y.i |
    // xy : x*t
    __m128d t1 = _mm_shuffle_pd(y1, y1, 3); // 1 cyc
    __m128d t2 = _mm_shuffle_pd(y2, y2, 3); // 1 cyc
    // dump_m128d(t);
    t1 = _mm_mul_pd(x1, t1); // 5 cyc
    // t2 = _mm_mul_pd(x2,t2);
    // return _mm_add_pd(t1,t2);
    return _mm_fmadd_pd(x2, t2, t1); // x2*t2+t1    // 5 cyc
                                        // dump_m128d(res);
                                        // return res;
}
#endif // __FMA__

#ifdef __AVX__
struct QX_ALIGNED(32) __v4d {
    union {
        struct {
            double d1, d2, d3, d4;
        };
        __m256d m;
    };
};

void dump_m256d(__m256d m) {
    __v4d x;
    x.m = m;
    println("[ ", x.d1, ", ", x.d2, ", ", x.d3, ", ", x.d4, " ]");
}

__m256d _mm256_cmul_pd(__m256d a, __m256d b) {
    __m256d t1 = _mm256_movedup_pd(b);
    __m256d t2 = _mm256_shuffle_pd(b, b, 0xF);
    t1 = _mm256_mul_pd(a, t1);
    t2 = _mm256_mul_pd(a, t2);
    t2 = _mm256_shuffle_pd(t2, t2, 0x5);
    return _mm256_addsub_pd(t1, t2);
}

// void _mm_cmul_add_pd(const complex_d &x1, const complex_d &x2,
// const complex_d &y1, const complex_d &y2, complex_d &res)
void _mm_cmul_add_pd(const complex_d &x1, const complex_d &x2,
                                   const complex_d &y1, const complex_d &y2,
                                   complex_d &res) {
    /*
     * __m256d _mm256_castpd128_pd256 (__m128d a)  // 0 cyc.
     * __m256d _mm256_shuffle_pd (__m256d a, __m256d b, const int imm8)
     *
     */
    __m256d a, b, t;
    __m128d ir;
    /*
    a  = _mm256_insertf128_pd(a,_mm_permute_pd(x1.xmm,1), 0); // 3 + 1 cyc.
    a  = _mm256_insertf128_pd(a,_mm_permute_pd(x2.xmm,1), 1); // 4
    b  = _mm256_insertf128_pd(b,_mm_permute_pd(y1.xmm,1), 0); // 4
    b  = _mm256_insertf128_pd(b,_mm_permute_pd(y2.xmm,1), 1); // 4
    */
    a = _mm256_castpd128_pd256(_mm_permute_pd(x1.xmm, 1)); // 0
    ir = _mm_permute_pd(x2.xmm, 1);
    t = _mm256_broadcast_pd(&ir);  // 1
    a = _mm256_blend_pd(a, t, 12); // 1100                    // 1

    b = _mm256_castpd128_pd256(_mm_permute_pd(y1.xmm, 1)); // 0
    ir = _mm_permute_pd(y2.xmm, 1);
    t = _mm256_broadcast_pd(&ir);  // 1
    b = _mm256_blend_pd(b, t, 12); // 1100                    // 1

    // a = _mm256_shuffle_f64x2(a,b,1);  // AVX-512 !!

    // std::cout << " a : "; dump_m256d(a);
    // std::cout << " b : "; dump_m256d(b);
    // mul
    __m256d t1 = _mm256_movedup_pd(b);         // 1
    __m256d t2 = _mm256_shuffle_pd(b, b, 0xF); // 1
    t1 = _mm256_mul_pd(a, t1);                 // 5
    t2 = _mm256_mul_pd(a, t2);                 // 5
    t2 = _mm256_shuffle_pd(t2, t2, 0x5);       // 1
    a = _mm256_addsub_pd(t1, t2);              // 3
    // add
    __m256d ar = _mm256_permute2f128_pd(a, a, 1); // 3
    a = _mm256_add_pd(a, ar);                     // 3
    res.xmm = _mm_permute_pd(_mm256_extractf128_pd(a, 0),
                             1); // 1 + 1  -> total: 39 cyc.
}

/**
 * mul(x1,y1)+mul(x1,y2) where :
 * x1,x2 : complex
 * y1,y2 : real
 */
__m128d _mm256_cr_mul_add_pd(__m128d x1, __m128d y1,
                                            __m128d x2, __m128d y2) {
    // xy : | (-x.i*y.i) | (x.r*y.i) |
    // x  : | x.r | x.i |
    // y  : | y.r |  0  |
    //
    // t  : | - y.i | y.i |
    // xy : x*t
    __m128d t1 = _mm_shuffle_pd(y1, y1, 3); // 1 cyc
    __m128d t2 = _mm_shuffle_pd(y2, y2, 3); // 1 cyc

    // __m256d t1t2 = _mm256_setr_m128d(t1,t2);          // 3
    __m256d t1t2;
    t1t2 = _mm256_insertf128_pd(t1t2, t1, 0); // 3
    t1t2 = _mm256_insertf128_pd(t1t2, t2, 1); // 3
    // __m256d x1x2 = _mm256_setr_m128d(x1,x2);          // 3
    __m256d x1x2;
    x1x2 = _mm256_insertf128_pd(x1x2, x1, 0); // 3
    x1x2 = _mm256_insertf128_pd(x1x2, x2, 1); // 3

    t1t2 = _mm256_mul_pd(t1t2, x1x2);             // 5
    x1x2 = _mm256_permute2f128_pd(t1t2, t1t2, 1); // 3
    t1t2 = _mm256_add_pd(t1t2, x1x2);             // 3
    // dump_m256d(t1t2);

    return _mm256_extractf128_pd(t1t2, 0); // 1
                                            // dump_m128d(res);
                                            // return res;
}
#endif

} // namespace xpu