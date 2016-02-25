/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		20-10-15
 * @brief		
 */
{
   uint32_t n=4;
   qx::qu_register reg(n);
   
   println("[+] initial state:");
   reg.dump();

   //qx::hadamard h(3);
   //h.apply(reg);
   
   for (int i=0; i<n; i++)
   {
	 qx::hadamard(i).apply(reg);
	 //qx::pauli_x(i).apply(reg);
	 //qx::pauli_y(i).apply(reg);
	 //qx::pauli_z(i).apply(reg);
   }

   println("[+] final state:");
   reg.dump();

}


