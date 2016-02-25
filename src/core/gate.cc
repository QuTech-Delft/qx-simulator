/**
 * @file		gate.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		23-10-15
 * @brief		
 */


inline void qx::core::sqg_apply(cmatrix_t &m, uint32_t qubit, qu_register& qureg)
{
   uint32_t n  = qureg.size();

   if (qubit == 0)
   {
	 cidentity_t i(1 << (n-1));
	 qureg = mxv(tensor(i,m), qureg.get_data());
   }
   else if (qubit == n-1)
   {
	 cidentity_t i(1 << (n-1));
	 qureg = mxv(tensor(m,i), qureg.get_data());
   }
   else
   {
	 cidentity_t i1(1 << (qubit));
	 cidentity_t i2(1 << (n-qubit-1));
	 qureg = mxv(tensor(tensor(i2,m),i1), qureg.get_data());
   }
}


