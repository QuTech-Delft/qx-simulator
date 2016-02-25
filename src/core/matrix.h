#ifndef QX_MATRIX_H
#define QX_MATRIX_H


#include <iostream>
#include <vector>
#include <complex>

#include <cassert>

#include <stdint.h>

namespace qx
{
   namespace linalg
   {
      


         /**
	  * \brief matrix
	  */
      template <typename __T>
	 class matrix
	 {

	    typedef std::vector<__T> row_t;

	    public:

	    matrix() : rows(0), cols(0)
	    {
	    }


	    matrix(uint32_t rows, uint32_t cols) : rows(rows), cols(cols)
	    {
	       for (uint32_t r=0; r<rows; ++r)
		  m.push_back(row_t(cols));
	    }

	    matrix(const matrix<__T>& x) 
	    {
	       rows = x.size1();
	       cols = x.size2();
	       for (uint32_t r=0; r<x.size1(); ++r)
	       {
		  m.push_back(row_t(x.m[r]));
		  //for (uint32_t j=0; j<x.size2(); ++j)
		     //m(i,j) = x(i,j);
	       }
	    }

	    matrix<__T>& operator=(matrix<__T> x)
	    {
	       rows = x.size1(); 
	       cols = x.size2();
	       m.clear();
	       for (uint32_t r=0; r<x.size1(); ++r)
	       {
		  m.push_back(row_t(x.m[r]));
	       }
	       return *this;
	    }

	    void resize(uint32_t rows, uint32_t cols, bool zero=false)
	    {
	       this->rows = rows;
	       this->cols = cols;
	       m.clear();
	       for (uint32_t r=0; r<rows; ++r)
		  m.push_back(row_t(cols));

	    }

	    __T& operator()(uint32_t r, uint32_t c)
	    {
	       return m[r][c];
	    }

	    uint32_t size1() const
	    {
	       return rows;
	    }

	    uint32_t size2() const
	    {
	       return cols;
	    }

	    private:

	    uint32_t rows;
	    uint32_t cols;

	    std::vector<row_t>       m;
	 };


	 /**
	  * \brief identity
	  */
      template <typename __T>
	 class identity_matrix
	 {

	    typedef std::vector<__T> diagonal_t;

	    public:

	    /**
	     * \brief identity_matrix
	     */
	    identity_matrix(uint32_t size) : size(size), diag(size), zero(0)
	    {
	       for (uint32_t i=0; i<size; ++i)
		  diag[i] = 1;
	    }

	    __T& operator()(uint32_t r, uint32_t c)
	    {
	       if (r == c)
		  return diag[r];
	       else
		  return zero;
	    }

	    uint32_t size1()
	    {
	       return size;
	    }

	    uint32_t size2()
	    {
	       return size;
	    }

	    private:

	    uint32_t   size;
	    diagonal_t diag;
	    __T        zero;

	 };

	 template<typename __T>
	 void mul(matrix<__T>& m1, matrix<__T>& m2, matrix<__T>& r)
	 {
	    assert(m1.size1() == m2.size2());
	    assert(m1.size2() == m2.size1());

	    for (uint32_t i=0; i<m1.size1(); ++i)
	    {
	       for (uint32_t j=0; j<m2.size2(); ++j)
	       {
		  __T x = 0;
		  for (uint32_t k=0; k<m1.size2(); ++k)
		     x += m1(i,k)*m2(k,j);
		  r(i,j) = x;
	       }
	    }
	 }


	 template <typename __T>
	 bool equals(matrix<__T>& m1, matrix<__T>& m2)
	 {
	    if (m1.size1() != m2.size1()) return false;
	    if (m1.size2() != m2.size2()) return false;
	    for (uint32_t i=0; i<m1.size1(); ++i)
	       for (uint32_t j=0; j<m1.size2(); ++j)
		  if (m1(i,j) != m2(i,j)) return false;
	    return true;
	 }
	 
	 template <typename __T>
	 bool equals(matrix<__T>& m1, identity_matrix<__T>& m2)
	 {
	    if (m1.size1() != m2.size1()) return false;
	    if (m1.size2() != m2.size2()) return false;
	    for (uint32_t i=0; i<m1.size1(); ++i)
	       for (uint32_t j=0; j<m1.size2(); ++j)
		  if (m1(i,j) != m2(i,j)) return false;
	    return true;
	 }



   }
}



#endif // QX_MATRIX_H

