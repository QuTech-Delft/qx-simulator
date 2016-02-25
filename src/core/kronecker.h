/**
 * @file       kronecker.h
 * @author     Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date
 * @brief      
 */

#include <iostream>
#include <vector>
#include <complex>
#include <cstring>

#include <core/linalg.h>

//#define println(x) std::cout << x << std::endl
//#define print(x) std::cout << x 


/**
 * type definition
 */

typedef std::complex<double>   complex_t;
typedef std::vector<complex_t> row_t;
typedef std::vector<row_t>     matrix_t;
typedef std::vector<complex_t> vector_t;


namespace qx
{
   namespace linalg
   {
      /**
       * kronecker operator interface
       */
      class kronecker_operator
      {
	 public:

	    virtual complex_t get(size_t i, size_t j) const = 0;
	    virtual size_t size() const = 0;
      };

      /**
       * identity
       */
      class identity : public kronecker_operator
      {
	 public:

	    identity(size_t n) : n(n)
	 { }

	    complex_t get(size_t i, size_t j) const
	    {
	       return (i==j ? 1 : 0);
	    }

	    size_t size() const
	    {
	       return n;
	    }

	 private:

	    size_t n;
      };


      /**
       * unitary matrix
       */
      class unitary_matrix : public kronecker_operator
      {
	 public:

	    unitary_matrix(size_t n, matrix_t& m) : n(n), m(m)
	 { }

	    complex_t get(size_t i, size_t j) const
	    {
	       return (m[i][j]);
	    }

	    size_t size() const
	    {
	       return n;
	    }

	 private:

	    size_t   n;
	    matrix_t m;
      };


      /**
       * kronecker
       */
      class kronecker
      {
	 public:

	    kronecker(kronecker_operator * m1,
		  kronecker_operator * m2,
		  kronecker_operator * m3=NULL) : m1(m1),
	    m2(m2),
	    m3(m3)
	 {
	 }

	    complex_t get(size_t i, size_t j)
	    {
	       if (m3)
	       {
		  size_t n1 = m1->size();
		  size_t n2 = m2->size();
		  size_t n3 = m3->size();
		  complex_t c1 = m1->get(i/(n2*n3), j/(n2*n3));
		  complex_t c2 = m2->get((i/n3)%n2, (j/n3)%n2);
		  complex_t c3 = m3->get(i%n3,      j%n3);
		  return (c1*c2*c3);
	       }
	       else
	       {
		  size_t n1 = m1->size();
		  size_t n2 = m2->size();
		  complex_t c1 = m1->get(i/n2, j/n2);
		  complex_t c2 = m2->get(i%n2, j%n2);
		  return (c1*c2);
	       }
	    }


	 private:

	    kronecker_operator * m1;
	    kronecker_operator * m2;
	    kronecker_operator * m3;

      };


      void printv(cvector_t& v)
      {
	 print("[ ");
	 for (int i=0; i<v.size(); ++i)
	    print(v[i].real() << ", ");
	 println(" ]");

      }


      void mulmv(kronecker& k, cvector_t& v, cvector_t& r)
      {
	 #pragma omp parallel for schedule(static)
	 for (int i=0; i<v.size(); i++)
	 {
	    complex_t s = 0;
	    for (int j=0; j<v.size(); j++)
	       s += v[j]*(k.get(i,j));
	    r[i] = s;
	 }
      }

      /**
       * to be tested for correctness
       */
      void mulmv_(kronecker& k, cvector_t& v, cvector_t& r)
      {
	 complex_t s = 0;
	 complex_t x = 0;
	 #pragma omp parallel for private(s,x) schedule(static)
	 for (int i=0; i<v.size(); i++)
	 {
	    s = 0;
	    for (int j=0; j<v.size(); j++)
	    {
	       x = k.get(i,j);
	       if ((x.real() != 0) || (x.imag() != 0))
	           s += v[j]*x;
	    }
	    r[i] = s;
	 }
      }


      void mulmv(kronecker& k, cvector_t& v, cvector_t& r, size_t block_ib, size_t block_ie, size_t block_jb, size_t block_je)
      {
	 for (int i=block_ib; i<block_ie; i++)
	 {
	    complex_t s = r[i];
	    for (int j=block_jb; j<block_je; j++)
	       s += v[j]*(k.get(i,j));
	    r[i] = s;
	 }
      }


   } // namespace qx 
} // namespace linalg
