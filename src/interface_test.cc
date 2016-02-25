/**
 * @file       interface_test.cc
 * @author     Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date       26-10-15
 * @brief      interface tester 
 */

#include <iostream>

#include <cstdlib>
#include <cstring>
#include <ctime>

#include <interface/quantum_core.h>

#define println(x) std::cout << x << std::endl
#define print(x) std::cout << x 

int main(int argc, char **argv)
{
   qx::quantum_core qc;  // quantum core interface
   qx::gate_factory gf;  // for gate creation

   qc.create_qubits(2);  // create 2 qubits

   qc.print_state();     // display the initial state

   qx::circuit_t c;      // define an empty circuit/sub-circuit

   c.push_back(gf.pauli_x (0));   // add gate to the circuit
   c.push_back(gf.hadamard(0));   // ...
   c.push_back(gf.hadamard(1));
   c.push_back(gf.cnot(1,0));
   c.push_back(gf.hadamard(1));

   qc.add_circuit(c);    // send circuit to the quantum core
   qc.execute();         // execute the circuit

   qc.print_state();     // print the final state

   return 0;
}

