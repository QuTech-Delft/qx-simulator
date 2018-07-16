
#include <iostream>

#include <core/register.h>
#include <core/circuit.h>

int main(int argc, char **argv)
{
   uint32_t n=3;
   qx::qu_register reg(n);
   qx::circuit qft(n); 

   // init
   qft.add(new qx::pauli_x(0));
   qft.add(new qx::pauli_x(1));
   qft.add(new qx::pauli_x(2));
   
   qft.add(new qx::display());

   // qft
   for (uint32_t i=0; i<n-1; ++i)
   {
      qft.add(new qx::hadamard(i));
      for (uint32_t j=i+1; j<n; ++j)
	 qft.add(new qx::ctrl_phase_shift(j,i));
   }
   qft.add(new qx::hadamard(n-1));

   qft.add(new qx::display());

   // reverse qubits
   for (uint32_t i=0; i<n/2; ++i)
      qft.add(new qx::swap(i,n-i-1));

   qft.add(new qx::display());

   // measure
   /*
   for (uint32_t i=0; i<n; ++i)
      qft.add(new qx::measure(i));

   qft.add(new qx::display());
   */

   qft.execute(reg);
   println("measure register");
   reg.measure();
   reg.dump();

   return 0;
}
