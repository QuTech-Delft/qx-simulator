/**
 * @file	simulator.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date	01-01-16
 * @brief		
 */

#include <iostream>

//#include <xpu.h>
//#include <xpu/runtime>

#include "qx/core/circuit.h"
#include "qx/qcode/quantum_code_loader.h"
#include "qx/core/error_model.h"
#include "qx/version.h"

/**
 * simulator
 */
int main(int argc, char **argv)
{
   println("");
   println("  =================================================================================================== "); 
   println("        _______                                                                                       ");
   println("       /  ___   \\   _  __      ____   ____   __  ___  __  __   __    ___  ______  ____    ___         ");
   println("      /  /   /  |  | |/_/     / __/  /  _/  /  |/  / / / / /  / /   / _ |/_  __/ / __ \\  / _ \\        ");
   println("     /  /___/  /  _>  <      _\\ \\   _/ /   / /|_/ / / /_/ /  / /__ / __ | / /   / /_/ / / , _/        ");
   println("     \\______/\\__\\ /_/|_|    /___/  /___/  /_/  /_/  \\____/  /____//_/ |_|/_/    \\____/ /_/|_|         ");
   println("                                                                                                      ");
   println("     version " << QX_VERSION << " - QuTech - " << QX_RELEASE_YEAR << " - report bugs and suggestions to: nader.khammassi@gmail.com");
   println("  =================================================================================================== ");
   println("");

   if ((argc != 2) && (argc !=3) )
   {
      println("error : you must specify a circuit file !");
      println("usage: \n   " << argv[0] << " file.qc");
      return -1;
   }

   if (argc == 3)
   {
      size_t ncpu = atoi(argv[2]);
      ncpu = ((ncpu < 128) ? ncpu : 0);
     /* if (ncpu)
	 xpu::init(ncpu);
      else
	 xpu::init();*/
   }
   /*else
      xpu::init();*/

   std::string file_name = argv[1];

   qx::quantum_code_parser qcp(file_name);

   println("[+] loading circuit from '" << file_name << "' ...");

   qcp.parse();
   //qcp.dump();

   qx::qu_register *reg = NULL;

   println("[+] creating quantum register of " << qcp.qubits() << " qubits... ");
   try {
	   reg = new qx::qu_register(qcp.qubits());
   } catch(std::bad_alloc& exception) {
	   println("[!] not enough memory, aborting");
	   //xpu::clean();
	   return -1;
   } catch(std::exception& exception) {
	   println("[!] unexpected exception (" << exception.what() << "), aborting");
	   //xpu::clean();
	   return -1;
   }

   qx::circuits_t circuits;

   uint32_t total_errors = 0;

   // check whether an error model is specified
   if (qcp.get_error_model() == qx::__depolarizing_channel__)
   {
      println("[+] generating noisy circuits...");
      double error_probability = qcp.get_error_probability(); 
      qx::circuits_t perfect_circuits = qcp.get_circuits();
      for (uint32_t i=0; i<perfect_circuits.size(); i++)
      {
         println("[>] processing circuit '" << perfect_circuits[i]->id() << "'...");
	 uint32_t iterations = perfect_circuits[i]->get_iterations();
	 if (iterations > 1)
	 {
	    for (uint32_t it=0; it<iterations; ++it)
	    {
	       qx::depolarizing_channel dep_ch(perfect_circuits[i], qcp.qubits(), error_probability);
	       qx::circuit * noisy_c = dep_ch.inject(true);
	       println("[+] noisy circuit : " << std::hex << noisy_c);
	       circuits.push_back(noisy_c);
	       total_errors += dep_ch.get_total_errors();
	    }
	 }
	 else
	 {
	    qx::depolarizing_channel dep_ch(perfect_circuits[i], qcp.qubits(), error_probability);
	    qx::circuit * noisy_c = dep_ch.inject(true);
	    circuits.push_back(noisy_c);
	    total_errors += dep_ch.get_total_errors();
	 }
      }
      println("[+] total errors injected in all circuits : " << total_errors);

   }
   else 
      circuits = qcp.get_circuits();
  
  
   // qcp.execute(reg);
   for (uint32_t i=0; i<circuits.size(); i++)
      circuits[i]->execute(*reg);
  
   //xpu::clean();

   return 0;
}

