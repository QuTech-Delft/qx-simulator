#include "qx/core/linalg.h"

namespace qx {
namespace linalg {
/**
 * \brief tensor product of vectors
 */
cvector_t tensor(cvector_t v1, cvector_t v2) {
    uint32_t n1 = v1.size();
    uint32_t n2 = v2.size();
    cvector_t res(n1 * n2);

#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t n = 0; n < (int64_t)res.size(); ++n)
        res[n] = 0.0;

    for (uint32_t i = 0; i < n1; ++i)
        for (uint32_t j = 0; j < n2; ++j)
            res[i * n2 + j] = v1[i] + v2[j];

    return res;
}

/**
 * \brief tensor product of matrices
 */
qx::linalg::matrix<complex_t> tensor(qx::linalg::matrix<complex_t> m1,
                                     qx::linalg::matrix<complex_t> m2) {
    uint32_t rows_1 = m1.size1();
    uint32_t cols_1 = m1.size2();
    uint32_t rows_2 = m2.size1();
    uint32_t cols_2 = m2.size2();

    uint32_t rows = rows_1 * rows_2;
    uint32_t cols = cols_1 * cols_2;
    complex_t z(0.0, 0.0);
    qx::linalg::matrix<complex_t> m(rows, cols, z);

    for (uint32_t i = 0; i < rows; ++i)
        for (uint32_t j = 0; j < cols; ++j)
            m(i, j) = m1(i / rows_2, j / cols_2) * m2(i % rows_2, j % cols_2);
    return m;
}

/**
 * \brief tensor product of matrices (result in m)
 */
uint32_t tensor(qx::linalg::matrix<complex_t> &m1,
                qx::linalg::matrix<complex_t> &m2,
                qx::linalg::matrix<complex_t> &m) {
    uint32_t rows_1 = m1.size1();
    uint32_t cols_1 = m1.size2();
    uint32_t rows_2 = m2.size1();
    uint32_t cols_2 = m2.size2();

    uint32_t rows = rows_1 * rows_2;
    uint32_t cols = cols_1 * cols_2;

    // cmatrix_t m(rows,cols);

    if ((m.size1() != rows) || (m.size2() == cols))
        m.resize(rows, cols, false);

    for (uint32_t i = 0; i < rows; ++i)
        for (uint32_t j = 0; j < cols; ++j)
            m(i, j) = m1(i / rows_2, j / cols_2) * m2(i % rows_2, j % cols_2);
    return 0;
}

/**
 * \brief nth tensor
 */
/*
cmatrix_t tensor(cmatrix_t m, uint32_t n)
{
  cmatrix_t r(m);
  while (r.size1() < n)
        r = tensor(r,m);
  return r;
}
*/


/**
 * \brief matrix vector product
 */
cvector_t mxv(qx::linalg::matrix<complex_t> m, cvector_t v) {
    // #ifdef __BUILTIN_LINALG__
    uint32_t n = v.size();
    cvector_t r(n);

#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int64_t i = 0; i < (int64_t)n; ++i) {
        complex_t c(0.0, 0.0);
        for (uint32_t j = 0; j < n; ++j)
            c += m(i, j) * v[j];
        r[i] = c;
    }

    return r;
    // #else
    // 	    return ublas::prec_prod(m,v);
    // #endif // __BUILTIN_LINALG__
}

/**
 * \brief matrix matrix product
 */
qx::linalg::matrix<complex_t> mxm(qx::linalg::matrix<complex_t> m1,
                                  qx::linalg::matrix<complex_t> m2) {
    // #ifdef __BUILTIN_LINALG__
    complex_t z(0.0, 0.0);
    qx::linalg::matrix<complex_t> r(m1.size1(), m2.size2(), z);
    qx::linalg::mul(m1, m2, r);
    return r;
    // #else
    // 	    return ublas::prec_prod(m1,m2);
    // #endif
}

/**
 * \brief matrix matrix product
 */
cmatrix_t mxm(cmatrix_t m1, cmatrix_t m2) {
    // #ifdef __BUILTIN_LINALG__
    complex_t z(0.0, 0.0);
    cmatrix_t r;
    qx::linalg::mul(m1, m2, r);
    return r;
    // #else
    // 	    return ublas::prec_prod(m1,m2);
    // #endif
}

/**
 * \brief verify if the matrices m1 and m2 are equals
 */
bool equals(qx::linalg::matrix<complex_t> &m1,
            qx::linalg::matrix<complex_t> &m2, double epsilon) {
    if (m1.size1() != m2.size1())
        return false;

    if (m1.size2() != m2.size2())
        return false;

    for (std::size_t i = 0; i < m1.size1(); ++i)
        for (std::size_t j = 0; j < m1.size2(); ++j) {
            // double d = std::abs(std::norm(m1(i,j))-std::norm(m2(i,j)));
            double d = m1(i, j).norm() - m2(i, j).norm();
            if (d > epsilon)
                return false;
        }
    return true;
}

uint32_t pow2(uint32_t x) { return (1 << x); }

/**
 * inc
 */
[[nodiscard]]
std::bitset<MAX_QB_N> inc(std::bitset<MAX_QB_N> in) {
    for (size_t i = 0; i < MAX_QB_N; ++i) {
        if (in[i] == 0) {
            in[i] = 1;
            break;
        }
        in[i] = 0; // this entry was 1; set to zero and carry the 1
    }
    return in;
}

/**
 * perms
 */
// std::vector<std::pair<uint32_t,uint32_t> >
perm_t perms(uint32_t n, uint32_t c, uint32_t t) {
    uint32_t nn = (1 << n);
    uint32_t p1, p2;
    std::bitset<MAX_QB_N> b;
    // std::vector<std::pair<uint32_t,uint32_t> > perms;
    perm_t res;

    b.reset();
    b.set(c);

    uint32_t bc = b.to_ulong();

    while (bc < nn) {
        b.set(c);
        p1 = b.to_ulong();
        b.flip(t);
        p2 = b.to_ulong();
        if (p2 > bc)
            res.push_back(std::pair<uint32_t, uint32_t>(p1, p2));
        b.flip(t);
        b = inc(b);
        b.set(c);
        bc = b.to_ulong();
    }

    // for (int i=0; i<res.size(); ++i)
    //  println("(" << res[i].first << ", " << res[i].second << ")");

    return res;
}

void perms(uint32_t n, uint32_t c, uint32_t t, cvector_t &amp) {
    uint32_t nn = (1 << n);
    uint32_t p1, p2;
    std::bitset<MAX_QB_N> b;
    // perm_t res;

    b.reset();
    b.set(c);

    uint32_t bc = b.to_ulong();

    while (bc < nn) {
        b.set(c);
        p1 = b.to_ulong();
        b.flip(t);
        p2 = b.to_ulong();
        if (p2 > bc) {
            std::swap(amp[p1], amp[p2]);
            // println("__swap(" << std::bitset<16>(p1) << ", " <<
            // std::bitset<16>(p2) << ")");
        }
        b.flip(t);
        b = inc(b);
        b.set(c);
        bc = b.to_ulong();
    }

    // return res;
}

perm_t perms(uint32_t n, uint32_t c1, uint32_t c2, uint32_t t) {
    uint32_t nn = (1 << n);
    uint32_t p1, p2;
    std::bitset<MAX_QB_N> b;
    // std::vector<std::pair<uint32_t,uint32_t> > perms;
    perm_t res;

    b.reset();
    b.set(c1);
    b.set(c2);

    uint32_t bc = b.to_ulong();

    while (bc < nn) {
        b.set(c1);
        b.set(c2);
        p1 = b.to_ulong();
        b.flip(t);
        p2 = b.to_ulong();
        res.push_back(std::pair<uint32_t, uint32_t>(p1, p2));
        b.flip(t);
        b = inc(b);
        b.set(c1);
        b.set(c2);
        bc = b.to_ulong();
    }

    // for (int i=0; i<res.size(); ++i)
    //  println("(" << res[i].first << ", " << res[i].second << ")");

    return res;
}

/**
 * dump matrix
 */
void dump_matrix(cmatrix_t &m, bool complex_format) {
    for (uint32_t i = 0; i < m.size1(); ++i) {
        println("");
        for (uint32_t j = 0; j < m.size1(); ++j) {
            if (complex_format)
                print(m(i, j), "  ");
            else
                print(m(i, j).re, "  ");
            // else print(m(i,j).real(), "  ");
        }
    }
    println("");
}

} // namespace linalg
} // namespace qx