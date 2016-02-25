/**
 * collapse state
 */
uint32_t qx::qu_register::collapse(uint32_t entry)
{
   for (uint32_t i=0; i<data.size(); ++i)
   {
      data[i] = 0;
   }
   data[entry] = 1;
   set_binary(entry,n_qubits);
   return entry;
}


/**
 * to binary
 */
void qx::qu_register::to_binary(uint32_t state, uint32_t nq)
{
   uint32_t k=0;
   while (nq--)
      std::cout << (((state >> nq) & 1) ? "1" : "0");
}


/**
 * set_binary
 */
void qx::qu_register::set_binary(uint32_t state, uint32_t nq)
{
   //uint32_t k=0;
   uint32_t k=nq-1;
   while (nq--)
   {
      binary[k--] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
      //binary[k++] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
   }
}


/**
 * \brief quantum register of n_qubit
 */
qx::qu_register::qu_register(uint32_t n_qubits) : data(1 << n_qubits), binary(n_qubits), n_qubits(n_qubits)
{
   data[0] = complex_t(1,0);
   for (uint32_t i=1; i<(1 << n_qubits); ++i)
      data[i] = 0;
   for (uint32_t i=0; i<n_qubits; i++)
      binary[i] = __state_0__;
}


/**
 * reset
 */
void qx::qu_register::reset()
{
   data[0] = complex_t(1,0);
   for (uint32_t i=1; i<(1 << n_qubits); ++i)
      data[i] = 0;
   for (uint32_t i=0; i<n_qubits; i++)
      binary[i] = __state_0__;
}


/**
 * \brief data getter
 */
cvector_t& qx::qu_register::get_data()
{
   return data;
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
uint32_t qx::qu_register::size()
{
   return n_qubits;
}


/**
 * \brief get states
 */
uint32_t qx::qu_register::states()
{
   return (1 << n_qubits);
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
complex_t& qx::qu_register::operator[](uint32_t i)
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
   for (int i=0; i<data.size(); ++i)
      sum += std::norm(data[i]);
   println("[+] register validity check : " << sum) ;
   return (std::fabs(sum-1) < QUBIT_ERROR_THRESHOLD);
}


/**
 * \brief measures one qubit
 */
int32_t qx::qu_register::measure()
{
#ifdef SAFE_MODE
   if (!check())
      return -1;
#endif // SAFE_MODE
   
   srand48(xpu::timer().current());
   double r = drand48();
   
   for (int i=0; i<data.size(); ++i)
   {
      r -= std::norm(data[i]);
      if (r <= 0)
      {
	 collapse(i);
	 return 1; 
      }
   }
   return -1;
}


/**
 * \brief dump
 */
void qx::qu_register::dump(bool only_binary=false)
{
   if (!only_binary)
   {
      println("--------------[quantum state]-------------- ");
      std::cout << std::fixed;
      for (int i=0; i<data.size(); ++i)
      {
	 if (data[i] != complex_t(0,0)) 
	 {
	    print("   " << std::fixed << data[i] << " |"); to_binary(i,n_qubits); println("> +");
	 }
      }
   }
   print("[>>] binary register:");
   print(" ");
   for (int i=binary.size()-1; i>=0; --i)
      print(" | " << __format_bin(binary[i]));  
   println(" |");
   println("------------------------------------------- ");
}


/**
 * set_binary
 */
void qx::qu_register::set_binary(uint32_t state)
{
   // print("  [-] set binary register to state : ");
   to_binary(state,n_qubits);
   uint32_t k=0;
   uint32_t nq = n_qubits;
   while (nq--)
   {
      binary[k++] = (((state >> nq) & 1) ? __state_1__ : __state_0__);
   }
}


/**
 * \brief setter
 * set bit <q>  to the state <s>
 */
void qx::qu_register::set_binary(uint32_t q, state_t s)
{
   assert(q<n_qubits);
   binary[q] = s;
}


/**
 * \brief getter
 * \return the state of bit <q> 
 */
state_t qx::qu_register::get_binary(uint32_t q)
{
   assert(q<n_qubits);
   return binary[q];
}


/**
 * \brief test bit <q> of the binary register
 * \return true if bit <q> is 1
 */
bool qx::qu_register::test(uint32_t q) // throw (qubit_not_measured_exception)  // trow exception if qubit value is unknown (never measured) !!!!
{
   assert(q<n_qubits);
   return (binary[q] == __state_1__);
}


/**
 * \brief
 */
void qx::qu_register::flip_binary(uint32_t q)
{
   assert(q<n_qubits);
   state_t s = binary[q];
   binary[q] = (s != __state_unknown__ ? (s == __state_1__ ? __state_0__ : __state_1__) : s);  
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
      f += sqrt(std::norm(s1[i])*std::norm(s2[i]));
   
   return f;
}

