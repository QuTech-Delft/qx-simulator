/**
 * @file		gate.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		02-10-15
 * @brief
 */

#pragma once

#include <inttypes.h>
#include <map>
#include <functional>
#include <unordered_set>

#include <emmintrin.h> // sse
#include <immintrin.h> // avx

#include <algorithm>
#include <memory>

#include "qx/core/linalg.h"
#include "qx/core/register.h"
#include "qx/core/kronecker.h"

#include "qx/compat.h"

#include <chrono>

#ifdef USE_OPENMP
#include <omp.h>
#endif

// #ifndef __BUILTIN_LINALG__
// #include <boost/numeric/ublas/matrix.hpp>
// #endif

#define SQRT_2                                                                 \
    (1.4142135623730950488016887242096980785696718753769480731766797379f)
#define R_SQRT_2                                                               \
    (0.7071067811865475244008443621048490392848359376884740365883398690f)

#define __bit_test(x, pos) ((x) & (1 << (pos)))
#define __bit_set(x, pos) ((x) | (1 << (pos)))
#define __bit_flip(x, pos) ((x) ^ (1 << (pos)))
#define __bit_reset(x, pos) ((x) & ~(1 << (pos)))

#define __AVX__NO
#define __OP_PREFETCH__

//#define SQRT_2   (1.41421356237309504880f)
//#define R_SQRT_2 (0.70710678118654752440f)

#define ROUND_DOWN(x, s) ((x) & ~((s)-1))
#define IS_ODD(x) (x & 1)

namespace qx {
/**
 * types definition
 */
using basis_state_t = uint64_t;
using quantum_state_t = std::map<basis_state_t, complex_t>;
enum gate_type_t {
    __identity_gate__,
    __hadamard_gate__,
    __pauli_x_gate__,
    __pauli_y_gate__,
    __pauli_z_gate__,
    __cnot_gate__,
    __toffoli_gate__,
    __swap_gate__,
    __phase_gate__,
    __rx_gate__,
    __ry_gate__,
    __rz_gate__,
    __cphase_gate__,
    __t_gate__,
    __tdag_gate__,
    __sdag_gate__,
    __custom_gate__,
    __prepx_gate__,
    __prepy_gate__,
    __prepz_gate__,
    __measure_gate__,
    __measure_reg_gate__,
    __measure_x_gate__,
    __measure_x_reg_gate__,
    __measure_y_gate__,
    __measure_y_reg_gate__,
    __ctrl_phase_shift_gate__,
    __parallel_gate__,
    __display__,
    __display_binary__,
    __print_str__,
    __bin_ctrl_gate__,
    __lookup_table__,
    __classical_not_gate__,
    __qft_gate__,
    __prepare_gate__,
    __unitary_gate__
};

/**
 * gates coeffecients
 */
QX_ALIGNED(64)
const complex_t cnot_c[] = {
    complex_t(1.0), complex_t(0.0), complex_t(0.0), complex_t(0.0),
    complex_t(0.0), complex_t(1.0), complex_t(0.0), complex_t(0.0),
    complex_t(0.0), complex_t(0.0), complex_t(0.0), complex_t(1.0),
    complex_t(0.0), complex_t(0.0), complex_t(1.0), complex_t(0.0)}; /* CNOT */
QX_ALIGNED(64)
const complex_t swap_c[] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                            0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}; /* SWAP */
QX_ALIGNED(64)
const complex_t identity_c[] = {complex_t(1.0), complex_t(0.0), complex_t(0.0),
                                complex_t(1.0)}; /* I */
QX_ALIGNED(64)
const complex_t pauli_x_c[] = {complex_t(0.0, 0.0), complex_t(1.0, 0.0),
                               complex_t(1.0, 0.0),
                               complex_t(0.0, 0.0)}; /* X */
QX_ALIGNED(64)
const complex_t pauli_y_c[] = {complex_t(0.0, 0.0), complex_t(0.0, -1.0),
                               complex_t(0.0, 1.0),
                               complex_t(0.0, 0.0)}; /* Y */
QX_ALIGNED(64)
const complex_t pauli_z_c[] = {complex_t(1.0, 0.0), complex_t(0.0, 0.0),
                               complex_t(0.0, 0.0),
                               complex_t(-1.0, 0.0)}; /* Z */
QX_ALIGNED(64)
const complex_t phase_c[] = {complex_t(1.0, 0.0), complex_t(0.0, 0.0),
                             complex_t(0.0, 0.0), complex_t(0.0, 1.0)}; /* S */
QX_ALIGNED(64)
const complex_t sdag_gate_c[] = {complex_t(1.0, 0.0), complex_t(0.0, 0.0),
                                 complex_t(0.0, 0.0),
                                 complex_t(0.0, -1.0)}; /* S_dag */
QX_ALIGNED(64)
const complex_t t_gate_c[] = {
    complex_t(1.0, 0.0), complex_t(0.0, 0.0), complex_t(0.0, 0.0),
    complex_t(cos(QX_PI / 4), sin(QX_PI / 4))}; /* T */
QX_ALIGNED(64)
const complex_t tdag_gate_c[] = {
    complex_t(1.0, 0.0), complex_t(0.0, 0.0), complex_t(0.0, 0.0),
    complex_t(cos(QX_PI / 4), -sin(QX_PI / 4))}; /* T_dag */
QX_ALIGNED(64)
const complex_t hadamard_c[] = {R_SQRT_2, R_SQRT_2, R_SQRT_2,
                                -R_SQRT_2}; /* H */

#define __rc(r, c, s) (r * s + c)

/**
 * \brief common abstract gate interface for
 *   all gates implementation.
 */
class gate {
public:
    virtual void apply(qu_register &qureg) = 0;
    virtual gate_type_t type() const = 0;
    virtual std::string micro_code() {
        return "# unsupported operation : qubit out of range";
    }
    virtual void dump() = 0;
    virtual ~gate(){};

    virtual void set_duration(uint64_t d) { duration = d; }
    virtual uint64_t get_duration() { return duration; }

protected:
    uint64_t duration = 0;
};

/**
 * \brief rotation in the x-z plane with a given
 *     angle theta (see "Large scale simulation of
 *     error-prone quantum systems" p.39" [Niwa 2002])
 */
inline cmatrix_t rotation(double theta) {
    cmatrix_t r; // (2,2);
    r(0, 0) = complex_t(cos(theta), 0);
    r(0, 1) = complex_t(-sin(theta), 0);
    r(1, 0) = complex_t(sin(theta), 0);
    r(1, 1) = complex_t(cos(theta), 0);
    return r;
}

/**
 * \brief phase shift for a given angle phi
 */
inline cmatrix_t phase(double phi) {
    cmatrix_t p; // (2,2);
    p(0, 0) = complex_t(1, 0);
    p(0, 1) = complex_t(0, 0);
    p(1, 0) = complex_t(0, 0);
    p(1, 1) = complex_t(cos(phi), sin(phi));
    return p;
}

/**
 * \brief generate noisy hadamard gate
 */
cmatrix_t noisy_hadamard(double epsilon1 = 0, double epsilon2 = 0) {
#ifdef __BUILTIN_LINALG__
    return mxm(rotation(QX_PI / 4 + epsilon1), phase(QX_PI + epsilon2));
#else
    cmatrix_t rz = rotation(QX_PI / 4 + epsilon1);
    cmatrix_t p = phase(QX_PI + epsilon2);
    return mxm(rz, p);
#endif
}

/**
 * \brief build n x n matrix from an array
 */
cmatrix_t build_matrix(const complex_t *c, uint64_t n) {
    // assert(n==2);
    // TO DO : remove the n parameter
    cmatrix_t m; // (n,n);
    for (std::size_t i = 0; i < n; i++)
        for (std::size_t j = 0; j < n; j++)
            m(i, j) = c[i * n + j];
    return m;
}

/**
 * sqg_apply
 */
#ifdef QX_COMPACT_GATE_OP

inline void sqg_apply(cmatrix_t &cm, uint64_t qubit, qu_register &qureg) {
    uint64_t n = qureg.size();

    matrix_t m(2, row_t(2, 0));
    m[0][0] = cm(0, 0);
    m[0][1] = cm(0, 1);
    m[1][0] = cm(1, 0);
    m[1][1] = cm(1, 1);

    if (qubit == 0) {
        identity id(1UL << (n - 1));
        unitary_matrix um(cm.size1(), m);
        kronecker k(&id, &um);
        cvector_t r(qureg.get_data());
        mulmv(k, qureg.get_data(), r);
        qureg = r;
    } else if (qubit == n - 1) {
        identity id(1UL << (n - 1));
        unitary_matrix um(cm.size1(), m);
        kronecker k(&um, &id);
        cvector_t r(qureg.get_data());
        mulmv(k, qureg.get_data(), r);
        qureg = r;
    } else {
        identity id1(1UL << (qubit));
        identity id2(1UL << (n - qubit - 1));
        unitary_matrix um(cm.size1(), m);
        kronecker k(&id2, &um, &id1);
        cvector_t r(qureg.get_data());
        mulmv(k, qureg.get_data(), r);
        qureg = r;
    }
}

/**
 * u on the kth qubit :
 * non-null value in each row of the kronocker matrix:
 *     for each row r :
 *            c1 = r || 000100   // 1 at the n-k bit
 *            c2 = r || 000000
 */

// #elif QX_SPARSE_MV_MUL
#else // QX_SPARSE_MV_MUL

uint64_t rw_process(uint64_t is, uint64_t ie, uint64_t s, uint64_t n,
                    uint64_t qubit, const kronecker *m, cvector_t *v,
                    cvector_t *res) {
    uint64_t k = n - qubit;
    // println("run : ", is, " .. ", ie);
    complex_t *pv = v->data();
    complex_t *pr = res->data();
    size_t nk = n - k;

    for (uint64_t r = is; r < ie; ++r) {
        size_t bc = r;
        size_t c1 = __bit_reset(bc, nk);
        size_t c2 = __bit_set(bc, nk);
        // complex_t s; // = 0;
        pr[r] = pv[c1] * (m->get(r, c1)) + pv[c2] * (m->get(r, c2));
    }

    return 0;
}

void sparse_mulmv(uint64_t n, uint64_t qubit, const kronecker &m, cvector_t &v,
                  cvector_t &res) {
    uint64_t rows = (1UL << n);
    uint64_t z = 0UL;

    /*xpu::task rw_t(rw_process,0,0,0,n,qubit,&m,&v,&res);
    xpu::parallel_for process(z,rows,1,&rw_t);
    process.run();*/
    rw_process(z, rows, 0UL, n, qubit, &m, &v, &res);
}

void __apply_m(std::size_t start, std::size_t end, const std::size_t qubit,
               complex_t *state, const std::size_t stride0,
               const std::size_t stride1, const complex_t *matrix) {

#if 0
      __m128d m00 = matrix[0].xmm;
      __m128d m01 = matrix[1].xmm;
      __m128d m10 = matrix[2].xmm;
      __m128d m11 = matrix[3].xmm;
#endif

    complex_t m00 = matrix[0];
    complex_t m01 = matrix[1];
    complex_t m10 = matrix[2];
    complex_t m11 = matrix[3];

#ifdef USE_OPENMP
#pragma omp parallel for // shared(m00,m01,m10,m11)
#endif
    for (int64_t offset = start; offset < (int64_t)end;
         offset += (1UL << (qubit + 1)))
        for (size_t i = (size_t)offset; i < (size_t)offset + (1UL << qubit);
             i++) {
            size_t i0 = i + stride0;
            size_t i1 = i + stride1;

            complex_t in0 = state[i0];
            complex_t in1 = state[i1];
            state[i0] = m00 * in0 + m01 * in1;
            state[i1] = m10 * in0 + m11 * in1;
#if 0
            __m128d in0 = state[i0].xmm;
            __m128d in1 = state[i1].xmm;
            state[i0].xmm = _mm_add_pd(xpu::_mm_mulc_pd(m00, in0), xpu::_mm_mulc_pd(m10, in1));
            state[i1].xmm = _mm_add_pd(xpu::_mm_mulc_pd(m10, in1), xpu::_mm_mulc_pd(m11, in1));
#endif
        }
}

#ifdef __SSE__
// #ifdef __FMA__
void __apply_x(std::size_t start, std::size_t end, const std::size_t qubit,
               complex_t *state, const std::size_t stride0,
               const std::size_t stride1, const complex_t *matrix) {
#ifdef USE_OPENMP
#pragma omp parallel for // private(m00,r00,neg)
#endif
    for (int64_t offset = start; offset < (int64_t)end;
         offset += (1UL << (qubit + 1UL)))
        for (size_t i = (size_t)offset; i < (size_t)offset + (1UL << qubit);
             i++) {
            size_t i0 = i + stride0;
            size_t i1 = i + stride1;

            __m128d xin0 =
                state[i0].xmm; // _mm_load_pd((double*)&(state[i0].xmm));
            // __m128d xin1 = state[i1].xmm; //
            // _mm_load_pd((double*)&(state[i1].xmm));

            state[i0].xmm = state[i1].xmm;
            state[i1].xmm = xin0;
        }
}
// #else
// #error "FMA not available !"
// #endif // FMA
#else
#error "SSE not available !"
#endif // SSE

#ifdef __SSE__
// #ifdef __FMA__
void __apply_h(std::size_t start, std::size_t end, const std::size_t qubit,
               complex_t *state, const std::size_t stride0,
               const std::size_t stride1, const complex_t *matrix) {
    __m128d m00 = matrix[0].xmm;
    __m128d r00 = _mm_shuffle_pd(m00, m00, 3); // 1 cyc
    __m128d neg = _mm_set1_pd(-0.0f);

#ifdef USE_OPENMP
#pragma omp parallel for // private(m00,r00,neg)
#endif
    for (int64_t offset = start; offset < (int64_t)end;
         offset += (1UL << (qubit + 1UL)))
        for (size_t i = (size_t)offset; i < (size_t)offset + (1UL << qubit);
             i++) {
            size_t i0 = i + stride0;
            size_t i1 = i + stride1;

            __m128d xin0 =
                state[i0].xmm; // _mm_load_pd((double*)&(state[i0].xmm));
            __m128d xin1 =
                state[i1].xmm; // _mm_load_pd((double*)&(state[i1].xmm));

            __m128d t2; // = _mm_shuffle_pd(m01,m01,3);     // 1 cyc
            __m128d t1 = _mm_mul_pd(xin0, r00);        // 5 cyc
#ifdef __FMA__
            __m128d xi0 = _mm_fmadd_pd(xin1, r00, t1); // x2*t2+t1    // 5 cyc
#else
            __m128d xi0 = _mm_mul_pd(xin1, r00);
            xi0 = _mm_add_pd(xi0, t1); // x2*t2+t1    // 5 cyc
#endif // __FMA__
       // t2 = _mm_shuffle_pd(m11,m11,3);              // 1 cyc
            t2 = _mm_xor_pd(r00, neg);                 // 1 cyc  (m11=-m00)
#ifdef __FMA__
            __m128d xi1 = _mm_fmadd_pd(xin1, t2, t1);  // x2*t2+t1    // 5 cyc
#else
            __m128d xi1 = _mm_mul_pd(xin1, t2);
            xi1 = _mm_add_pd(xi1, t1); // x2*t2+t1    // 5 cyc
#endif

            state[i0].xmm = xi0; // _mm_store_pd((double*)(&state[i0].xmm),xi0);
            state[i1].xmm = xi1; // _mm_store_pd((double*)(&state[i1].xmm),xi1);
        }
}
// #else
// #error "FMA not available !"
// #endif // FMA
#else
#error "SSE not available !"
#endif // SSE

uint64_t rw_process_ui(uint64_t is, uint64_t ie, uint64_t s, uint64_t n,
                       uint64_t qubit, kronecker_ui m, cvector_t *v,
                       cvector_t *res) {
    uint64_t k = n - qubit;
    // println("run : ", is, " .. ", ie);
    complex_t *pv = v->data();
    complex_t *pr = res->data();
    size_t bc, c1, c2;
    size_t nk = n - k;

    for (uint64_t r = is; r < ie; ++r) {
        bc = r;
        c1 = __bit_reset(bc, nk);
        c2 = __bit_set(bc, nk);
        bc++;
#ifdef __OP_PREFETCH__
        _mm_prefetch((char *)&pv[__bit_reset(bc, nk)], _MM_HINT_T0);
        _mm_prefetch((char *)&pv[__bit_set(bc, nk)], _MM_HINT_T0);
#endif // __OP_PREFETCH__
#ifdef __AVX__
        // cxc
        xpu::_mm_cmul_add_pd(pv[c1], pv[c2], m.get(r, c1), m.get(r, c2), pr[r]);
        // cxr
        // pr[r].xmm = complex_t::_mm256_cr_mul_add_pd(pv[c1].xmm,
        // m.get(r,c1).xmm, pv[c2].xmm, m.get(r,c2).xmm);
#elif __SSE__
        // complex_t s; // = 0;
        // pr[r] = pv[c1]*(m->get(r,c1)) + pv[c2]*(m->get(r,c2));
        // --- cc mul add ---
        pr[r].xmm = _mm_add_pd((pv[c1] * (m.get(r, c1))).xmm,
                               (pv[c2] * (m.get(r, c2))).xmm);
        // --- cr mul add --- pr[r].xmm =
        // _mm_add_pd(complex_t::mul_cr(pv[c1].xmm,m.get(r,c1).xmm),
        // complex_t::mul_cr(pv[c2].xmm,m.get(r,c2).xmm));
        // --- f. mul add ---
        // pr[r].xmm = complex_t::_mm_cr_mul_add_pd(pv[c1].xmm, m.get(r,c1).xmm,
        // pv[c2].xmm, m.get(r,c2).xmm);
#else
        pr[r] = (pv[c1] * (m.get(r, c1))) + (pv[c2] * (m.get(r, c2)));
#endif
    }

    return 0;
}

void sparse_mulmv(uint64_t n, uint64_t qubit, kronecker_ui m, cvector_t &v,
                  cvector_t &res) {
    uint64_t rows = (1UL << n);
    uint64_t z = 0;

#ifdef SEQUENTIAL
    rw_process_ui(z, rows, 1, n, qubit, m, &v, &res);
#else
    /*xpu::task rw_t(rw_process_ui,0,0,0,n,qubit,m,&v,&res);
    xpu::parallel_for process(z,rows,1,&rw_t);
    process.run();*/
    rw_process_ui(z, rows, 0, n, qubit, m, &v, &res);
#endif
}

uint64_t rw_process_iu(uint64_t is, uint64_t ie, uint64_t s, uint64_t n,
                       uint64_t qubit, kronecker_iu m, cvector_t *v,
                       cvector_t *res) {
    uint64_t k = n - qubit;
    // println("run : ", is, " .. ", ie);
    complex_t *pv = v->data();
    complex_t *pr = res->data();
    size_t bc, c1, c2;
    size_t nk = n - k;

    for (uint64_t r = is; r < ie; ++r) {
        bc = r;
        c1 = __bit_reset(bc, nk);
        c2 = __bit_set(bc, nk);
        bc++;
#ifdef __OP_PREFETCH__
        _mm_prefetch((char *)&pv[__bit_reset(bc, nk)], _MM_HINT_T0);
        _mm_prefetch((char *)&pv[__bit_set(bc, nk)], _MM_HINT_T0);
#endif // __OP_PREFETCH__
#ifdef __AVX__
        // cxc
        xpu::_mm_cmul_add_pd(pv[c1], pv[c2], m.get(r, c1), m.get(r, c2), pr[r]);
        // cxr
        // pr[r].xmm = complex_t::_mm256_cr_mul_add_pd(pv[c1].xmm,
        // m.get(r,c1).xmm, pv[c2].xmm, m.get(r,c2).xmm);
#elif __SSE__
        // complex_t s; // = 0;
        // pr[r] = pv[c1]*(m->get(r,c1)) + pv[c2]*(m->get(r,c2));
        // --- cc mul add ---
        pr[r].xmm = _mm_add_pd((pv[c1] * (m.get(r, c1))).xmm,
                               (pv[c2] * (m.get(r, c2))).xmm);
        // --- cr mul add --- pr[r].xmm =
        // _mm_add_pd(complex_t::mul_cr(pv[c1].xmm,m.get(r,c1).xmm),
        // complex_t::mul_cr(pv[c2].xmm,m.get(r,c2).xmm));
        // --- f. mul add ---
        // pr[r].xmm = complex_t::_mm_cr_mul_add_pd(pv[c1].xmm, m.get(r,c1).xmm,
        // pv[c2].xmm, m.get(r,c2).xmm);
#else
        pr[r] = (pv[c1] * (m.get(r, c1))) + (pv[c2] * (m.get(r, c2)));
#endif
    }

    return 0;
}

void sparse_mulmv(uint64_t n, uint64_t qubit, kronecker_iu m, cvector_t &v,
                  cvector_t &res) {
    uint64_t rows = (1UL << n);
    uint64_t z = 0;

#ifdef SEQUENTIAL
    rw_process_iu(z, rows, 1, n, qubit, m, &v, &res);
#else
    /*xpu::task rw_t(rw_process_iu,0,0,0,n,qubit,m,&v,&res);
    xpu::parallel_for process(z,rows,1,&rw_t);
    process.run();*/
    rw_process_iu(z, rows, 0, n, qubit, m, &v, &res);
#endif
}

// static xpu::core::os::mutex mtx;

uint64_t rw_process_iui(uint64_t is, uint64_t ie, uint64_t s, uint64_t n,
                        uint64_t qubit, kronecker_iui m, cvector_t *v,
                        cvector_t *res) {
    uint64_t k = n - qubit;
    // println("run : " , is , " .. " , ie);
    complex_t *pv = v->data();
    complex_t *pr = res->data();
    size_t bc, c1, c2;
    size_t nk = n - k;

    for (uint64_t r = is; r < ie; r++) //+=2)
    {
        // 1st
        bc = r;
        c1 = __bit_reset(bc, nk);
        c2 = __bit_set(bc, nk);
        bc++;
#ifdef __OP_PREFETCH__
        _mm_prefetch((char *)&pv[__bit_reset(bc, nk)], _MM_HINT_T0);
        _mm_prefetch((char *)&pv[__bit_set(bc, nk)], _MM_HINT_T0);
#endif // __OP_PREFETCH__
#ifdef __AVX__
        // mtx.lock();
        // cxc :
        xpu::_mm_cmul_add_pd(pv[c1], pv[c2], m.get(r, c1), m.get(r, c2), pr[r]);
        // cxr
        // pr[r].xmm = complex_t::_mm256_cr_mul_add_pd(pv[c1].xmm,
        // m.get(r,c1).xmm, pv[c2].xmm, m.get(r,c2).xmm);

        /*
           __m256d a; //_mm256_loadu2_m128d((double*)&pv[c1], (double*)&pv[c2]);
           a  = _mm256_insertf128_pd(a,_mm_permute_pd(pv[c1].xmm,1), 0);
           a  = _mm256_insertf128_pd(a,_mm_permute_pd(pv[c2].xmm,1), 1);
           print("(r="<<r<<") : pr12: "); xpu::dump_m256d(a);
        // __m256d b  = _mm256_set_m128d((m.get(r,c1)).xmm, (m.get(r,c2)).xmm);
        __m256d b;
        b  = _mm256_insertf128_pd(b,_mm_permute_pd(m.get(r,c1).xmm, 1), 1);
        print("(r="<<r<<") : c1 : "); xpu::dump_m256d(b);
        b  = _mm256_insertf128_pd(b,_mm_permute_pd(m.get(r,c2).xmm, 1), 0);
        print("(r="<<r<<") : c2 : "); xpu::dump_m256d(b);
        __m256d ab  = xpu::_mm256_cmul_pd(a,b);
        print("(r="<<r<<") : mul: "); xpu::dump_m256d(ab);
        __m256d abr = _mm256_permute2f128_pd(ab, ab, 1);
        print("(r="<<r<<") : prm: "); xpu::dump_m256d(abr);
        ab = _mm256_add_pd(ab,abr);
        print("(r="<<r<<") : add: "); xpu::dump_m256d(ab);
        pr[r].xmm = _mm256_extractf128_pd(ab,0);
        print("(r="<<r<<") : res:"); xpu::dump_m128d(pr[r].xmm);
        mtx.unlock();
         */
#elif __SSE__
        /*
           mtx.lock();
           print("(r="<<r<<") : pr1: "); xpu::dump_m128d(pv[c1].xmm);
           print("(r="<<r<<") : pr2: "); xpu::dump_m128d(pv[c2].xmm);
           print("(r="<<r<<") : c1 : "); xpu::dump_m128d((m.get(r,c1)).xmm);
           print("(r="<<r<<") : c2 : "); xpu::dump_m128d((m.get(r,c2)).xmm);
         */
        // --- cxc mul ---
        pr[r].xmm = _mm_add_pd((pv[c1] * (m.get(r, c1))).xmm,
                               (pv[c2] * (m.get(r, c2))).xmm);
        // --- cxr mul --- pr[r].xmm =
        // _mm_add_pd(complex_t::mul_cr(pv[c1].xmm,m.get(r,c1).xmm),
        // complex_t::mul_cr(pv[c2].xmm,m.get(r,c2).xmm));
        // --- fus ma  ---
        // pr[r].xmm = complex_t::_mm_cr_mul_add_pd(pv[c1].xmm, m.get(r,c1).xmm,
        // pv[c2].xmm, m.get(r,c2).xmm);

        // pr[r].xmm = xpu::_mm128_mul_add_pc(pv[c1].xmm, pv[c2].xmm,
        // m.get(r,c1).xmm, m.get(r,c2).xmm);
        /*
           print("(r="<<r<<") : res: "); xpu::dump_m128d(pr[r].xmm);
           mtx.unlock();
         */
#else
        pr[r] = (pv[c1] * (m.get(r, c1))) + (pv[c2] * (m.get(r, c2)));
#endif
        /*
        // 2nd
        c1 = __bit_reset(bc,n-k);
        c2 = __bit_set(bc,n-k);
#ifdef __AVX__NO
a  = _mm256_loadu2_m128d((double*)&pv[c1], (double*)&pv[c2]);
        // __m256d b  = _mm256_set_m128d((m.get(r,c1)).xmm, (m.get(r,c2)).xmm);
        b  = _mm256_insertf128_pd(b,(m.get(bc,c1)).xmm, 1);
        b  = _mm256_insertf128_pd(b,(m.get(bc,c2)).xmm, 0);
        ab  = xpu::_mm256_cmul_pd(a,b);
        abr = _mm256_permute2f128_pd(ab, ab, 1);
        ab = _mm256_add_pd(ab,abr);
        pr[bc].xmm = _mm256_extractf128_pd(ab,0);
#elif  __SSE__
pr[bc].xmm = _mm_add_pd((pv[c1]*(m.get(bc,c1))).xmm,
(pv[c2]*(m.get(bc,c2))).xmm); #else pr[bc] = (pv[c1]*(m.get(bc,c1))) +
(pv[c2]*(m.get(bc,c2))); #endif
         */
    }

    return 0;
}

void sparse_mulmv(uint64_t n, uint64_t qubit, kronecker_iui m, cvector_t &v,
                  cvector_t &res) {
    uint64_t rows = (1UL << n);

#ifdef SEQUENTIAL
    rw_process_iui(z, rows, 1, n, qubit, m, &v, &res);
#else
    /*xpu::task rw_t(rw_process_iui,0,0,0,n,qubit,m,&v,&res);
    xpu::parallel_for process(z,rows,1,&rw_t);
    process.run();*/
#endif
}

inline void sqg_apply(cmatrix_t &cm, uint64_t qubit, qu_register &qureg) {
    uint64_t n = qureg.size();
    complex_t *s = qureg.get_data().data();
    // cm.dump();
    __apply_m(0, (1UL << n), qubit, s, 0, (1UL << qubit), cm.m);
    return;
}

#endif // remove naive tensor computation

enum elementary_operation_t { __x180__, __x90__, __y180__, __y90__, __ym90__ };

static const char *pulse_lt[][5] = {
    {"  pulse 9,0,0", "  pulse 10,0,0", "  pulse 11,0,0", "  pulse 12,0,0",
     "  pulse 14,0,0"},
    {"  pulse 0,9,0", "  pulse 0,10,0", "  pulse 0,11,0", "  pulse 0,12,0",
     "  pulse 0,14,0"},
    {"  pulse 0,0,9", "  pulse 0,0,10", "  pulse 0,0,11", "  pulse 0,0,12",
     "  pulse 0,0,14"},
};

/**
 * \brief hadamard gate:
 *
 *               | 1   1|
 *     1/sqrt(2) |      |
 *               | 1  -1|
 */
class hadamard final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    hadamard(uint64_t qubit)
        : qubit(qubit) //,m((complex_t*)hadamard_c)
    {
        m = build_matrix(hadamard_c, 2);
    }

    void apply(qu_register &qureg) override {
        size_t qs = qureg.states();
        complex_t *data = qureg.get_data().data();
        // sqg_apply(m,qubit,qureg);
        __apply_h(0, qs, qubit, data, 0, (1UL << qubit), hadamard_c);
        // __apply_m(0, qs, qubit, data, 0, (1 << qubit), hadamard_c);
        //__apply_h_old(0, qs, qubit, data, 0, (1 << qubit), hadamard_c);

        // qureg.set_binary(qubit,__state_unknown__);
        qureg.set_measurement_prediction(qubit, __state_unknown__);
    }

    std::string micro_code() override {
        /**
          | wait 5
          | y90 q0  --> { pulse 12,0,0 }
          | wait 5
          | x180 q0 --> { pulse 9,0,0 }
         */
        if (qubit > 2)
            return "# unsupported operation : qubit out of range";
        std::stringstream uc;
        uc << pulse_lt[qubit][__y90__] << "\n";
        uc << "  wait 4 \n";
        uc << pulse_lt[qubit][__x180__] << "\n";
        uc << "  wait 4 \n";
        return uc.str();
    }

    gate_type_t type() const override { return __hadamard_gate__; }

    void dump() override { println("  [-] hadamard(q=", qubit, ")"); }
};

inline void __swap(cvector_t &amp, size_t size, size_t bit, size_t trg,
                   size_t ctrl, size_t offset = 0) {
    size_t incrementer = 1UL << (bit + 1);

    if ((1UL << bit) == 1) {
        for (size_t i = __bit_set(0, bit), end = (1UL << size); i < end;
             i += incrementer) {
            size_t v = i + offset;
            std::swap(amp[v], amp[__bit_reset(v, trg)]);
        }
    } else {
        for (size_t i = __bit_set(0, bit), end = (1UL << size); i < end;
             i += incrementer) {
            size_t v = i + offset;
            for (size_t j = 0; j < (1UL << bit); j++) {
                // v += j;
                /*
                   #ifdef __SSE__
                   __m128d x = _mm_load_pd((const double *)&p[v]);
                   __m128d y = _mm_load_pd((const double
                   *)&p[__bit_reset(v,trg)]); _mm_store_pd((double
                   *)&p[__bit_reset(v,trg)],x); _mm_store_pd((double *)&p[v],y);
                   #else
                */
                std::swap(amp[v], amp[__bit_reset(v, trg)]);
                ++v;
                // println("swap(",v,",",__bit_reset(v,trg),")");
                // #endif
            }
        }
    }
}

inline int cx_worker(uint64_t cs, uint64_t ce, uint64_t s, cvector_t *p_amp,
                     size_t bit1, size_t bit2, size_t trg, size_t ctrl) {
    cvector_t &amp = *p_amp;
    // xpu::parallel_for fswp(__bit_set(0,b1), (1 << qn), (1 << (b1+1)), &t);
    size_t step = (1UL << (bit1 + 1));
    // size_t b   = cs;
    // size_t e   = ce;
    size_t offset = __bit_set(0, bit1);

    // for (size_t i=__bit_set(0,bit1); i<(1<<size); i += (1 << (bit1+1)))
    //__swap(amp,bit1,bit2,trg,ctrl,i);
    //  for (size_t i=b; i<e; i++)
    //     __swap(amp,bit1,bit2,trg,ctrl,offset+(i*step));
    for (size_t i = cs; i < ce; i++)
        __swap(amp, bit1, bit2, trg, ctrl, offset + (i * step));
    return 0;
}

/**
 * \brief controlled-not gate:
 *
 *    | 1  0  0  0 |
 *    | 0  1  0  0 |
 *    | 0  0  0  1 |
 *    | 0  0  1  1 |
 */
class cnot final : public gate {
private:
    uint64_t control_qubit;
    uint64_t target_qubit;

    cmatrix_t m;

public:
    cnot(uint64_t ctrl_q, uint64_t target_q)
        : control_qubit(ctrl_q), target_qubit(target_q) {
        // m = build_matrix(cnot_c,4); // stack smaching
    }

    // #define CG_HASH_SET
    //#define CG_MATRIX
#ifndef CG_BC
#ifndef CG_MATRIX
#define CG_BC
#endif
#endif // CG_BC

    void apply(qu_register &qreg) override {
        // println("cnot " , control_qubit , "," , target_qubit);
#ifdef CG_MATRIX
        uint64_t sn = qreg.states();
        uint64_t qn = qreg.size();
        uint64_t cq = control_qubit;
        uint64_t tq = target_qubit;

        cmatrix_t i = cidentity_t(sn);
        perm_t p = perms(qn, cq, tq);

        // dump_matrix(i);

        for (perm_t::iterator it = p.begin(); it != p.end(); it++) {
            i(it->first, it->second) = 1;
            i(it->second, it->first) = 1;
            i(it->first, it->first) = 0;
            i(it->second, it->second) = 0;
        }

        // dump_matrix(i);

        qreg = mxv(i, qreg.get_data());

#elif defined(CG_BC)

        uint64_t qn = qreg.size();
        uint64_t cq = control_qubit;
        uint64_t tq = target_qubit;

        cvector_t &amp = qreg.get_data();

        // perms(qn,cq,tq,amp);
        // #if 0
        size_t b1 = std::max(cq, tq);
        size_t b2 = std::min(cq, tq);

        size_t steps =
            ((1UL << qn) - (__bit_set(0, b1))) / (1UL << (b1 + 1)) + 1;
        /*
           println("from=" , (__bit_set(0,b1)));
           println("to=" , (1 , qn));
           println("s=" , (1 , (b1+1)));
           println("steps=" , steps);
         */
        if (qn < 17) {
            fast_cx(amp, qn, b1, b2, tq, cq);
        } else {
#ifdef USE_OPENMP
#pragma omp parallel
#ifndef _MSC_VER
#pragma omp for simd
#endif // _MSC_VER
#endif // USE_OPENMP
            for (size_t i = 0; i < steps; ++i) {
                cx_worker(i, i + 1, 1UL, &amp, b1, b2, (size_t)tq, (size_t)cq);
            }
        }

#elif defined(CG_HASH_SET)

        uint64_t j = control_qubit + 1;
        uint64_t k = target_qubit + 1;

        uint64_t k2 = (1UL << (k - 1));
        uint64_t j2 = (1UL << (j - 1));

        uint64_t r_size = qreg.states();

        std::unordered_set<uint64_t> swap_set;

        // find swap pairs
        for (uint64_t t = 0; t < r_size; t++) {
            if ((t & j2) <= 0)
                continue;

            if (swap_set.find(t - k2) == swap_set.end())
                swap_set.insert(t);
        }

        int64_t t2;
        cvector_t &amp = qreg.get_data();
        complex_t c1(0., 0.), c2(0., 0.);

        for (auto& element: swap_set) {
            t2 = (element + k2 < r_size) ? element + k2 : element - k2;
            c1 = amp(element);
            c2 = amp(t2);
            std::swap(c1, c2);
            amp(element) = c1;
            amp(t2) = c2;
        }
        // qreg=amp;

#endif // CG_HASH_SET

        // if (qreg.get_binary(control_qubit) == __state_1__)
        if (qreg.get_measurement_prediction(control_qubit) == __state_1__)
            qreg.flip_binary(target_qubit);
        // else if (qreg.get_binary(control_qubit) == __state_unknown__)
        else if (qreg.get_measurement_prediction(control_qubit) ==
                 __state_unknown__)
            qreg.set_measurement_prediction(target_qubit, __state_unknown__);
        // qreg.set_binary(target_qubit,__state_unknown__);
    }

    gate_type_t type() const override { return __cnot_gate__; }

    void dump() override {
        println("  [-] cnot(ctrl_qubit=", control_qubit,
                ", target_qubit=", target_qubit, ")");
    }

private:
#if 0
	   void __swap(cvector_t& amp, size_t size, size_t bit, size_t trg, size_t ctrl, size_t offset=0)
	   {
	      // println("bit=" , bit);
	      // println("ctrl=" , ctrl);
	      for (size_t i=__bit_set(0,bit); i<(1UL<<size); i += (1UL << (bit+1)))
		 for (size_t j=0; j<(1<<bit); j++)
		 {
		    size_t v = i+j+offset; 
		    std::swap(amp[v], amp[__bit_reset(v,trg)]);
		    // println(" swap(" , std::bitset<16>(v) , "," , std::bitset<16>(__bit_reset(v,trg)) , ")");
		 }
	   }
#endif

    void fast_cx(cvector_t &amp, size_t size, size_t bit1, size_t bit2,
                 size_t trg, size_t ctrl) {
        /*
           println("from=" , (__bit_set(0,bit1)));
           println("to=" , (1 , size));
           println("s=" , (1 , (bit1+1)));
         */
        for (size_t i = __bit_set(0, bit1); i < (1UL << size);
             i += (1UL << (bit1 + 1)))
            __swap(amp, bit1, bit2, trg, ctrl, i);
    }
};

template <typename T> void swap_if_greater(T &a, T &b) {
    if (a > b) {
        T tmp(a);
        a = b;
        b = tmp;
    }
}

template <typename T> void sort(T &a, T &b, T &c) {
    swap_if_greater(a, b);
    swap_if_greater(a, c);
    swap_if_greater(b, c);
}

/**
 * \brief toffoli gate:
 *
 *    | 1  0  0  0 |
 *    | 0  1  0  0 |
 *    | 0  0  0  1 |
 *    | 0  0  1  1 |
 */
class toffoli final : public gate {
private:
    uint64_t control_qubit_1;
    uint64_t control_qubit_2;
    uint64_t target_qubit;

public:
    toffoli(uint64_t ctrl_q1, uint64_t ctrl_q2, uint64_t target_q)
        : control_qubit_1(ctrl_q1), control_qubit_2(ctrl_q2),
          target_qubit(target_q) {}

    void apply(qu_register &qreg) override {
        uint64_t qn = qreg.size();
        uint64_t cq1 = control_qubit_1;
        uint64_t cq2 = control_qubit_2;
        uint64_t tq = target_qubit;

        cvector_t &amp = qreg.get_data();

        // println("\ntoffoli " ,cq1 ,"," ,cq2 ,"," ,tq);
#if 1
        size_t c1 = cq1;
        size_t c2 = cq2;
        size_t c3 = tq;
        size_t t = tq;
        size_t size = qn;

        sort(c1, c2, c3);
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
        for (int64_t i = __bit_set(__bit_set(__bit_set(0, c1), c2), c3);
             i < (int64_t)(1UL << size); i += (1UL << (c3 + 1)))
            for (size_t j = (size_t)i; j < ((size_t)i + (1UL << c3));
                 j += (1UL << (c2 + 1)))
                for (size_t k = j; k < (j + (1UL << c2));
                     k += (1UL << (c1 + 1)))
                    for (size_t l = k; l < (k + (1UL << (c1))); l++) {
                        std::swap(amp[__bit_set(l, t)], amp[__bit_reset(l, t)]);
                        // println("swap : " , __bit_set(l,t) , "," ,
                        // __bit_reset(l,t));
                    }
#else
        std::vector<uint64_t> done(sn, 0);
        perm_t p = perms(qn, cq1, cq2, tq);

        uint64_t p1, p2;

        for (perm_t::iterator it = p.begin(); it != p.end(); it++) {
            p1 = it->first;
            p2 = it->second;
            if (!(done[p1] || done[p2]))
            // if (!(done[p1]))
            {
                // std::swap(amp(p1),amp(p2)); // ublas
                std::swap(amp[p1], amp[p2]);
                // println("swap : " , p1 , "," , p2);
                done[p1] = 1;
                done[p2] = 1;
            }
        }
#endif

        if ((qreg.get_measurement_prediction(control_qubit_1) == __state_1__) &&
            (qreg.get_measurement_prediction(control_qubit_2) == __state_1__)) {
            qreg.flip_binary(target_qubit);
        } else if ((qreg.get_measurement_prediction(control_qubit_1) ==
                    __state_unknown__) ||
                   (qreg.get_measurement_prediction(control_qubit_2) ==
                    __state_unknown__)) {
            qreg.set_measurement_prediction(target_qubit, __state_unknown__);
            // qreg.set_binary(target_qubit,__state_unknown__);
        }
    }

    gate_type_t type() const override { return __toffoli_gate__; }

    void dump() override {
        println("  [-] toffoli(ctrl_qubit_1=", control_qubit_1,
                ", ctrl_qubit_2=", control_qubit_2,
                ", target_qubit=", target_qubit, ")");
    }
};

int fliper(int cs, int ce, int s, uint64_t q, cvector_t *p_amp) {
    cvector_t &amp = *p_amp;
    for (int i = cs; i < ce; ++i) {
        if (__bit_test(i, q))
            std::swap(amp[i], amp[__bit_flip(i, q)]);
    }
    return 0;
}

#define __swap_xmm(x, y)                                                       \
    {                                                                          \
        x = _mm_xor_pd(x, y);                                                  \
        y = _mm_xor_pd(y, x);                                                  \
        x = _mm_xor_pd(x, y);                                                  \
    }

void fast_flip(uint64_t q, uint64_t n, cvector_t &amp) {
    complex_t *x = amp.data();
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t i = 0; i < (int64_t)(1UL << n); i += (1UL << (q + 1)))
        for (size_t j = (size_t)i; j < ((size_t)i + (1UL << q)); j++)
            //__swap_xmm(x[j].xmm,x[__bit_flip(j,q)].xmm);
            std::swap(x[j].xmm, x[__bit_flip(j, q)].xmm);
}

void flip(uint64_t q, uint64_t n, cvector_t &amp) {
    uint64_t nn = (1UL << n);
    uint64_t p1, p2;
    std::bitset<MAX_QB_N> b;
    // perm_t res;

    b.reset();
    b.set(q);

    uint64_t bc = b.to_ulong();

    while (bc < nn) {
        b.set(q);
        p1 = b.to_ulong();
        b.flip(q);
        p2 = b.to_ulong();
        if (p2 < p1)
            std::swap(amp[p1], amp[p2]);
        b.flip(q);
        b = inc(b);
        b.set(q);
        bc = b.to_ulong();
    }
    // return res;
}

/**
 * \brief  identity :
 *
 *    | 1  0 |
 *    | 0  1 |
 *
 */
class identity final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    identity(uint64_t qubit) : qubit(qubit) { m = build_matrix(identity_c, 2); }

    void apply(qu_register &qreg) override {}

    std::string micro_code() override {
        if (qubit > 2)
            return "# unsupported operation : qubit out of range";
        std::stringstream uc;
        // uc << pulse_lt[qubit][__x180__] << "\n";
        uc << "  wait 4 \n";
        return uc.str();
    }

    void dump() override { println("  [-] identity(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __identity_gate__; }
};

/**
 * \brief  pauli-x :
 *
 *    | 0  1 |
 *    | 1  0 |
 *
 */
class pauli_x final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    pauli_x(uint64_t qubit) : qubit(qubit) { m = build_matrix(pauli_x_c, 2); }

    void apply(qu_register &qreg) override {
        // #define FAST_FLIP
#ifdef FAST_FLIP
        uint64_t qn = qreg.size();
        cvector_t &amp = qreg.get_data();

        // flip(qubit,qn,amp);
        fast_flip(qubit, qn, amp);
        /*
           xpu::task flip_t(fliper,0,0,0,qubit,&amp);
           xpu::parallel_for parallel_flip(0,(1 << qn),1,&flip_t);
           parallel_flip.run();
         */
#else
        uint64_t n = qreg.size();
        complex_t *s = qreg.get_data().data();
        // cm.dump();
        __apply_m(0, (1UL << n), qubit, s, 0, (1UL << qubit), m.m);
        // sqg_apply(m,qubit,qreg);
#endif // FAST_FLIP

        qreg.flip_binary(qubit);
    }

    std::string micro_code() override {
        /**
          | wait 5
          | x180 q0 --> { pulse 9,0,0 }
         */
        if (qubit > 2)
            return "# unsupported operation : qubit out of range";
        std::stringstream uc;
        uc << pulse_lt[qubit][__x180__] << "\n";
        uc << "  wait 4 \n";
        return uc.str();
    }

    void dump() override { println("  [-] pauli-x(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __pauli_x_gate__; }
};

/**
 * \brief  pauli-y :
 *
 *    | 0 -i |
 *    | i  0 |
 */
class pauli_y final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    pauli_y(uint64_t qubit) : qubit(qubit) { m = build_matrix(pauli_y_c, 2); }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
        qreg.flip_binary(qubit);
    }

    std::string micro_code() override {
        /**
          | wait 5
          | x180 q0 --> { pulse 9,0,0 }
         */
        if (qubit > 2)
            return "# unsupported operation : qubit out of range";
        std::stringstream uc;
        uc << pulse_lt[qubit][__y180__] << "\n";
        uc << "  wait 4 \n";
        return uc.str();
    }

    void dump() override { println("  [-] pauli-y(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __pauli_y_gate__; }
};

/**
 * \brief  pauli-z :
 *
 *    | 1  0 |
 *    | 0 -1 |
 */
class pauli_z final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    pauli_z(uint64_t qubit) : qubit(qubit) { m = build_matrix(pauli_z_c, 2); }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
    }

    std::string micro_code() override {
        /**
          | wait 5
          | x180 q0 --> { pulse 9,0,0 }
         */
        if (qubit > 2)
            return "# unsupported operation : qubit out of range";
        std::stringstream uc;
        uc << pulse_lt[qubit][__y180__] << "\n";
        uc << "  wait 4 \n";
        uc << pulse_lt[qubit][__x180__] << "\n";
        uc << "  wait 4 \n";
        return uc.str();
    }

    void dump() override { println("  [-] pauli-z(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __pauli_z_gate__; }
};

/**
 * \brief  phase :
 *
 *    | 1  0 |
 *    | 0  i |
 */
class phase_shift final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    phase_shift(uint64_t qubit) : qubit(qubit) { m = build_matrix(phase_c, 2); }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
    }

    std::string micro_code() override {
        if (qubit > 2)
            return "# unsupported operation : qubit out of range";
        std::stringstream uc;
        uc << pulse_lt[qubit][__y90__] << "\n";
        uc << "  wait 4 \n";
        uc << pulse_lt[qubit][__x90__] << "\n";
        uc << "  wait 4 \n";
        uc << pulse_lt[qubit][__ym90__] << "\n";
        uc << "  wait 4 \n";
        return uc.str();
    }

    void dump() override { println("  [-] phase(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __phase_gate__; }
};

/**
 * \brief S dag gate
 */
class s_dag_gate final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    s_dag_gate(uint64_t qubit) : qubit(qubit) {
        m = build_matrix(sdag_gate_c, 2);
    }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
    }

    void dump() override { println("  [-] s_dag_gate(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __sdag_gate__; }
};

/**
 * \brief T gate
 */
class t_gate final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    t_gate(uint64_t qubit) : qubit(qubit) { m = build_matrix(t_gate_c, 2); }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
    }

    void dump() override { println("  [-] t_gate(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __t_gate__; }
};

/**
 * \brief T dag gate
 */
class t_dag_gate final : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    t_dag_gate(uint64_t qubit) : qubit(qubit) {
        m = build_matrix(tdag_gate_c, 2);
    }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
    }

    void dump() override { println("  [-] t_dag_gate(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __tdag_gate__; }
};

/**
 * phase factoring
 */
void reset_gphase(cmatrix_t &m) {
    double n = m(0, 0).norm();

    if (n > 10e-9) {
        complex_t p(m(0, 0).re / n, m(0, 0).im / n);
        m(0, 0) /= p;
        m(0, 1) /= p;
        m(1, 0) /= p;
        m(1, 1) /= p;
    } else {
        n = m(0, 1).norm();
        complex_t p(m(0, 1).re / n, m(0, 1).im / n);
        m(0, 0) /= p;
        m(0, 1) /= p;
        m(1, 0) /= p;
        m(1, 1) /= p;
    }
    double n1 = std::sqrt(m(0, 0).norm() + m(1, 0).norm());
    double n2 = std::sqrt(m(0, 1).norm() + m(1, 1).norm());
    m(0, 0) /= n1;
    m(0, 1) /= n2;
    m(1, 0) /= n1;
    m(1, 1) /= n2;
}

/**
 *                  | (cos(?/2)       -e(i?)sin(?/2))    |
 * general gate u = |                                    |
 *                  | (e(i?)sin(?/2)   e(i?+i?)cos(?/2)) |
 */
class unitary final : public gate {
private:
    uint64_t qubit;
    double angle[3];
    cmatrix_t m;

public:
    unitary(uint64_t qubit, double angle[3]) : qubit(qubit) {
        // m.resize(2,2);
        m(0, 0) = cos(angle[1] / 2);
        m(0, 1) = complex_t(-cos(angle[2] / 2), -sin(angle[2] / 2)) *
                  sin(angle[1] / 2);
        m(1, 0) =
            complex_t(cos(angle[3] / 2), sin(angle[3] / 2)) * sin(angle[1] / 2);
        m(1, 1) = complex_t(cos((angle[3] / 2) + (angle[2] / 2)),
                            sin((angle[3] / 2) + (angle[2] / 2))) *
                  cos(angle[1] / 2);
    }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
        qreg.set_measurement_prediction(qubit, __state_unknown__);
        // qreg.set_binary(qubit,__state_unknown__);
    }

    double get_angle() { return *angle; }

    void dump() override {
        println("  [-] unitary(qubit=", qubit, ", angle=", angle, ")");
    }

    gate_type_t type() const override { return __unitary_gate__; }
};

/**
 * \brief  rotation-x :
 */
class rx final : public gate {
private:
    uint64_t qubit;
    double angle;
    cmatrix_t m;

public:
    rx(uint64_t qubit, double angle) : qubit(qubit), angle(angle) {
        // m.resize(2,2);
        m(0, 0) = cos(angle / 2);
        m(0, 1) = complex_t(0, -sin(angle / 2));
        m(1, 0) = complex_t(0, -sin(angle / 2));
        m(1, 1) = cos(angle / 2);
        reset_gphase(m);
    }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
        qreg.set_measurement_prediction(qubit, __state_unknown__);
        // qreg.set_binary(qubit,__state_unknown__);
    }

    void dump() override {
        println("  [-] rx(qubit=", qubit, ", angle=", angle, ")");
    }

    gate_type_t type() const override { return __rx_gate__; }
};

/**
 * \brief  rotation-y :
 */
class ry final : public gate {
private:
    uint64_t qubit;
    double angle;
    cmatrix_t m;

public:
    ry(uint64_t qubit, double angle) : qubit(qubit), angle(angle) {
        // m.resize(2,2);
        m(0, 0) = cos(angle / 2);
        m(0, 1) = -sin(angle / 2);
        m(1, 0) = sin(angle / 2);
        m(1, 1) = cos(angle / 2);
        // reset_gphase(m);
    }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
        qreg.set_measurement_prediction(qubit, __state_unknown__);
        // qreg.set_binary(qubit,__state_unknown__);
    }

    void dump() override {
        println("  [-] ry(qubit=", qubit, ", angle=", angle, ")");
    }

    gate_type_t type() const override { return __ry_gate__; }
};

/**
 * \brief  rotation-z :
 */
class rz final : public gate {
private:
    uint64_t qubit;
    double angle;
    cmatrix_t m;

public:
    rz(uint64_t qubit, double angle) : qubit(qubit), angle(angle) {
        // m.resize(2,2);
        m(0, 0) = complex_t(cos(-angle / 2), sin(-angle / 2));
        m(0, 1) = 0;
        m(1, 0) = 0;
        m(1, 1) = complex_t(cos(angle / 2), sin(angle / 2));
        reset_gphase(m);
    }

    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
        qreg.set_measurement_prediction(qubit, __state_unknown__);
        // qreg.set_binary(qubit,__state_unknown__);
    }

    void dump() override {
        println("  [-] rz(qubit=", qubit, ", angle=", angle, ")");
    }

    gate_type_t type() const override { return __rz_gate__; }
};

void __shift(cvector_t &amp, size_t size, size_t bit, complex_t p,
             size_t offset = 0) {
    // println("bit=" , bit);
    // println("ctrl=" , ctrl);
    complex_t *x = amp.data();
    // println(">>>> " , p);
    for (size_t i = __bit_set(0, bit); i < (1UL << size);
         i += (1UL << (bit + 1)))
        for (size_t j = 0; j < (1UL << bit); j++) {
            size_t v = i + j + offset;
            // amp[v] *= p;
            // println(" before mul : " , x[v]);
            x[v] *= p;
            // println(" after mul : " , x[v]);
            // println(" swap(" , std::bitset<16>(v) , "," ,
            // std::bitset<16>(__bit_reset(v,trg)) , ")");
        }
}

void __shift(complex_t *x, size_t size, size_t bit, complex_t p,
             size_t offset = 0) {
    // println("bit=" , bit);
    // println("ctrl=" , ctrl);
    for (size_t i = __bit_set(0, bit); i < (1UL << size);
         i += (1UL << (bit + 1)))
        for (size_t j = 0; j < (1UL << bit); j++) {
            size_t v = i + j + offset;
            // amp[v] *= p;
            x[v] *= p;
            // println(" swap(" , std::bitset<16>(v) , "," ,
            // std::bitset<16>(__bit_reset(v,trg)) , ")");
        }
}

int shift_worker(int cs, int ce, int s, cvector_t *p_amp, size_t bit1,
                 size_t bit2, complex_t p) {
    cvector_t &amp = *p_amp;
    // xpu::parallel_for fswp(__bit_set(0,b1), (1 << qn), (1 << (b1+1)), &t);
    size_t step = (1UL << (bit1 + 1));
    size_t b = cs;
    size_t e = ce;

    size_t offset = __bit_set(0, bit1);

    // for (size_t i=__bit_set(0,bit1); i<(1<<size); i += (1 << (bit1+1)))
    //__swap(amp,bit1,bit2,trg,ctrl,i);
    for (size_t i = b; i < e; i++)
        __shift(amp, bit1, bit2, p, offset + (i * step));
    return 0;
}

uint64_t qft_1st_fold_worker(uint64_t is, uint64_t ie, uint64_t s, uint64_t n,
                             uint64_t qubit, kronecker_ui m, cvector_t *v,
                             cvector_t *res) {
    uint64_t k = n - qubit;
    // println("run : " , is , " .. " , ie);
    complex_t *pv = v->data();
    complex_t *pr = res->data();
    size_t bc = 0, c1 = 0, c2 = 0;

    for (uint64_t r = is; r < ie; ++r) {
        bc = r;
        c1 = __bit_reset(bc, n - k);
        c2 = __bit_set(bc, n - k);
#ifdef __OP_PREFETCH__
        _mm_prefetch((char *)&pv[__bit_reset((bc + 1), n - k)], _MM_HINT_T0);
        _mm_prefetch((char *)&pv[__bit_set((bc + 1), n - k)], _MM_HINT_T0);
#endif         // __OP_PREFETCH__
#ifdef __AVX__ // NO
        xpu::_mm_cmul_add_pd(pv[c1], pv[c2], m.get(r, c1), m.get(r, c2), pr[r]);
#else
        // complex_t s; // = 0;
        // pr[r] = pv[c1]*(m->get(r,c1)) + pv[c2]*(m->get(r,c2));
        pr[r].xmm = _mm_add_pd((pv[c1] * (m.get(r, c1))).xmm,
                               (pv[c2] * (m.get(r, c2))).xmm);
#endif
    }
    size_t bit2 = qubit;
    for (size_t j = qubit + 1; j < n; ++j) {
        complex_t p(cos(QX_PI / (1UL << (j - qubit))),
                    sin(QX_PI / (1UL << (j - qubit))));
        size_t bit1 = j;
        size_t step = (1UL << (bit1 + 1));
        size_t offset = __bit_set(0, bit1);
        for (size_t i = is; i < ie; i++) {
            // println("i=" , i*step);
            __shift(pr, bit1, bit2, p, offset + (i * step));
        }
    }

    return 0;
}

void qft_1st_fold(uint64_t n, uint64_t qubit, kronecker_ui m, cvector_t &v,
                  cvector_t &res) {
    uint64_t k = n - qubit;
    uint64_t rows = (1UL << n);
    uint64_t z = 0;

    // xpu::task qf_t(qft_fold_worker,0,0,0,n,qubit,m,&v,&res);
    // xpu::parallel_for process(z,rows,1,&qf_t);
    // process.run();

    static const uint64_t SIZE = 1000;
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t batch = 0; batch <= rows / SIZE; batch++) {
        qft_1st_fold_worker(batch * SIZE,
                            std::min<uint64_t>((batch + 1) * SIZE, rows), 1, n,
                            qubit, m, &v, &res);
    }
}

uint64_t qft_nth_fold_worker(uint64_t is, uint64_t ie, uint64_t s, uint64_t n,
                             uint64_t qubit, kronecker_iui m, cvector_t *v,
                             cvector_t *res) {
    uint64_t k = n - qubit;
    // println("run : " , is , " .. " , ie);
    complex_t *pv = v->data();
    complex_t *pr = res->data();
    size_t bc, c1, c2;

    for (uint64_t r = is; r < ie; ++r) {
        bc = r;
        c1 = __bit_reset(bc, n - k);
        c2 = __bit_set(bc, n - k);
#ifdef __OP_PREFETCH__
        _mm_prefetch((char *)&pv[__bit_reset((bc + 1), n - k)], _MM_HINT_T0);
        _mm_prefetch((char *)&pv[__bit_set((bc + 1), n - k)], _MM_HINT_T0);
#endif         // __OP_PREFETCH__
#ifdef __AVX__ // NO
        xpu::_mm_cmul_add_pd(pv[c1], pv[c2], m.get(r, c1), m.get(r, c2), pr[r]);
#else
        // complex_t s; // = 0;
        // pr[r] = pv[c1]*(m->get(r,c1)) + pv[c2]*(m->get(r,c2));
        pr[r].xmm = _mm_add_pd((pv[c1] * (m.get(r, c1))).xmm,
                               (pv[c2] * (m.get(r, c2))).xmm);
#endif
    }
    size_t bit2 = qubit;
    for (size_t j = qubit + 1; j < n; ++j) {
        complex_t p(cos(QX_PI / (1UL << (j - qubit))),
                    sin(QX_PI / (1UL << (j - qubit))));
        size_t bit1 = j;
        size_t step = (1UL << (bit1 + 1));
        size_t offset = __bit_set(0, bit1);
        for (size_t i = is; i < ie; i++) {
            __shift(pr, bit1, bit2, p, offset + (i * step));
        }
    }

    return 0;
}

void qft_nth_fold(uint64_t n, uint64_t qubit, kronecker_iui m, cvector_t &v,
                  cvector_t &res) {
    uint64_t k = n - qubit;
    uint64_t rows = (1UL << n);
    uint64_t z = 0;

    // xpu::task qf_t(qft_fold_worker,0,0,0,n,qubit,m,&v,&res);
    // xpu::parallel_for process(z,rows,1,&qf_t);
    // process.run();

    static const uint64_t SIZE = 1000;
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t batch = 0; batch <= rows / SIZE; batch++) {
        qft_nth_fold_worker(batch * SIZE,
                            std::min<uint64_t>((batch + 1) * SIZE, rows), 1, n,
                            qubit, m, &v, &res);
    }
}

int qft_worker(int cs, int ce, int s, size_t n, cvector_t &p_in,
               cvector_t &p_out, kronecker_ui kr, size_t qubit) {
    cvector_t &amp = p_out;
    // xpu::parallel_for fswp(__bit_set(0,b1), (1 << qn), (1 << (b1+1)), &t);
    size_t b = cs;
    size_t e = ce;

    rw_process_ui(cs, ce, s, n, qubit, kr, &p_in, &p_out); // H

    size_t bit2 = qubit;
    for (size_t j = qubit + 1; j < n; ++j) {
        complex_t p(cos(QX_PI / (1UL << (j - qubit))),
                    sin(QX_PI / (1UL << (j - qubit))));
        size_t bit1 = j;
        size_t step = (1UL << (bit1 + 1));
        size_t offset = __bit_set(0, bit1);
        for (size_t i = b; i < e; i++) {
            println("i=", i * step);
            __shift(amp, bit1, bit2, p, offset + (i * step));
        }
    }
    return 0;
}

int qft_worker(int cs, int ce, int s, size_t n, cvector_t &p_in,
               cvector_t &p_out, kronecker_iui kr, size_t qubit) {
    // xpu::parallel_for fswp(__bit_set(0,b1), (1 << qn), (1 << (b1+1)), &t);
    size_t b = cs;
    size_t e = ce;
    rw_process_iui(cs, ce, s, n, qubit, kr, &p_in, &p_out); // H
    return 0;
    size_t bit2 = qubit;
    for (size_t j = qubit + 1; j < n; ++j) {
        complex_t p(cos(QX_PI / (1UL << (j - qubit))),
                    sin(QX_PI / (1UL << (j - qubit))));
        size_t bit1 = j;
        size_t step = (1UL << (bit1 + 1));
        size_t offset = __bit_set(0, bit1);
        for (size_t i = b; i < e; i++) {
            __shift(p_out, bit1, bit2, p, offset + (i * step));
        }
    }
    return 0;
}

/**
 * \brief qft
 */
class qft final : public gate {
private:
    std::vector<uint64_t> qubit;
    cmatrix_t hm;

public:
    qft(std::vector<uint64_t> qubit) : qubit(qubit) {
        hm = build_matrix(hadamard_c, 2);
    }

    void apply(qu_register &qreg) override {
        size_t n = qreg.size();
        size_t s = qreg.states();
        cvector_t &in = qreg.get_data();
        cvector_t &out = qreg.get_aux();

        // kronecker_ui kui(hm,2,(1 << (n-1)));
        kronecker_ui kui(hadamard_c, 2, (1 << (n - 1)));
        qft_1st_fold(n, 0, kui, in, out);
        for (size_t i = 1; i < n - 1; ++i) {
            size_t q = qubit[i];
            // kronecker_iui kiui(hm, 2, (1 << (n-q-1)), (1 << (q)));
            kronecker_iui kiui(hadamard_c, 2, (1UL << (n - q - 1)),
                               (1UL << (q)));
            qft_nth_fold(n, 0, kiui, in, out);
        }
        in.swap(out);
        return;
#if 0
            // 1st fold
            qft_worker(0, s, 1, n, in, out, kronecker_ui(m,2,s-2), 0);
            return 0;
            // ith fold
            for (size_t i=1; i<qubit.size(); ++i)
            {
               size_t q = qubit[i];
               kronecker_iui k(m, 2, (1UL << (n-q-1)), (1UL << (q)));
               qft_worker(0, qreg.states(), 1, qreg.size(), (qreg.get_data()), (qreg.get_aux()), k, q);
            }
            // last fold
            kronecker_iu k(m,2,(1UL << (n-1)));
            sparse_mulmv(n,qubit[n-1],k,qreg.get_data(),qreg.get_aux());
            in.swap(out);
            return;
#endif
    }

    void dump() override {
        print("  [-] qft(");
        for (size_t i = 0; i < (qubit.size() - 1); ++i)
            print("q", qubit[i], ",");
        println("q", qubit[qubit.size() - 1], ")");
    }

    gate_type_t type() const override { return __qft_gate__; }
};

/**
 * phase shifter
 */
void __apply_cm(complex_t *state, complex_t m[2][2], std::size_t i11,
                std::size_t i12, std::size_t i13, std::size_t i21,
                std::size_t i22, std::size_t i23, std::size_t i31,
                std::size_t i32, std::size_t ish) {
    complex_t m00 = m[0][0], m01 = m[0][1], m10 = m[1][0], m11 = m[1][1];

    for (std::size_t r1 = i11; r1 < i12; r1 += i13) {
#ifdef USE_OPENMP
// #pragma omp parallel for
#endif
        for (std::size_t r2 = r1 + i21; r2 < r1 + i22; r2 += i23) {
            for (std::size_t ind0 = r2 + i31; ind0 < r2 + i32; ind0++) {
                std::size_t ind1 = ind0 + ish;
                complex_t in0 = state[ind0], in1 = state[ind1];

                state[ind0] = m00 * in0 + m01 * in1;
                state[ind1] = m10 * in0 + m11 * in1;
            }
        }
    }
}

/**
 * \brief  controlled phase shift by arbitrary phase angle or
 * (2*pi/(2^(k=ctrl-target)))
 */
class ctrl_phase_shift final : public gate {
private:
    uint64_t ctrl_qubit;
    uint64_t target_qubit;
    complex_t z;
    complex_t m[2][2];
    double phase;

protected:
    void build_operator() {
        m[0][0] = complex_t(cos(-phase / 2), sin(-phase / 2));
        m[0][1] = 0.0;
        m[1][0] = 0.0;
        m[1][1] = complex_t(cos(phase / 2), sin(phase / 2));

        double n = m[0][0].norm();
        if (n > 10e-9) {
            complex_t p(m[0][0].re / n, m[0][0].im / n);
            m[0][0] /= p;
            m[0][1] /= p;
            m[1][0] /= p;
            m[1][1] /= p;
        } else {
            n = m[0][1].norm();
            complex_t p(m[0][0].re / n, m[0][0].im / n);
            m[0][0] /= p;
            m[0][1] /= p;
            m[1][0] /= p;
            m[1][1] /= p;
        }
    }

public:
    /**
     * ctor (q)
     */
    ctrl_phase_shift(uint64_t ctrl_qubit, uint64_t target_qubit)
        : ctrl_qubit(ctrl_qubit), target_qubit(target_qubit), z(0.0, 0.0) {
        phase = 2 * QX_PI / (1UL << (ctrl_qubit - target_qubit));
        build_operator();
    }

    /**
     * ctor (k)
     */
    ctrl_phase_shift(uint64_t ctrl_qubit, uint64_t target_qubit, size_t k)
        : ctrl_qubit(ctrl_qubit), target_qubit(target_qubit) {
        phase = 2 * QX_PI / (1UL << k);
        build_operator();
    }

    /**
     * ctor (p)
     */
    ctrl_phase_shift(uint64_t ctrl_qubit, uint64_t target_qubit, double angle)
        : ctrl_qubit(ctrl_qubit), target_qubit(target_qubit), phase(angle) {
        build_operator();
    }

    void apply(qu_register &qreg) override {
        uint64_t n = qreg.size();
        size_t c = ctrl_qubit;
        size_t t = target_qubit;

        if (c > t)
            __apply_cm(qreg.get_data().data(), m, 0UL, (1UL << n),
                       1UL << (c + 1l), 1UL << c, 1UL << (c + 1UL),
                       1UL << (t + 1UL), 0UL, 1UL << t, 1UL << t);
        else
            __apply_cm(qreg.get_data().data(), m, 0UL, (1UL << n),
                       1UL << (t + 1UL), 0UL, 1UL << t, 1UL << (c + 1l),
                       1UL << c, 1UL << (c + 1UL), 1UL << t);
    }

    void dump() override {
        println("  [-] ctrl_phase_shift(ctrl_qubit=", ctrl_qubit,
                ", target_qubit: ", target_qubit, ", phase = (", z.re, ", i.",
                z.im, ") )");
    }

    gate_type_t type() const override { return __ctrl_phase_shift_gate__; }
};

/**
 * \brief  swap :
 *
 *    | 1  0  0  0 |
 *    | 0  0  1  0 |
 *    | 0  1  0  0 |
 *    | 0  0  0  1 |
 */
class swap final : public gate {
private:
    uint64_t qubit1;
    uint64_t qubit2;

    // cmatrix_t m;

public:
    swap(uint64_t qubit1, uint64_t qubit2) : qubit1(qubit1), qubit2(qubit2) {
        // m =  build_matrix(swap_c,4);
    }

    void apply(qu_register &qreg) override {
        cnot(qubit1, qubit2).apply(qreg);
        cnot(qubit2, qubit1).apply(qreg);
        cnot(qubit1, qubit2).apply(qreg);
    }

    void dump() override {
        println("  [-] swap(q1=", qubit1, ", q2=", qubit2, ")");
    }

    gate_type_t type() const override { return __swap_gate__; }
};

/**
 * \brief cphase
 */
class cphase final : public gate {
private:
    uint64_t ctrl_qubit;
    uint64_t target_qubit;

public:
    cphase(uint64_t ctrl_qubit, uint64_t target_qubit)
        : ctrl_qubit(ctrl_qubit), target_qubit(target_qubit) {}

    void apply(qu_register &qreg) override {
        hadamard(target_qubit).apply(qreg);
        cnot(ctrl_qubit, target_qubit).apply(qreg);
        hadamard(target_qubit).apply(qreg);
    }

    void dump() override {
        println("  [-] cphase(ctrl_qubit=", ctrl_qubit,
                ", target_qubit=", target_qubit, ")");
    }

    gate_type_t type() const override { return __cphase_gate__; }
};

/**
 * \brief  custom matrix gate
 *
 */
class custom : public gate {
private:
    uint64_t qubit;
    cmatrix_t m;

public:
    // #ifdef __BUILTIN_LINALG__
    //	   custom(std::vector<uint64_t>  qubits, qx::linalg::matrix<complex_t>
    // m) : qubits(qubits), m(m)
    // #else
    custom(uint64_t qubit, cmatrix_t m)
        : qubit(qubit), m(m)
    // #endif
    {
        //		 uint64_t size = 1 << qubits.size();
        //		 if (size != m.size1() || size != m.size2())
        //		    println("[x] error: cutom gate : the matrix size do
        // not match the number of qubits !");
        // verify also that the matrix is unitary
        // #ifdef __BUILTIN_LINALG__
        // cmatrix_t ctr(m.size2(),m.size1());
        // qx::linalg::matrix<complex_t> ctr(m.size2(),m.size1());
        // for (uint64_t i=0; i<m.size2(); ++i)
        //    for (uint64_t j=0; j<m.size1(); ++j)
        //       ctr(i,j) = m(j,i).conj();
        // // cmatrix_t mxctr = mxm(m,ctr);
        // qx::linalg::matrix<complex_t> mxctr = mxm(m,ctr);
        // qx::linalg::identity_matrix<complex_t> id(m.size1());
        // #else
        // 		 cmatrix_t mxctr = mxm(m,ublas::trans(conj(m)));
        // 		 ublas::identity_matrix<complex_t> id(m.size1());
        // #endif

        // #ifdef __BUILTIN_LINALG__
        //		 if (qx::linalg::equals(mxctr,id))
        // #else
        // 		 if (equals(mxctr,id))
        // #endif
        //		    println("[x] error: custom gate : the specified
        // matrix is not unitary !");
    }

    /**
     * apply
     */
    void apply(qu_register &qreg) override {
        sqg_apply(m, qubit, qreg);
        qreg.set_measurement_prediction(qubit, __state_unknown__);
    }

    /**
     * dump
     */
    void dump() override {
        println("  [-] custom matrix on qubit ", qubit);
        // println("  [-] custom(qubits=" , qubits , ", matrix=" , m , ")");
    }

    /**
     * type
     */
    gate_type_t type() const override { return __custom_gate__; }
};

double p1_worker(uint64_t cs, uint64_t ce, uint64_t qubit, cvector_t *p_data) {
    cvector_t &data = *p_data;
    double local_p1 = 0;
    uint64_t ref = 1UL << qubit;
    uint64_t offset = 0;

    // We need to calculate the "offset_start" in order to maintain the
    // correctness of the index calculation in the parallel region
    uint64_t reminder = cs % ref;
    uint64_t factor = std::floor((cs - reminder) / ref);
    uint64_t offset_start = factor * ref;

    offset = offset_start;

    /* *******************************************************************************
     */
    // The following for-loop is a decimal-based representation of the identical
    // binary- based for-loop: uint64_t size = qreg.size(); uint64_t n = (1 <<
    // size);
    //  std::bitset<MAX_QB_N> b;
    //  b.reset();
    //  b.set(qubit);
    //  for (uint64_t i = b.to_ulong(); i < n; i=b.to_ulong()) {
    //        p += data[i].norm();
    //        b = inc(b);
    //        b.set(qubit);
    //  }
    /* *******************************************************************************
     */
    for (uint64_t i = cs; i < ce; ++i) {
        if (!(i % ref))
            offset = ref + i;
        local_p1 += data[i + offset].norm();
    }

    return local_p1;
}

inline double zero_worker_norm(uint64_t cs, uint64_t ce, cvector_t *p_data) {
    uint64_t num_elts = ce - cs;
    uint64_t tile_size = std::min<uint64_t>(num_elts, 32UL);
    complex_t *vd = p_data->data();
    double local_length = 0.;

#if defined(__AVX__)
    __m256d sum = _mm256_set1_pd(0.0);
    for (uint64_t i = cs; i < ce; i += tile_size) {
        for (uint64_t j = i, end = std::min(ce, tile_size + i); j < end;
             j += 2) {
            double *pvd = (double *)&vd[j];
            sum = _mm256_add_pd(
                sum, _mm256_mul_pd(_mm256_load_pd(pvd), _mm256_load_pd(pvd)));
        }
    }
    __m256d r2 = _mm256_hadd_pd(sum, sum);
    local_length = _mm_cvtsd_f64(
        _mm_add_pd(_mm256_extractf128_pd(r2, 1), _mm256_castpd256_pd128(r2)));
#elif defined(__SSE__)
    __m128d sum = _mm_set1_pd(0.0);
    for (uint64_t i = cs; i < ce; i += tile_size) {
        for (uint64_t j = i, end = std::min(ce, tile_size + i); j < end; ++j) {
            double *pvd = (double *)&vd[j];
            sum =
                _mm_add_pd(sum, _mm_mul_pd(_mm_load_pd(pvd), _mm_load_pd(pvd)));
        }
    }
    local_length = _mm_cvtsd_f64(_mm_hadd_pd(sum, sum));
#else
    for (uint64_t i = cs; i < ce; i += tile_size) {
        for (uint64_t j = i, end = std::min(ce, tile_size + i); j < end;
             j += 2) {
            local_length += data[j].norm() + data[j + 1].norm();
        }
    }
#endif
    return local_length;
}

inline double zero_worker_true(uint64_t cs, uint64_t ce, uint64_t s,
                               /*double * length,*/ uint64_t qubit,
                               /*xpu::lockable * l, */ cvector_t *p_data) {
    cvector_t &data = *p_data;
    uint64_t pos = 1UL << qubit;

    for (uint64_t i = cs; i < ce; i += 2) {
        // ((x) & (1<<(pos)))
        if (i & pos)
            data[i] = 0.0;
        if ((i + 1) & pos)
            data[i + 1] = 0.0;
    }

    return zero_worker_norm(cs, ce, p_data);
}

inline double zero_worker_false(uint64_t cs, uint64_t ce, uint64_t s,
                                /*double * length,*/ uint64_t qubit,
                                /*xpu::lockable * l, */ cvector_t *p_data) {
    cvector_t &data = *p_data;
    uint64_t pos = 1UL << qubit;

    for (uint64_t i = cs; i < ce; i += 2) {
        // ((x) & (1<<(pos)))
        if (!(i & pos)) {
            data[i] = 0.0;
        }
        if (!((i + 1) & pos))
            data[i + 1] = 0.0;
    }

    return zero_worker_norm(cs, ce, p_data);
}

int renorm_worker(uint64_t cs, uint64_t ce, uint64_t s, double *length,
                  cvector_t *p_data) {
    double l = *length;
    double l_rec = 1. / l;
    uint64_t num_elts = ce - cs;
    uint64_t tile_size = std::min<uint64_t>(num_elts, 16UL);
    complex_t *vd = p_data->data();

#ifdef __AVX512F__
    __m512d vl = _mm512_set1_pd(l_rec);
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (uint64_t i = cs; i < ce; i += tile_size) {
        for (uint64_t j = i, end = std::min(ce, tile_size + i); j < end;
             j += 4) {
            double *pvd = (double *)&vd[j];
            _mm512_store_pd(pvd, _mm512_mul_pd(_mm512_load_pd(pvd), vl));
        }
    }
#elif defined(__AVX__)
    __m256d vl = _mm256_set1_pd(l_rec);
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t i = cs; i < (int64_t)ce; i += tile_size) {
        for (uint64_t j = (uint64_t)i,
                      end = std::min(ce, tile_size + (uint64_t)i);
             j < end; j += 2) {
            auto *pvd = (double *)&vd[j];
            _mm256_store_pd(pvd, _mm256_mul_pd(_mm256_load_pd(pvd), vl));
        }
    }
#elif defined(__SSE__)
    __m128d vl = _mm_set1_pd(l_rec);
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t i = cs; i < (int64_t)ce; i += tile_size) {
        for (uint64_t j = (uint64_t)i,
                      end = std::min(ce, tile_size + (uint64_t)i);
             j < end; ++j) {
            double *pvd = (double *)&vd[j];
            _mm_store_pd(pvd, _mm_mul_pd(_mm_load_pd(pvd), vl));
        }
    }
#else
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t i = cs; i < (int64_t)ce; i += tile_size) {
        for (uint64_t j = (uint64_t)i,
                      end = std::min(ce, tile_size + (uint64_t)i);
             j < end; ++j) {
            data[j] *= l_rec;
        }
    }
#endif

    // // Update the remaining elements if there are any
    // uint64_t reminder = num_elts % tile_size;
    // if (reminder) {
    //    for (uint64_t i=ce-reminder; i<ce; ++i)
    //    {
    //       data[i] *= l_rec;
    //    }
    // }

    return 0;
}

/**
 * measure
 */

class measure final : public gate {
private:
    uint64_t qubit;

    static bool
    apply_single(uint64_t qubit, qu_register &qreg) {
        double f = qreg.rand();
        double p = 0;
        bool value = false;
        uint64_t size = qreg.size();
        uint64_t n = (1UL << size);
        cvector_t &data = qreg.get_data();
        double length = 0;

        // Basically, this "if" operator determines what to do if we have more
        // than 64 qubits. It also determines whether to invoke parallel or
        // sequential computations. As of now, we set parallel execution as the
        // default one.
        if (1) // size > 64
        {
            static const uint64_t SIZE = 1000;

            uint64_t range = (n >> 1);
#ifdef USE_OPENMP
#pragma omp parallel for reduction(+ : p)
#endif
            for (int64_t batch = 0; batch <= (int64_t)range / SIZE; batch++) {
                p += p1_worker(batch * SIZE,
                               std::min<uint64_t>((batch + 1) * SIZE, range),
                               qubit, &data);
            }

            value = (f < p);

#ifdef USE_OPENMP
#pragma omp parallel
#endif
            if (value) {
#ifdef USE_OPENMP
#pragma omp for reduction(+ : length)
#endif
                for (int64_t batch = 0; batch <= (int64_t)n / SIZE;
                        batch++) {
                    length += zero_worker_false(
                        batch * SIZE,
                        std::min<uint64_t>((batch + 1) * SIZE, n),
                        (uint64_t)1, qubit, &data);
                }
            } else {
#ifdef USE_OPENMP
#pragma omp for reduction(+ : length)
#endif
                for (int64_t batch = 0; batch <= (int64_t)n / SIZE;
                        batch++) {
                    length += zero_worker_true(
                        batch * SIZE,
                        std::min<uint64_t>((batch + 1) * SIZE, n),
                        (uint64_t)1, qubit, &data);
                }
            }

            length = std::sqrt(length);

            renorm_worker((uint64_t)0, n, (uint64_t)1, &length, &data);
        } else {
            std::bitset<MAX_QB_N> b;
            b.reset();
            b.set(qubit);
            uint64_t bc = b.to_ulong();

            while (bc < n) {
                p += data[bc].norm();
                b = inc(b);
                b.set(qubit);
                bc = b.to_ulong();
            }

            value = (f < p);

            if (value)
            {          // reset all states where the qubit is 0
                for (uint64_t i = 0; i < (1UL << size); ++i) {
                    if (!__bit_test(i, qubit)) {
                        data[i] = 0.0;
                    }
                }
            } else {
                for (uint64_t i = 0; i < (1UL << size); ++i) {
                    if (__bit_test(i, qubit)) {
                        data[i] = 0.0;
                    }
                }
            }

            for (uint64_t k = 0; k < (1UL << size); k++) {
                length += data[k].norm(); // std::norm(data[k]);
            }

            length = std::sqrt(length);

            for (uint64_t k = 0; k < (1UL << size); k++) {
                data[k] /= length;
            }
        }

        qreg.set_measurement_prediction(
            qubit, (value ? __state_1__ : __state_0__));
        qreg.set_measurement(qubit, value);

        return value;
    }

public:
    measure(uint64_t qubit)
        : qubit(qubit) {}

    void apply(qu_register &qreg) override {
        apply_single(qubit, qreg);
    }

    void dump() override {
        println("  [-] measure(qubit=", qubit, ")");
    }

    gate_type_t type() const override {
        return __measure_gate__;
    }

    uint64_t measured_qubit() const {
        return qubit;
    }
};

class measure_all final : public gate {
public:
    measure_all() = default;

    std::bitset<MAX_QB_N> apply_and_get_result(qu_register &qreg) {
        std::bitset<MAX_QB_N> b;
        std::bitset<MAX_QB_N> measured_state;
        bool has_found_measured_state = false;

        double p = 0;
        double f = qreg.rand();
        auto &data = qreg.get_data();
        auto bc = b.to_ulong();

        while (bc < (1UL << qreg.size())) {
            p += data[bc].norm();
            if (!has_found_measured_state && f < p) {
                measured_state = b;
                has_found_measured_state = true;
                data[bc] = data[bc] / std::sqrt(data[bc].norm());
            } else {
                data[bc] = 0;
            }
            b = inc(b);
            bc = b.to_ulong();
        }

        assert(has_found_measured_state);
        qreg.set_measurement(measured_state);

        return measured_state;
    }

    void apply(qu_register &qreg) override {
        apply_and_get_result(qreg);
    }

    void dump() override {
        println("  [-] measure(register)");
    }

    gate_type_t type() const override {
        return __measure_reg_gate__;
    }
};

/**
 * measure_x
 */
class measure_x final : public gate {
private:
    uint64_t qubit;

    qx::hadamard hg;
    qx::measure mg;

public:
    measure_x(uint64_t qubit) : qubit(qubit), hg(qubit), mg(qubit) {}

    measure_x()
        : qubit(0), hg(qubit), mg(qubit) {}

    void apply(qu_register &qreg) override {
        hg.apply(qreg);
        mg.apply(qreg);
        hg.apply(qreg);
    }

    void dump() override {
        println("  [-] measure_x(qubit=", qubit, ")");
    }

    gate_type_t type() const override {
        return __measure_x_gate__;
    }
};

/**
 * measure_y
 */
class measure_y final : public gate {
private:
    uint64_t qubit;

    qx::phase_shift sg;
    qx::pauli_z zg;
    qx::measure_x mg;

public:
    measure_y(uint64_t qubit) : qubit(qubit), sg(qubit), zg(qubit), mg(qubit) {}

    measure_y()
        : qubit(0), sg(qubit), zg(qubit), mg() {}

    void apply(qu_register &qreg) override {
        sg.apply(qreg);
        zg.apply(qreg);
        mg.apply(qreg);
        sg.apply(qreg);
    }

    void dump() override {
        println("  [-] measure_y(qubit=", qubit, ")");
    }

    gate_type_t type() const override {
        return __measure_y_gate__;
    }
};

/**
 * \brief   generic binary controlled gate
 */
class bin_ctrl final : public gate {
private:
    // uint64_t bit;
    std::vector<size_t> bits{};
    std::shared_ptr<gate> g;

public:
    bin_ctrl(size_t bit, std::shared_ptr<gate> g) : g(std::move(g)) {
        bits.push_back(bit);
    }

    bin_ctrl(std::vector<size_t> bit, std::shared_ptr<gate> g)
        : g(std::move(g)) {
        for (auto b : bit)
            bits.push_back(b);
    }

    void apply(qu_register &qreg) override {
        bool m = true;
        for (auto b : bits) {
            if (!qreg.test(b)) {
                m = false;
                break;
            }
        }
        if (m) {
            g->apply(qreg);
        }
    }

    std::shared_ptr<gate> get_gate() { return g; }

    std::vector<size_t> get_bits() { return bits; }

    void dump() override {
        print("  [-] bin_ctrl: \n bit=", bits[0], " -> ");
        g->dump();
    }

    gate_type_t type() const override { return __bin_ctrl_gate__; }
};

#define bin_ctrl_pauli_x(b, q) bin_ctrl(b, std::make_shared<pauli_x>(q))
#define bin_ctrl_pauli_y(b, q) bin_ctrl(b, std::make_shared<pauli_y>(q))
#define bin_ctrl_pauli_z(b, q) bin_ctrl(b, std::make_shared<pauli_z>(q))

/**
 * \brief classical binary not gate
 */
class classical_not final : public gate {
private:
    uint64_t bit;

public:
    classical_not(uint64_t bit) : bit(bit) {}

    void apply(qu_register &qreg) override {
        qreg.flip_measurement(bit);
    }

    uint64_t get_bit() { return bit; }

    void dump() override {
        // println("  [-] classical not gate: \n bit=" , bit);
        println("  [-] not ", bit);
    }

    gate_type_t type() const override { return __classical_not_gate__; }
};

/**
 * prepz
 */
class prepz final : public gate {
private:
    uint64_t qubit;

public:
    prepz(uint64_t qubit) : qubit(qubit) {}

    void apply(qu_register &qreg) override {
        measure(qubit).apply(qreg);
        bin_ctrl_pauli_x(qubit, qubit).apply(qreg);
        // bin_ctrl_pauli_z(qubit,qubit).apply(qreg);
        qreg.set_measurement(qubit, false);
    }

    void dump() override { println("  [-] prepz(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __prepz_gate__; }
};

/**
 * prepx
 */
class prepx final : public gate {
private:
    uint64_t qubit;
    hadamard h;

public:
    prepx(uint64_t qubit) : qubit(qubit), h(qubit) {}

    void apply(qu_register &qreg) override {
        h.apply(qreg);
        measure(qubit).apply(qreg);
        h.apply(qreg);
        bin_ctrl_pauli_z(qubit, qubit).apply(qreg);
        qreg.set_measurement(qubit, false);
    }

    void dump() override { println("  [-] prepx(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __prepx_gate__; }
};

/**
 * prepy
 */
class prepy final : public gate {
private:
    uint64_t qubit;
    prepx px;
    phase_shift s;

public:
    prepy(uint64_t qubit) : qubit(qubit), px(qubit), s(qubit) {}

    void apply(qu_register &qreg) override {
        px.apply(qreg);
        s.apply(qreg);
        qreg.set_measurement(qubit, false);
    }

    void dump() override { println("  [-] prepy(qubit=", qubit, ")"); }

    gate_type_t type() const override { return __prepy_gate__; }
};

class lookup_gate_table final : public gate {
private:
    std::vector<uint64_t> ctrl_bits;
    std::map<uint64_t, std::shared_ptr<gate>> gates;

public:
    lookup_gate_table(uint64_t b0) { ctrl_bits.push_back(b0); }

    lookup_gate_table(uint64_t b0, uint64_t b1) {
        ctrl_bits.push_back(b0);
        ctrl_bits.push_back(b1);
    }

    lookup_gate_table(uint64_t b0, uint64_t b1, uint64_t b2) {
        ctrl_bits.push_back(b0);
        ctrl_bits.push_back(b1);
        ctrl_bits.push_back(b2);
    }

    lookup_gate_table(std::vector<uint64_t> ctrl_bits) : ctrl_bits(ctrl_bits) {}

    void add_gate(uint64_t cond, std::shared_ptr<gate> g) {
        assert(cond < (1 << ctrl_bits.size()));
        gates[cond] = g;
    }

    void apply(qu_register &qreg) override {
        uint64_t k = 0;

        for (uint64_t i = 0; i < ctrl_bits.size(); i++) {
            // println(qreg.get_binary(i));
            if (qreg.test(ctrl_bits[i]))
                k = k * 2 + 1;
            else
                k *= 2;
        }

        // println("[+] lookup table : cond = " , k);

        auto it = gates.find(k);

        if (it != gates.end())
            (*it).second->apply(qreg);
    }

    void dump() override { println("  [-] lookup gate table : "); }

    gate_type_t type() const override { return __lookup_table__; }
};

/**
 * \brief display : debug utility
 *    display intermediate quantum states of a
 *   quantum register whithin a circuit.
 */
class display final : public gate {
private:
    bool only_binary;

public:
    display(bool only_binary = false) : only_binary(only_binary) {}

    void apply(qu_register &qreg) override {
        qreg.dump(only_binary);
    }

    void dump() override {
        println("  [-] display(only_binary=", only_binary, ")");
    }

    gate_type_t type() const override {
        if (only_binary)
            return __display_binary__;
        else
            return __display__;
    }
};

/**
 * parallel gates
 */
class parallel_gates final : public gate {
public:
    parallel_gates() = default;

    void apply(qu_register &qreg) override {
        for (uint64_t i = 0; i < gates.size(); i++)
            gates[i]->apply(qreg);
    }

    uint64_t add(std::shared_ptr<gate> gate) {
        gates.push_back(std::move(gate));
        return gates.size();
    }

    bool has(std::function<bool(gate const *)> predicate) {
        for (auto const &gate : gates) {
            if (predicate(gate.get())) {
                return true;
            }
        }
        return false;
    }

    void dump() override {
        println("  [-] parallel_gates (", gates.size(), " gates) : ");
        for (uint64_t i = 0; i < gates.size(); i++)
            gates[i]->dump();
    }

    gate_type_t type() const override { return __parallel_gate__; }

private:
    std::vector<std::shared_ptr<gate>> gates; // list of the parallel gates
};

/**
 * prepare the qubits into an arbitrary quantum state
 */
class prepare final : public gate {
private:
    quantum_state_t *state;

public:
    prepare(quantum_state_t *state) : state(state) {}

    void apply(qu_register &qreg) override {
        qreg.reset();
        cvector_t &q = qreg.get_data();
        double norm = 0;

        for (quantum_state_t::iterator i = state->begin(); i != state->end();
             ++i) {
            basis_state_t bs = (*i).first;
            complex_t c = (*i).second;
            // println("bs=" , bs , ", a=" , c);
            q[bs] = c;
            norm += c.norm(); // std::norm(c);
        }

        if (std::fabs(norm - 1) > QUBIT_ERROR_THRESHOLD) {
            println("[!] warning : the loaded quantum state is not normalized "
                    "(norm = ",
                    norm, ") !");
            println("[!] renormalizing the quantum state...");
            qreg.normalize();
            println("[!] quantum state renormalized successfully.");
        }

        for (size_t qi = 0; qi < qreg.size(); ++qi) {
            qreg.set_measurement_prediction(qi, __state_unknown__);
            // qreg.set_binary(qi,__state_unknown__);
        }
    }

    void dump() override {
        println("  [-] prepare (quantum_state=", state, ")");
    }

    gate_type_t type() const override { return __prepare_gate__; }
};

/**
 * \brief print  : debug utility
 *     print arbitrary string
 */
class print_str final : public gate {
private:
    std::string str;

public:
    print_str(std::string &s) : str(s) {}

    void apply(qu_register &qreg) override {
        println(str);
    }

    void dump() override { println(" print ", str, "\""); }

    gate_type_t type() const override { return __print_str__; }
};

} // namespace qx