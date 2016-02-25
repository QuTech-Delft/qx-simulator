/**
 * @file		builder.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		21-10-15
 * @brief		
 */

#include <iostream>

#include <interface/circuit_builder.h>

int main(int argc, char **argv)
{
   qx::circuit_builder cb;             // create circuit builder

   cb.create_qubit(4);                 // create 4 qubits
   
   cb.add_gate(new qx::hadamard(2));   // add a 'hadamard' gate on qubit 2
   cb.add_gate(new qx::pauli_x (1));   // add a 'pauli_x'  gate on qubit 1
   cb.add_gate(new qx::cnot  (2,1));   // add a 'cnot' gate (control qubit = 2, target qubit = 1)

   cb.execute();                       // executes the circuit
   
   qx::bitset state = cb.get_state(1); // measure the qubit register

    
   return 0;
}
