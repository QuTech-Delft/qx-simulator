/**
 * @file	simulator.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date	01-01-16
 * @brief		
 */

#include <iostream>

#include <xpu.h>
#include <xpu/runtime>

#include <core/circuit.h>
#include <qcode/quantum_code_loader.h>
#include <core/error_model.h>

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
   println("     version 0.1 beta - QuTech - 2016 - report bugs and suggestions to: nader.khammassi@gmail.com     ");
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
      if (ncpu)
	 xpu::init(ncpu);
      else
	 xpu::init();
   }
   else
      xpu::init();

   std::string file_name = argv[1];

   qx::quantum_code_parser qcp(file_name);

   println("[+] loading circuit from '" << file_name << "' ...");

   qcp.parse();
   //qcp.dump();


   println("[+] creating quantum register of " << qcp.qubits() << " qubits... ");
   qx::qu_register reg(qcp.qubits());


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
	 qx::depolarizing_channel dep_ch(perfect_circuits[i], qcp.qubits(), error_probability);
	 circuits.push_back(dep_ch.inject(true));
	 total_errors += dep_ch.get_total_errors();
      }
      println("[+] total errors injected in all circuits : " << total_errors);

   }
   else 
      circuits = qcp.get_circuits();
  
  
   // qcp.execute(reg);
   for (uint32_t i=0; i<circuits.size(); i++)
      circuits[i]->execute(reg);
  
   return 0;
}

