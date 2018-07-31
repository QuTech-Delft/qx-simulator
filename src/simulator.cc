/**
 * @file	simulator.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date	01-01-16
 * @brief		
 */


#include <xpu.h>
#include <xpu/runtime>
#include <qx_representation.h>
#include <libqasm_interface.h>
#include <qasm_semantic.hpp>


#include <iostream>

#include "qx_version.h"

void print_banner() {
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
}

/**
 * simulator
 */
int main(int argc, char **argv)
{
  std::string file_path;
  size_t ncpu = 0;
  print_banner();

  if (!(argc == 2 || argc == 3))
  {
      println("error : you must specify a circuit file !");
      println("usage: \n   " << argv[0] << " file.qc");
      return -1;
  }
    
  // Parse arguments and initialise xpu cores
  file_path = argv[1];
  if (argc == 3) ncpu = (atoi(argv[2]));
  if (ncpu && ncpu < 128) xpu::init(ncpu);
  else xpu::init();

  // Parse file and create abstract syntax tree
  println("[+] loading circuit from '" << file_path << "' ...");
  FILE *qasm_file = fopen(file_path.c_str(), "r");
  if (!qasm_file)
  {
    std::cerr << "[x] Error: Could not open " << file_path << std::endl;
    xpu::clean();
    return -1;
  }

  // Construct libqasm parser and safely parse input file
  compiler::QasmSemanticChecker *parser;
  compiler::QasmRepresentation ast;
  try
  {
    parser = new compiler::QasmSemanticChecker(qasm_file);
    ast = parser->getQasmRepresentation();
  }
  catch (std::exception &e)
  {
    std::cerr << "Error while parsing file " << file_path << ": " << std::endl;
    std::cerr << e.what() << std::endl;
    xpu::clean();
    return -1;
  }

  
  // Create qx circuits and register
  std::vector<qx::circuit*> circuits;
  uint32_t qubits = ast.numQubits();
  println("[+] creating quantum register of " << qubits << " qubits... ");
  qx::qu_register *reg = NULL;

  try {
	   reg = new qx::qu_register(qubits);
  } catch(std::bad_alloc& exception) {
	   std::cerr << "[x] not enough memory, aborting" << std::endl;
	   xpu::clean();
	   return -1;
  } catch(std::exception& exception) {
	   std::cerr << "[x] unexpected exception (" << exception.what() << "), aborting" << std::endl;
	   xpu::clean();
	   return -1;
  }

  // Convert libqasm ast to qx internal representation
  qx::QxRepresentation qxr = qx::QxRepresentation(qubits);
  std::vector<compiler::SubCircuit> subcircuits
    = ast.getSubCircuits().getAllSubCircuits();
  ITER_FOR_IN(subcircuit, subcircuits)
  {
    try
    {
      circuits.push_back(qxCircuitFromCQasm(qubits, *subcircuit));
    }
    catch (std::string type)
    {
      std::cerr << "[x] Encountered unsuported gate: " << type << std::endl;
      xpu::clean();
      return -1;
    }
  }

  // Execute qx code
  uint32_t total_errors = 0;

  // check whether an error model is specified
  if (qxr.getErrorModel() == qx::__depolarizing_channel__)
  {
     println("[+] generating noisy circuits...");
     double error_probability = qxr.getErrorProbability(); 
     std::vector<qx::circuit*> perfect_circuits = qxr.circuits();
     for (uint32_t i=0; i<perfect_circuits.size(); i++)
     {
        println("[>] processing circuit '" << perfect_circuits[i]->id() << "'...");
  uint32_t iterations = perfect_circuits[i]->get_iterations();
  if (iterations > 1)
  {
     for (uint32_t it=0; it<iterations; ++it)
     {
        qx::depolarizing_channel dep_ch(perfect_circuits[i], qubits, error_probability);
        qx::circuit * noisy_c = dep_ch.inject(true);
        println("[+] noisy circuit : " << std::hex << noisy_c);
        circuits.push_back(noisy_c);
        total_errors += dep_ch.get_total_errors();
     }
  }
  else
  {
     qx::depolarizing_channel dep_ch(perfect_circuits[i], qubits, error_probability);
     qx::circuit * noisy_c = dep_ch.inject(true);
     circuits.push_back(noisy_c);
     total_errors += dep_ch.get_total_errors();
  }
     }
     println("[+] total errors injected in all circuits : " << total_errors);

  }
 
  for (uint32_t i=0; i<circuits.size(); i++)
     circuits[i]->execute(*reg);
 
  xpu::clean();

  return 0;
}
