/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		20-10-15
 * @brief		
 */
{
   uint32_t n=2;
   qx::qu_register reg(n);
   
   println("[+] initial state:");
   reg.dump();

   qx::circuit c(n); 
   
   c.add(new qx::pauli_x (0));
   c.add(new qx::hadamard(0));
   c.add(new qx::hadamard(1));
   c.add(new qx::cnot(1,0));
   //c.add(new qx::hadamard(1));
   c.add(new qx::measure(1));

   c.execute(reg,true);
/*
   qx::pauli_x(0).apply(reg);
   reg.dump();
   qx::cnot(0,2).apply(reg);
*/
   println("[+] final state:");
   reg.dump();

}

