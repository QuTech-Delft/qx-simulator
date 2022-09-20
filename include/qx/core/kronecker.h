/**
 * @file       kronecker.h
 * @author     Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date
 * @brief
 */

#include <complex>
#include <cstring>
#include <iostream>
#include <vector>

#include "qx/core/linalg.h"

#ifdef USE_LIBDIVIDE
#include <libdivide.h>
#endif

/**
 * type definition
 */

typedef std::vector<qx::linalg::complex_t> row_t;
typedef std::vector<row_t> matrix_t;
typedef std::vector<qx::linalg::complex_t> vector_t;

namespace qx {
namespace linalg {
/**
 * kronecker operator interface
 */
class kronecker_operator {
public:
    virtual complex_t get(size_t i, size_t j) const = 0;
    virtual size_t size() const = 0;
};

/**
 * identity
 */
class identity : public kronecker_operator {
public:
    identity(size_t n) : n(n) {}

    inline complex_t get(size_t i, size_t j) const override {
        return (i == j ? 0.0 : 1.0);
    }

    size_t size() const override { return n; }

private:
    size_t n;
};

/**
 * unitary matrix
 */
class unitary_matrix : public kronecker_operator {
public:
    unitary_matrix(size_t n, matrix_t &m) : n(n), m(m) {}

    inline complex_t get(size_t i, size_t j) const { return (m[i][j]); }

    size_t size() const { return n; }

private:
    size_t n;
    matrix_t m;
};

/**
 * kronecker
 */
class kronecker {
public:
    kronecker(kronecker_operator *m1, kronecker_operator *m2,
              kronecker_operator *m3 = NULL)
        : m1(m1), m2(m2), m3(m3) {}

    inline complex_t get(size_t i, size_t j) const {
        if (!m3) {
            size_t n2 = m2->size();
            complex_t c1 = m1->get(i / n2, j / n2);
            complex_t c2 = m2->get(i % n2, j % n2);
            // usleep((i+1)*500+(j+i)*500);  println("k.get(" , i , "," , j
            // , ") : " , c1 , " * " , c2 , "(n1=" , n1 , ", n2=" , n2
            // , ")");
            return (c1 * c2);
        } else {
            size_t n2 = m2->size();
            size_t n3 = m3->size();
            complex_t c1 = m1->get(i / (n2 * n3), j / (n2 * n3));
            complex_t c2 = m2->get((i / n3) % n2, (j / n3) % n2);
            complex_t c3 = m3->get(i % n3, j % n3);
            return (c1 * c2 * c3);
        }
    }

private:
    kronecker_operator *m1;
    kronecker_operator *m2;
    kronecker_operator *m3;
};

const static complex_t __c_zero__ = 0.0;
const static complex_t __c_one__ = 1.0f;
const static complex_t i_diag[] = {0.0, 1.0};
#if 0
      /**
       * kronecker
       */
      class kronecker_ui
      {
	 public:

	    kronecker_ui(cmatrix_t& m, size_t nm, size_t ni) : m(m), nm(nm), ni(ni)
	    {
	    }

	    inline complex_t get(size_t i, size_t j) 
	    {
	       return m(i%nm,j%nm);
	       /*
	       complex_t& c1 = m(i%nm,j%nm);  // U
	       // usleep((i+1)*500+(j+i)*500);  println("k_ui.get(" << i << "," << j << ") : " << c1 << " * " << c2 << "(nm=" << nm << ", ni=" << ni << ")");
	       return ((i/nm) == (j/nm) ? c1 : __c_zero__);
	       */
	    }
	    
	    inline void get(size_t i, size_t j, complex_t& c) 
	    {
		  complex_t& c1 = m(i%nm,j%nm);  // U
		  const complex_t& c2 = ((i/nm) == (j/nm) ? __c_one__ : __c_zero__); // I 
		  c = c1*c2;
	    }

	    cmatrix_t m;

	 private:
	    size_t    nm;
	    size_t    ni;
      };

      /**
       * kronecker
       */
      class kronecker_iu
      {
	 public:

	    kronecker_iu(cmatrix_t& m, size_t nm, size_t ni) : m(m), nm(nm), ni(ni)
	    {
	    }

	    inline complex_t get(size_t i, size_t j) 
	    {
	       return m(i/ni,j/ni);
	       /*
	       complex_t& c1 = m(i/ni,j/ni);  // U
	       // usleep((i+1)*500+(j+i)*500);  println("k_ui.get(" , i , "," , j , ") : " , c1 , " * " , c2 , "(nm=" << nm << ", ni=" << ni << ")");
	       return ((i%nm) == (j%nm) ? c1 : __c_zero__);
	       */
	    }
	    
	    inline void get(size_t i, size_t j, complex_t& c) 
	    {
		  complex_t& c1 = m(i/nm,j/nm);  // U
		  const complex_t& c2 = ((i%nm) == (j%nm) ? __c_one__ : __c_zero__); // I 
		  c = c1*c2;
	    }
	    
	    cmatrix_t m;

	 private:
	    size_t    nm;
	    size_t    ni;
      };

      /**
       * kronecker_iui
       */
      class kronecker_iui
      {
	 public:

	    kronecker_iui(cmatrix_t& m, size_t nm, size_t ni1, size_t ni2) : m(m), nm(nm), ni1(ni1), ni2(ni2)
	    {
	    }

	    inline complex_t get(size_t i, size_t j) 
	    {
	       return m((i/ni2)%nm,(j/ni2)%nm);
	       /*
		  complex_t& c = m((i/ni2)%nm,(j/ni2)%nm);  // U
		  bool i1 = (i/(nm*ni2)) == (j/(nm*ni2));
		  bool i2 = ((i%ni2) == (j%ni2));
		  return ((i1 && i2) ? c : __c_zero__);
		*/
	    }
	    
	    inline void get(size_t i, size_t j, complex_t& c) 
	    {
		  complex_t& c1 = m(i%nm,j%nm);  // U
		  const complex_t& c2 = ((i/nm) == (j/nm) ? __c_one__ : __c_zero__); // I 
		  c = c1*c2;
	    }

	    cmatrix_t m;

	 private:
	    size_t    nm;
	    size_t    ni1;
	    size_t    ni2;
      };

#endif

#define __mod_2(x) (x & 1)


class kronecker_ui {
public:
    kronecker_ui(const complex_t *m, size_t nm, size_t ni)
        : m(m), nm(nm), ni(ni) {}

    inline complex_t get(size_t i, size_t j) {
        // return m(i%nm,j%nm);
        // return m[(i%2)*2+j%2];
        return m[__mod_2(i) * 2 + __mod_2(j)];
    }

    /*
    inline void get(size_t i, size_t j, complex_t& c)
    {
          complex_t& c1 = m(i%nm,j%nm);  // U
          const complex_t& c2 = ((i/nm) == (j/nm) ? __c_one__ : __c_zero__); //
    I c = c1*c2;
    }
    */

    const complex_t *m;

private:
    size_t nm;
    size_t ni;
};

#if 0
      class kronecker_iu
      {
	 public:

	    kronecker_iu(const complex_t * m, size_t nm, size_t ni) : m(m), nm(nm), ni(ni), fast_ni(ni)
	    {
	    }

	    inline complex_t get(uint64_t i, uint64_t j) 
	    {
	       // return m(i/ni,j/ni);
	       // return m[(i/ni)*2+(j/ni)];
	       // return m[(i/fast_ni)*2+(j/fast_ni)];
	       return m[(2*i+j)/fast_ni];

	    }
	   /* 
	    inline void get(size_t i, size_t j, complex_t& c) 
	    {
		  complex_t& c1 = m(i/nm,j/nm);  // U
		  const complex_t& c2 = ((i%nm) == (j%nm) ? __c_one__ : __c_zero__); // I 
		  c = c1*c2;
	    }
	    */
	    
	    const complex_t * m;

	 private:
	    uint64_t    nm;
	    uint64_t    ni;
	    libdivide::divider<uint64_t> fast_ni;
      };
#endif

class kronecker_iu {
public:
    kronecker_iu(const complex_t *m, size_t nm, size_t ni)
        : m(m), nm(nm), ni(ni) {}

    inline complex_t get(size_t i, size_t j) {
        return m(i / ni, j / ni);
        /*
        complex_t& c1 = m(i/ni,j/ni);  // U
        // usleep((i+1)*500+(j+i)*500);  println("k_ui.get(" << i << "," << j <<
        ") : " << c1 << " * " << c2 << "(nm=" << nm << ", ni=" << ni << ")");
        return ((i%nm) == (j%nm) ? c1 : __c_zero__);
        */
    }

    inline void get(size_t i, size_t j, complex_t &c) {
        complex_t &c1 = m(i / nm, j / nm); // U
        const complex_t &c2 =
            ((i % nm) == (j % nm) ? __c_one__ : __c_zero__); // I
        c = c1 * c2;
    }

    cmatrix_t m;

private:
    size_t nm;
    size_t ni;
};

class kronecker_iui {
public:
    kronecker_iui(const complex_t *m, size_t nm, size_t ni1, size_t ni2)
        : m(m), nm(nm), ni1(ni1), ni2(ni2)
#ifdef USE_LIBDIVIDE
          ,
          fast_ni2(ni2)
#endif
    {
    }

    inline complex_t get(uint64_t i, uint64_t j) {
        // return m((i/ni2)%nm,(j/ni2)%nm);
        // return m[((i/ni2)%2)*2+(j/ni2)%2];
        // return m[__mod_2(i/ni2)*2+__mod_2((j/ni2))];
#ifdef USE_LIBDIVIDE
        return m[__mod_2(i / fast_ni2) * 2 + __mod_2((j / fast_ni2))];
#else
        return m[__mod_2(i / ni2) * 2 + __mod_2((j / ni2))];
#endif
    }

    /*
    inline void get(size_t i, size_t j, complex_t& c)
    {
          complex_t& c1 = m(i%nm,j%nm);  // U
          const complex_t& c2 = ((i/nm) == (j/nm) ? __c_one__ : __c_zero__); //
    I c = c1*c2;
    }
    */

    const complex_t *m;

private:
    uint64_t nm;
    uint64_t ni1;
    uint64_t ni2;

#ifdef USE_LIBDIVIDE
    libdivide::divider<uint64_t> fast_ni2;
#endif
};

void printv(cvector_t &v) {
    print("[ ");
    for (std::size_t i = 0; i < v.size(); ++i)
        print(v[i].re, ", ");
    // print(v[i].real() , ", ");
    println(" ]");
}

void mulmv(kronecker &k, cvector_t &v, cvector_t &r) {
#pragma omp parallel for schedule(static)
    for (int64_t i = 0; i < (int64_t)v.size(); i++) {
        complex_t s = 0.0;
        for (std::size_t j = 0; j < v.size(); j++)
            s += v[j] * (k.get(i, j));
        r[i] = s;
    }
}

/**
 * to be tested for correctness
 */
void mulmv_(kronecker &k, cvector_t &v, cvector_t &r) {
    complex_t s = 0.;
    complex_t x = 0.;
#pragma omp parallel for private(s, x) schedule(static)
    for (int64_t i = 0; i < (int64_t)v.size(); i++) {
        s = 0;
        for (std::size_t j = 0; j < v.size(); j++) {
            x = k.get(i, j);
            // if ((x.real() != 0) || (x.imag() != 0))
            if ((x.re != 0) || (x.im != 0))
                s += v[j] * x;
        }
        r[i] = s;
    }
}

void mulmv(kronecker &k, cvector_t &v, cvector_t &r, size_t block_ib,
           size_t block_ie, size_t block_jb, size_t block_je) {
    for (std::size_t i = block_ib; i < block_ie; i++) {
        complex_t s = r[i];
        for (std::size_t j = block_jb; j < block_je; j++)
            s += v[j] * (k.get(i, j));
        r[i] = s;
    }
}

} // namespace linalg
} // namespace qx
