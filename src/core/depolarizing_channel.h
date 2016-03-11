#ifndef QX_DEPOLARIZING_CHANNEL_H
#define QX_DEPOLARIZING_CHANNEL_H

#include <random>
#include <core/error_injector.h>

namespace qx
{

   typedef std::vector<uint64_t>   histogram_t;


   #define __third__ (0.33333333333333333333333333333f)

   
   /**
    * \brief depolarizing channel implementation
    */
   class depolarizing_channel : public error_injector
   {
      public:

        /**
	 * ctor
	 */
        depolarizing_channel(qx::circuit * c, uint64_t nq, double pe) : nrg(0,__third__), c(c), nq(nq),
							                pe(pe), 
	                                                                xp(__third__), 
							                yp(__third__), 
							                zp(__third__),
									overall_error_probability(0),
									total_errors(0),
									error_histogram(nq+1,0)
	{
#if 0
	   println("   [e] error injector initialization... ");
	   println("   [e] single qubit error probability : " << pe);
#endif

	   for (uint64_t i=1; i<(nq+1); ++i)
	   {
	      double pp = error_probability(nq,i,pe);
	      overall_error_probability += pp;
	      // println("[i] simultaneous error(s) probability of " << i << " qubits out of " << nq << " : " << pp);
	   }

#if 0
	   println("   [e] total error probability        : " << overall_error_probability);
#endif
	   
	   srand48(xpu::timer().current());

	}
        

	/**
	 * ctor
	 */
	depolarizing_channel(qx::circuit * c, uint64_t nq, double pe, double xp, double yp, double zp) : nrg(0,__third__), c(c), nq(nq), 
	                                                                                                 pe(pe), 
											                 xp(xp), 
											                 yp(yp), 
											                 zp(zp),
													 overall_error_probability(0),
									                                 total_errors(0),
													 error_histogram(nq+1,0)
	{
#if 0
	   println("   [e] error injector initialization... ");
	   println("   [e] single qubit error probability : " << pe);
#endif

	   for (uint64_t i=1; i<(nq+1); ++i)
	   {
	      double pp = error_probability(nq,i,pe);
	      overall_error_probability += pp;
	      // println("[i] simultaneous error(s) probability of " << i << " qubits out of " << nq << " : " << pp);
	   }

#if 0
	   println("   [e] total error probability        : " << overall_error_probability);
#endif

	   srand48(xpu::timer().current());
	}

	#define __verbose__ if (verbose)

	/**
	 * \brief inject errors
	 * \return noisy circuit 
	 */
	qx::circuit *  inject(bool verbose=false)
	{
	   println("   [e] depolarizing_channel : injecting errors...");
	   uint64_t steps = c->size();
	   // uint64_t errors = 0; // number of injected errors
	   // uint64_t error_position=0;
	   qx::circuit * noisy_c = new qx::circuit(nq,c->id() + "(noisy)");

	   __verbose__ println("   [+] circuit steps : " << steps);
	   for (uint64_t p=0; p<steps; ++p)
	   {
	      noisy_c->add(c->get(p));
	      // error_position = p+errors;
	      // double x = drand48();
	      double x = uniform_rand();
	      //println("p = " << x);
	      if (x<overall_error_probability)
	      {
		 uint32_t affected_qubits = 1;

		 //__verbose__ println("--------------------------");
		 //__verbose__ println("[+] rand : " << x);

		 // x *= overall_error_probability;
		 for (uint32_t i=2; i<(nq+1); ++i)
		 {
		    if (x>error_probability(nq,i,pe))
		       break;
		    affected_qubits++;
		 }
		 // errors++; 
		 total_errors += affected_qubits;
		 error_histogram[affected_qubits]++;

		 __verbose__ println("   [>>>] error injection step " << p << " : number of affected qubits: " << affected_qubits);

		 if (affected_qubits==1)
		 {
		    uint64_t q = (rand()%nq);
		    //c->insert(error_position, new qx::pauli_x(q));
		    //noisy_c->add(new qx::pauli_x(q));
		    __verbose__  print("     |--> error on qubit  " << q);
		    noisy_c->add(single_qubit_error(q,verbose));
		 }
		 else
		 {
		    qx::parallel_gates * g = new qx::parallel_gates();

		    std::vector<bool> v(nq,0);
		    for (uint64_t i=0; i<affected_qubits; ++i)
		    { 
		       uint64_t q = (rand()%nq);
		       while (v[q])
			  q = (rand()%nq);
		       v[q] = 1;
		       // g->add(new qx::pauli_x(q));
		       __verbose__  print("     |--> error on qubit  " << q);
		       g->add(single_qubit_error(q,verbose));
		    }

		    //c->insert(error_position, g);
		    noisy_c->add(g);
		 }
	      }
	   }
	   println("   [+] total injected errors : " << total_errors);

	   return noisy_c;
	}


	/**
	 * \brief total errors
	 * \return total errors
	 */
	 uint32_t get_total_errors()
	 {
	    return total_errors;
	 }


	/**
	 * dump
	 */
	void dump()
	{
	   println("   [+] depolarizing channel :");
	   println("   [-] single qubit error probability : " << pe);
	   for (uint64_t i=1; i<(nq+1); ++i)
	   {
	      double pp = error_probability(nq,i,pe);
	      overall_error_probability += pp;
	      println("   [i] simultaneous error(s) probability of " << i << " qubits out of " << nq << " : " << pp);
	   }
	   println("   [-] probability of (x) errors: " << xp);
	   println("   [-] probability of (z) errors: " << yp);
	   println("   [-] probability of (y) errors: " << zp);
	}

      private:


	/**
	 * uniform random number generator
	 */
	double uniform_rand()
	{
	   return urg.next();
	}

	/**
	 * normal random number generator
	 */
	double normal_rand()
	{
	   double r = 1-std::abs(nrg.next()); 
	   r = (r < 0 ? 0 : (r > __limit__ ? __limit__ : r));
	   return r; 
	}

       /**
        * single qubit error 
	*/
	qx::gate * single_qubit_error(uint32_t q, bool verbose=false)
	{
	   //double p = drand48();
	   double p = uniform_rand();
	   if (p<xp)
	   {
	      __verbose__ println(" (x error) ");
	      return new qx::pauli_x(q);
	   }
	   else if (p<(zp+xp))
	   {
	      __verbose__ println(" (z error) ");
	      return new qx::pauli_z(q);
	   }
	   else
	   {
	      __verbose__ println(" (y error) ");
	      return new qx::pauli_y(q);
	   }
	}

	/**
	 * factorial
	 */
	uint64_t factorial(uint64_t n)
	{
	   return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
	}

	/**
	 * combinations
	 */
	uint64_t combinations(uint64_t n, uint64_t k)
	{
	   return (factorial(n)/(factorial(k)*factorial(n-k)));
	}

	/**
	 * nq : total number of qubits
	 * ne : number of simultaneously affected qubits
	 * p  : probability of single error
	 */
	double error_probability(uint64_t nq, uint64_t ne, double p)
	{
	   double c = (double)combinations(nq,ne);
	   double res = c*std::pow(p,ne)*std::pow(1-p,nq-ne);
	   return res;
	}

      private:

	/**
	 * parameters
	 */
	 
	 qx::normal_random_number_generator  nrg;
	 qx::uniform_random_number_generator urg;
	 
	 qx::circuit *        c;
	 uint64_t             nq;
	 
	 double               pe;
	 double               xp;
	 double               yp;
	 double               zp;
	 
	 double               overall_error_probability;
	 uint64_t             total_errors;
	 histogram_t          error_histogram;

   };
}


#endif // QX_ERROR_INJECTOR

