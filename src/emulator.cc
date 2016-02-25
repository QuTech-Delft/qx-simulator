/**
 * @file		emulator.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		02-10-15
 * @brief		main file
 */

#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <cstring>
#include <ctime>

#include <cmath>

#include <core/register.h>
#include <core/circuit.h>
#include <core/circuit_loader.h>

#include <core/gaussian.h>

#include <core/memory_monitor.h>

#define println(x) std::cout << x << std::endl
#define print(x) std::cout << x 

using namespace qx;
using namespace qx::linalg;

/**
 * main
 */
int main(int argc, char **argv)
{ 
   // #include "tests/noisy_gate_test.cc"  
   // #include "tests/precision_loss_test.cc"
   #include "tests/precision_loss_experiment.cc"
   // #include "tests/first_test.cc"
   // #include "tests/gate_test.cc"
   // #include "tests/circuit_test.cc"
   // #include "tests/hadamard_test.cc"
   // #include "tests/error_correction_3b_code.cc"

   // #include "tests/stress_test.cc"
   // println("[!] current resident set size (phys. mem.) : " << (get_current_rss()/1024000) << " M");
   // println("[!] peak resident set size (phys. mem.)    : " << (get_peak_rss()/1024000)    << " M");
    
   return 0;
}
