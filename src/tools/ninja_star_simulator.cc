/**
 * @file		ninja_star_simulator.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		21-12-15
 * @brief		
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <stdint.h>

#include <xpu.h>
#include <xpu/runtime>


#include <core/circuit.h>
#include <qcode/quantum_code_loader.h>

#include <qecc/ninja_star.h>

// #include <quantum_code/token.h>
// #include <quantum_code/syntax_error_exception.h>

using namespace str;

// #define println(x) std::cout << x << std::endl;
#define error(x) std::cout << "[x] error : " << x << std::endl;

#define __surface_code_17q_circuit__  "surface_code_17q_ninja_star.qc" 

#define MAX_QUBITS 32

// inject noise
qx::circuits_t inject_noise(qx::circuits_t& perfect_circuits, uint32_t qubits, double error_probability)
{
   qx::circuits_t circuits;

   uint32_t total_errors = 0;

   // check whether an error model is specified
   println("[+] generating noisy circuits...");
   for (uint32_t i=0; i<perfect_circuits.size(); i++)
   {
      println("[>] processing circuit '" << perfect_circuits[i]->id() << "'...");
      qx::depolarizing_channel dep_ch(perfect_circuits[i], qubits, error_probability);
      circuits.push_back(dep_ch.inject(false));
      total_errors += dep_ch.get_total_errors();
   }
   println("[+] total errors injected in all circuits : " << total_errors);
   return circuits;
}

// execute
void execute(qx::circuits_t& circuits, qx::qu_register& reg, uint32_t addr=0)
{
   assert(addr < circuits.size());
   for (uint32_t i=addr; i<circuits.size(); i++)
      circuits[i]->execute(reg);
}


 


/**
 * main
 */
int main(int agc, char ** argv)
{
   xpu::init();

   std::string report_file = "report_17q_ninja_star_under_noise.html";

   std::ofstream file;
   file.open (report_file);
   //file.open (report_file, ios::out | ios::app);
   file << "	<!DOCTYPE html>\n";
   file << "	   <html>\n";
   file << "	   <body>\n";
   file << "";

   qx::quantum_code_parser qcp(__surface_code_17q_circuit__);

   println("[+] loading circuit...");
   qcp.parse();
   qcp.dump();

   println("[+] creating quantum register of " << qcp.qubits() << " qubits... ");
   qx::qu_register reg(qcp.qubits());
   reg.dump(true);
  
   // initialization + 1 round
   qcp.execute(reg);

   // return 0;
  
   qx::qecc::ninja_star_17q ns17(&reg);
   /*
   ns17.report(file,"Initial state");

   qx::circuit bit_flip_d1(qcp.qubits(), "error_injection : bit-flip on D1");
   bit_flip_d1.add(new qx::pauli_x(1));

   qx::circuit bit_flip_d11(qcp.qubits(), "error_injection : bit-flip on D11");
   bit_flip_d11.add(new qx::pauli_x(7));

   qx::circuit phase_flip_d1(qcp.qubits(), "error_injection : phase-flip on D1");
   phase_flip_d1.add(new qx::pauli_z(1));
   */
   println("[+] executing circuit...");
   
   qcp.execute(reg,1);
   ns17.report(file,"First Round (No Errors) Error Injection");
   ns17.draw();

   const char * data_qubits_n[] = {"D1" , "D3" , "D5", 
                                   "D11", "D13", "D15",
		  		   "D21", "D23", "D25" };

   uint32_t     data_qubits_i[] = { 1   ,  2   ,  3  , 
                                    7   ,  8   ,  9  ,
		  		    13  ,  14  ,  15 };

   qx::circuits_t perfect_circuit = qcp.get_circuits();
   qx::circuits_t noisy_circuit;

   uint32_t simulations = 10000;
   double   error_probability = 0.00006; // correspond to an overall error of 1e-3
   //double   error_probability = 0.000006; // correspond to an overall error of 1e-4

   for (uint32_t i=0; i<simulations; ++i)
   {
      // reset register
      reg.reset();
      qcp.execute(reg);  // init to 0L

      ns17.update_history();

      noisy_circuit = inject_noise(perfect_circuit, reg.size(), error_probability);
      execute(noisy_circuit,reg,1);

      ns17.update_history();

      // correct
      ns17.correct(1);
      noisy_circuit = inject_noise(perfect_circuit, reg.size(), error_probability);
      execute(noisy_circuit,reg,1);

      ns17.update_history();

      ns17.correct(2);
      noisy_circuit = inject_noise(perfect_circuit, reg.size(), error_probability);
      execute(noisy_circuit,reg,1);
      
      ns17.update_history();

      ns17.correct(3);

      ns17.print_history();

      // check logical state
      if (!ns17.is_0_logical_state(reg))
	 println("[x] result : logical error !");
      else
	 println("[+] result : no error.");
      
      // ns17.draw();
      if (i%100 == 0) println("[+] progression : " << (i*100/simulations) << "% ...");
      // detect logical error here
   }


#if 0 

   for (uint32_t i=0; i<9; i++)
   {
      // reset register
      reg.reset();
      qcp.execute(reg);  // init to 0L
      
      // inject error
      qx::pauli_x(data_qubits_i[i]).apply(reg);
      qcp.execute(reg,1);
      //qx::measure(data_qubits_i[i]).apply(reg); // for coloring !
      std::stringstream comment;
      comment << "Error " << i << " : Bit-Flip on " << data_qubits_n[i];
      println(comment.str());
      ns17.draw();
      ns17.report(file,comment.str());
      // 3 round correction 
      //qx::pauli_x(data_qubits_i[i]).apply(reg); // manual correction
      ns17.correct_bitflips(1);
      qcp.execute(reg,1);
      ns17.correct_bitflips(2);
      qcp.execute(reg,1);
      ns17.correct_bitflips(3);
      qcp.execute(reg,1);
      ns17.report(file,"  --> Correction");
   }

#endif

   file << "	   </body>\n";
   file << "	   </html>\n";

   file.close();

   return 0;
  



#if 0  
   bit_flip_d1.execute(reg); // error injection
   
   qcp.execute(reg,1);
   ns17.report(file,"Second Round : After Error Injection (Bit-flip on qubit D1)");
   ns17.draw();
   
   bit_flip_d1.execute(reg); // correction
   
   qcp.execute(reg,1);
   ns17.report(file,"Third Round : After Error Correction on Qubit D1");
   ns17.draw();
   
   bit_flip_d11.execute(reg); // error injection 
   
   qcp.execute(reg,1);
   ns17.report(file,"Fourth Round : After Error Injection (Bit-flip on D11)");
   ns17.draw();
   
   bit_flip_d11.execute(reg); // correction
   
   qcp.execute(reg);
   ns17.report(file,"Fifth Round : After Error Correction on D11");
   ns17.draw();

   phase_flip_d1.execute(reg); // error injection 

   qcp.execute(reg,1);
   ns17.report(file,"Sixth Round : After Error Injection (Phase-flip on D1)");
   ns17.draw();

   phase_flip_d1.execute(reg); // correction 

   qcp.execute(reg,1);
   ns17.report(file,"Seventh Round : After Error Correction on D1");
   ns17.draw();

/**
   // toffoli gate test
   qx::qu_register tr(3);
   qx::circuit tt(3, "toffoli_test");
   tt.add(new qx::pauli_x(0));
   tt.add(new qx::pauli_x(1));
   tt.add(new qx::toffoli(0,1,2));
   tt.add(new qx::display());
   tt.execute(tr);
**/

   file << "	   </body>\n";
   file << "	   </html>\n";

   file.close();

   return 0;

#endif
}
