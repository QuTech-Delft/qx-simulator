/**
 * @file		register.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		01-10-15
 * @brief		quantum register implementation
 *
 */

#ifndef QX_REGISTER_H
#define QX_REGISTER_H

#include <iostream>
#include <complex>
#include <vector>
#include <cfloat>
#include <cassert>
#include <ctime>


#include <xpu/timer.h>
#include <core/linalg.h>

// #define SAFE_MODE 1  // state norm check
#define QUBIT_ERROR_THRESHOLD (1e-10)
#define RAND_RANGE 

using namespace qx::linalg;

#define __flt_format(x) (std::abs(x) < DBL_MIN ? 0 : x)
#define __cpl_format(x) __flt_format(x.real()) << ", " << __flt_format(x.imag()) 
#define __format_bin(x) (x == __state_0__ ? '0' : (x == __state_1__ ? '1' : 'X')) 

char bin_state_lt [] = { '0', '1', 'X' };




namespace qx
{
   /**
    * state_t
    */
   typedef enum __state_t
   {
      __state_0__,
      __state_1__,
      __state_unknown__
   } state_t;

   typedef std::vector<state_t> bvector_t;

   /**
    * \brief quantum register implementation.
    */
   class qu_register
   {
      private:

	 cvector_t  data;
	 bvector_t  binary; 
	 uint32_t   n_qubits; 

	 /**
	  * \brief collapse
	  */
	 uint32_t collapse(uint32_t entry);



	 /**
	  * \brief convert to binary
	  */
	 void to_binary(uint32_t state, uint32_t nq);


	 /**
	  * \brief set from binary
	  */
	 void set_binary(uint32_t state, uint32_t nq);



      public:

	 /**
	  * \brief quantum register of n_qubit
	  */
	 qu_register(uint32_t n_qubits);


	 /**
	  * reset
	  */
	 void reset();


	 /**
	  * \brief data getter
	  */
	 cvector_t& get_data();

	 /**
	  * \brief data setter
	  */
	 void set_data(cvector_t d);

	 /**
	  * \brief size getter
	  */
	 uint32_t size();

	 /**
	  * \brief get states
	  */
	 uint32_t states();

	 /**
	  * \brief assign operator
	  */
	 cvector_t & operator=(cvector_t d);

	 /**
	  * \brief return ith amplitude
	  */
	 complex_t& operator[](uint32_t i);

	 /**
	  * \brief qubit validity check
	  *   moduls squared of qubit entries must equal 1.
	  *
	  */
	 bool check();

	 /**
	  * \brief measures one qubit
	  */
	 int32_t measure();

	 /**
	  * \brief dump
	  */
	 void dump(bool only_binary);


	 void set_binary(uint32_t state);


	 /**
	  * \brief setter
	  * set bit <q>  to the state <s>
	  */
	 void set_binary(uint32_t q, state_t s);


	 /**
	  * \brief getter
	  * \return the state of bit <q> 
	  */
	 state_t get_binary(uint32_t q);


	 /**
	  * \brief test bit <q> of the binary register
	  * \return true if bit <q> is 1
	  */
	 bool test(uint32_t q); // throw (qubit_not_measured_exception)  // trow exception if qubit value is unknown (never measured) !!!!

	 /**
	  * \brief flip binary
	  */
	 void flip_binary(uint32_t q); 

   };

   /**
    * \brief fidelity
    */
   double fidelity(qu_register& s1, qu_register& s2);


   #include "register.cc"

}


#endif // QX_REGISTER_H

