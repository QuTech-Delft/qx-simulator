#include <exception>

/**
 * set_binary
 */
// void qx::qu_register::set_binary(uint64_t state, uint64_t nq)
void qx::qu_register::set_measurement_prediction(uint64_t state, uint64_t nq)
{
   //uint64_t k=0;
   uint64_t k=nq-1;
   while (nq--)
   {
      // binary[k--] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
      measurement_prediction[k--] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
   }
}


/**
 * \brief set measurement outcome
 */
void qx::qu_register::set_measurement(uint64_t state, uint64_t nq)
{
   //uint64_t k=0;
   uint64_t k=nq-1;
   while (nq--)
   {
      // binary[k--] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
      measurement_register[k--] = ((state >> nq) & 1);
   }
}

/**
 * collapse state
 */
uint64_t qx::qu_register::collapse(uint64_t entry)
{
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
   for (int64_t i=0; i<(int64_t)data.size(); ++i)
   {
      data[i] = 0.0;
   }
   data[entry] = 1;
   // set_binary(entry,n_qubits);
   set_measurement_prediction(entry,n_qubits);
   set_measurement(entry,n_qubits);
   return entry;
}


/**
 * to binary
 */
void qx::qu_register::to_binary(uint64_t state, uint64_t nq)
{
   uint64_t k=0;
   while (nq--)
      std::cout << (((state >> nq) & 1) ? "1" : "0");
}

/**
 * to binary string
 */
std::string  qx::qu_register::to_binary_string(uint64_t state, uint64_t nq)
{
   std::string s(nq,'0');
   uint64_t k=0;
   while (nq--)
   {
      s[k] = (((state >> nq) & 1) ? '1' : '0');
      k++;
   }
   return s;
}


/**
 * \brief quantum register of n_qubit
 */
// qx::qu_register::qu_register(uint64_t n_qubits) : data(1 << n_qubits), binary(n_qubits), n_qubits(n_qubits), rgenerator(xpu::timer().current()*10e5), udistribution(.0,1)
//qx::qu_register::qu_register(uint64_t n_qubits) : data(1 << n_qubits), measurement_prediction(n_qubits), measurement_register(n_qubits), n_qubits(n_qubits), rgenerator(xpu::timer().current()*10e5), udistribution(.0,1)
qx::qu_register::qu_register(uint64_t n_qubits) : data(1ULL << n_qubits), aux(1ULL << n_qubits), measurement_prediction(n_qubits), measurement_register(n_qubits), n_qubits(n_qubits), rgenerator(xpu::timer().current()*10e5), udistribution(.0,1), measurement_averaging_enabled(true), measurement_averaging(n_qubits)
{
   if(n_qubits>63) {
	   throw std::invalid_argument("hard limit of 63 qubits exceeded");
   }

   uint64_t num_elts = (1ULL << n_qubits);

#ifdef USE_OPENMP
#pragma omp parallel for
#endif
   for (int64_t i=0; i<(int64_t)num_elts; ++i) {
      data[i] = 0.0;
      aux[i] = 0.0;
   }
   data[0] = complex_t(1,0);

   for (uint64_t i=0; i<n_qubits; i++)
   {
      measurement_prediction[i] = __state_0__;
      measurement_register[i]   = 0;
   }

   for (size_t i=0; i<measurement_averaging.size(); ++i)
   {
      measurement_averaging[i].ground_states = 0;
      measurement_averaging[i].ground_states = 0;
   }
}


/**
 * reset
 */
void qx::qu_register::reset()
{
   uint64_t num_elts = (1ULL << n_qubits);

#ifdef USE_OPENMP
#pragma omp parallel for
#endif
   for (int64_t i=0; i<(int64_t)num_elts; ++i) {
      data[i] = 0.0;
   }
   data[0] = complex_t(1,0);
   
   for (uint64_t i=0; i<n_qubits; i++)
   {
      // binary[i] = __state_0__;
      measurement_prediction[i] = __state_0__;
      measurement_register[i]   = 0;
   }
}


/**
 * \brief data getter
 */
cvector_t& qx::qu_register::get_data()
{
   return data;
}

cvector_t& qx::qu_register::get_aux()
{
   return aux;
}

/**
 * \brief data setter
 */
void qx::qu_register::set_data(cvector_t d)
{
   data = d;
}


/**
 * \brief size getter
 */
uint64_t qx::qu_register::size()
{
   return n_qubits;
}


/**
 * \brief get states
 */
uint64_t qx::qu_register::states()
{
   return (1ULL << n_qubits);
}


/**
 * \brief assign operator
 */
cvector_t & qx::qu_register::operator=(cvector_t d)
{ 
   assert(d.size() == data.size());
   data.resize(d.size());
   data = d;
   // data.resize(d.size());
   return data;
} 


/**
 * \brief return ith amplitude
 */
complex_t& qx::qu_register::operator[](uint64_t i)
{
   return data[i];
}


/**
 * \brief qubit validity check
 *   moduls squared of qubit entries must equal 1.
 *
 */
bool qx::qu_register::check()
{
   double sum=0;
#ifdef USE_OPENMP
#pragma omp parallel for reduction(+:sum)
#endif
   for (int64_t i=0; i<(int64_t)data.size(); ++i)
      sum += data[i].norm();
      // sum += std::norm(data[i]);
   println("[+] register validity check : " << sum) ;
   return (std::fabs(sum-1) < QUBIT_ERROR_THRESHOLD);
}


/**
 * \brief measures one qubit
 */
int64_t qx::qu_register::measure()
{
#ifdef SAFE_MODE
   if (!check())
      return -1;
#endif // SAFE_MODE
   
   //srand48(xpu::timer().current());
   //double r = drand48();
   double r = this->rand();
   
   for (int i=0; i<data.size(); ++i)
   {
      // r -= std::norm(data[i]);
      r -= data[i].norm();
      if (r <= 0)
      {
         collapse(i);
         return 1; 
      }
   }
   return -1;
}

#define __amp_epsilon__ (0.000001f)

/**
 * \brief dump
 */
void qx::qu_register::dump(bool only_binary=false)
{
    if (!only_binary)
    {
        println("--------------[quantum state]-------------- ");
        std::streamsize stream_size = std::cout.precision();
        // std::cout.precision(std::numeric_limits<double>::digits10);
        std::cout.precision(7); //std::numeric_limits<double>::digits10);
        std::cout << std::fixed;
        for (std::size_t i=0; i<data.size(); ++i)
        {
            if ((std::abs(data[i].re) > __amp_epsilon__) ||
                (std::abs(data[i].im) > __amp_epsilon__))
            {
                print("  [p = " << std::showpos << data[i].norm() << "]");
                print("  " << std::showpos << data[i] << " |");
                to_binary(i,n_qubits);
                println("> +");
            }
        }
        std::cout.precision(stream_size);
    }
    if (measurement_averaging_enabled)
    {
        std::setprecision(9);
        println("------------------------------------------- ");
        print("[>>] measurement averaging (ground state) :");
        print(" ");
        for (int i=measurement_averaging.size()-1; i>=0; --i)
        {
            double gs = measurement_averaging[i].ground_states;
            double es = measurement_averaging[i].exited_states;
            double av = ((es+gs) != 0. ? (gs/(es+gs)) : 0.);
            print(" | " << std::setw(9) << av);  
        }
        println(" |");
    }
    println("------------------------------------------- ");
    print("[>>] measurement prediction               :");
    print(" ");
    for (int i=measurement_prediction.size()-1; i>=0; --i)
        print(" | " <<  std::setw(9) << __format_bin(measurement_prediction[i]));  
    println(" |");
    println("------------------------------------------- ");
    print("[>>] measurement register                 :");
    print(" ");
    for (int i=measurement_register.size()-1; i>=0; --i)
        print(" | " <<  std::setw(9) << (measurement_register[i] ? '1' : '0'));  
    println(" |");
    println("------------------------------------------- ");
}

/**
 * \brief return the quantum state as string
 */
std::string qx::qu_register::get_state(bool only_binary=false)
{
   std::stringstream ss;
   if (!only_binary)
   {
      std::cout << std::fixed;
      for (int i=0; i<data.size(); ++i)
      {
         if (data[i] != complex_t(0,0)) 
         {
            ss << "   " << std::showpos << std::setw(7) << data[i] << " |"; ss << to_binary_string(i,n_qubits); ss << "> +";
            ss << "\n";
         }
      }
   }
   return ss.str();
}


/**
 * set_binary
 */
void qx::qu_register::set_measurement_prediction(uint64_t state)
{
   // print("  [-] set binary register to state : ");
   to_binary(state,n_qubits);
   uint64_t k=0;
   uint64_t nq = n_qubits;
   while (nq--)
   {
      // binary[k++] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
      measurement_prediction[k++] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
   }
}


/**
 * \brief setter
 * set bit <q>  to the state <s>
 */
void qx::qu_register::set_measurement_prediction(uint64_t q, state_t s)
{
   assert(q<n_qubits);
   // binary[q] = s;
   measurement_prediction[q] = s;
}

/**
 * \brief setter
 * set measurement outcome of <q>  to the state <s>
 */
void qx::qu_register::set_measurement(uint64_t q, bool m)
{
   assert(q<n_qubits);
   // binary[q] = s;
   measurement_register[q] = m;
}


/**
 * \brief getter
 * \return the state of bit <q> 
 */
state_t qx::qu_register::get_measurement_prediction(uint64_t q)
{
   assert(q<n_qubits);
   return measurement_prediction[q];
}


bool qx::qu_register::get_measurement(uint64_t q)
{
   assert(q<n_qubits);
   return measurement_register[q];
}


/**
 * \brief test bit <q> of the binary register
 * \return true if bit <q> is 1
 */
bool qx::qu_register::test(uint64_t q) // throw (qubit_not_measured_exception)  // trow exception if qubit value is unknown (never measured) !!!!
{
   assert(q<n_qubits);
   return (measurement_register[q]);
   // return (binary[q] == __state_1__);
}


/**
 * \brief
 */
void qx::qu_register::flip_binary(uint64_t q)
{
   assert(q<n_qubits);
   // state_t s = binary[q];
   state_t s = measurement_prediction[q];
   measurement_prediction[q] = (s != __state_unknown__ ? (s == __state_1__ ? __state_0__ : __state_1__) : s);  
   // binary[q] = (s != __state_unknown__ ? (s == __state_1__ ? __state_0__ : __state_1__) : s);  
}

/**
 * \brief
 */
void qx::qu_register::flip_measurement(uint64_t q)
{
   assert(q<n_qubits);
   measurement_register[q] = !measurement_register[q];
}




/**
 * fidelity
 */
double fidelity(qu_register& s1, qu_register& s2)
{
   if (s1.size() != s2.size())
   {
      println("[x] error : the specified registers have different sizes !");
      return -1;
   }

   double f = 0;  
   for (int i=0; i<s1.states(); ++i)
      // f += sqrt(std::norm(s1[i])*std::norm(s2[i]));
      f += sqrt(s1[i].norm()*s2[i].norm());
   
   return f;
}
