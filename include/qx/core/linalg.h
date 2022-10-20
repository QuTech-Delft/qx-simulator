#pragma once

// #define __BUILTIN_LINALG_

// #ifndef __BUILTIN_LINALG__
// #include <boost/numeric/ublas/vector.hpp>
// #include <boost/numeric/ublas/matrix.hpp>
// #include <boost/numeric/ublas/io.hpp>
// #endif

#include "qx/core/matrix.h"

#ifdef USE_OPENMP
#include <omp.h>
#endif

#include <bitset>
#include <cstdint>
#include <iostream>
#include <vector>

#include "qx/xpu/aligned_memory_allocator.h"
#include "qx/xpu/complex.h"
#include "qx/xpu/vector.h"

#define MAX_QB_N 64

// #ifndef __BUILTIN_LINALG__
// using namespace boost::numeric;
// #endif

namespace qx {
namespace linalg {
// using complex_t = std::complex<double>;
using complex_t = xpu::complex_d;
// #ifndef __BUILTIN_LINALG__
// 	 using cvector_t ublas::vector<complex_t>;
// 	 using cmatrix_t ublas::matrix<complex_t>;
// 	 using cidentity_t = ublas::identity_matrix<complex_t>;
// #else
using cvector_t =
    std::vector<complex_t, xpu::aligned_memory_allocator<complex_t, 64>>;
// using cvector_t = xpu::vector<complex_t,16>;
// using cmatrix_t = qx::linalg::matrix<complex_t>;
using cmatrix_t = qx::linalg::tiny_matrix<complex_t, 2>;
using cidentity_t = qx::linalg::identity_matrix<complex_t>;
// #endif // __BUILTIN_LINALG__

using perm_t = std::vector<std::pair<uint32_t, uint32_t>>;

/**
 * \brief tensor product of vectors
 */
cvector_t tensor(cvector_t v1, cvector_t v2);

/**
 * \brief tensor product of matrices
 */
qx::linalg::matrix<complex_t> tensor(qx::linalg::matrix<complex_t> m1,
                                     qx::linalg::matrix<complex_t> m2);

/**
 * \brief tensor product of matrices (result in m)
 */
uint32_t tensor(qx::linalg::matrix<complex_t> &m1,
                qx::linalg::matrix<complex_t> &m2,
                qx::linalg::matrix<complex_t> &m);

/**
 * \brief nth tensor
 */
// cmatrix_t tensor(cmatrix_t m, uint32_t n);

/**
 * \brief matrix vector product
 */
cvector_t mxv(qx::linalg::matrix<complex_t> m, cvector_t v);

/**
 * \brief matrix matrix product
 */
qx::linalg::matrix<complex_t> mxm(qx::linalg::matrix<complex_t> m1,
                                  qx::linalg::matrix<complex_t> m2);

/**
 * \brief matrix matrix product
 */
cmatrix_t mxm(cmatrix_t m1, cmatrix_t m2);

/**
 * \brief verify if the matrices m1 and m2 are equals
 */
bool equals(qx::linalg::matrix<complex_t> &m1,
            qx::linalg::matrix<complex_t> &m2, double epsilon = 10e-14);

uint32_t pow2(uint32_t x);

/**
 * inc
 */
[[nodiscard]]
std::bitset<MAX_QB_N> inc(std::bitset<MAX_QB_N> in);

/**
 * perms
 */
perm_t perms(uint32_t n, uint32_t c, uint32_t t);

void perms(uint32_t n, uint32_t c, uint32_t t, cvector_t &amp);

perm_t perms(uint32_t n, uint32_t c1, uint32_t c2, uint32_t t);

/**
 * dump matrix
 */
void dump_matrix(cmatrix_t &m, bool complex_format = false);

} // namespace linalg
} // namespace qx