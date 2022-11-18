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
#include <core/density_operator.h>
#include <core/error_model.h>
#include <qcode/quantum_code_loader.h>

#define __hadamard_8q_bench__ "hadamard_8q_bench.qc"   // hadamard on 8  qubits
#define __hadamard_16q_bench__ "hadamard_16q_bench.qc" // hadamard on 16 qubits
#define __hadamard_32q_bench__ "hadamard_24q_bench.qc" // hadamard on 32 qubits

#define __pauli_x_8q_bench__ "pauli_x_8q_bench.qc"   // pauli_x on 8  qubits
#define __pauli_x_16q_bench__ "pauli_x_16q_bench.qc" // pauli_x on 16 qubits
#define __pauli_x_32q_bench__ "pauli_x_24q_bench.qc" // pauli_x on 32 qubits

#define __pauli_x_8q_bench__ "pauli_x_8q_bench.qc"   // pauli_x on 8  qubits
#define __pauli_y_16q_bench__ "pauli_y_16q_bench.qc" // pauli_y on 16 qubits
#define __pauli_y_32q_bench__ "pauli_y_24q_bench.qc" // pauli_y on 32 qubits

#define __pauli_z_8q_bench__ "pauli_z_8q_bench.qc"   // pauli_z on 8  qubits
#define __pauli_z_16q_bench__ "pauli_z_16q_bench.qc" // pauli_z on 16 qubits
#define __pauli_z_32q_bench__ "pauli_z_24q_bench.qc" // pauli_z on 32 qubits

#define __rx_8q_bench__ "rx_8q_bench.qc"   // rx on 8  qubits
#define __rx_16q_bench__ "rx_16q_bench.qc" // rx on 16 qubits
#define __rx_32q_bench__ "rx_24q_bench.qc" // rx on 32 qubits

#define __prep_z_8q_bench__ "prep_z_8q_bench.qc"   // prep_z on 8  qubits
#define __prep_z_16q_bench__ "prep_z_16q_bench.qc" // prep_z on 16 qubits
#define __prep_z_32q_bench__ "prep_z_24q_bench.qc" // prep_z on 32 qubits

#define __toffoli_8q_bench__ "toffoli_8q_bench.qc"   // toffoli on 8  qubits
#define __toffoli_16q_bench__ "toffoli_16q_bench.qc" // toffoli on 16 qubits
#define __toffoli_32q_bench__ "toffoli_24q_bench.qc" // toffoli on 32 qubits

#define __cnot_8q_bench__ "cnot_8q_bench.qc"   // cnot on 8  qubits
#define __cnot_16q_bench__ "cnot_16q_bench.qc" // cnot on 16 qubits
#define __cnot_32q_bench__ "cnot_24q_bench.qc" // cnot on 32 qubits

#define __measure_8q_bench__ "measure_8q_bench.qc"   // measure on 8  qubits
#define __measure_16q_bench__ "measure_16q_bench.qc" // measure on 16 qubits
#define __measure_32q_bench__ "measure_24q_bench.qc" // measure on 32 qubits

#define __measure_reg_8q_bench__                                               \
    "measure_reg_8q_bench.qc" // measure_reg on 8  qubits
#define __measure_reg_16q_bench__                                              \
    "measure_reg_16q_bench.qc" // measure_reg on 16 qubits
#define __measure_reg_32q_bench__                                              \
    "measure_reg_24q_bench.qc" // measure_reg on 32 qubits

#define __qft_8q_bench__ "qft_8q_bench.qc"   // qft on 8  qubits
#define __qft_16q_bench__ "qft_16q_bench.qc" // qft on 16 qubits
#define __qft_32q_bench__ "qft_24q_bench.qc" // qft on 32 qubits

/**
 * simulator
 */
int main(int argc, char **argv) {
    // clang-format off
    println("");
    println("  ======================================================================================================="); 
    println("       _______                                                                                       ");
    println("      /  ___   \\   _  __      ____   ____   __  ___  __  __   __    ___   ______  ____    ___         ");
    println("     /  /   /  |  | |/_/     / __/  /  _/  /  |/  / / / / /  / /   / _ | /_  __/ / __ \\  / _ \\        ");
    println("    /  /___/  /  _>  <      _\\ \\   _/ /   / /|_/ / / /_/ /  / /__ / __ |  / /   / /_/ / / , _/        ");
    println("    \\______/\\__\\ /_/|_|    /___/  /___/  /_/  /_/  \\____/  /____//_/ |_| /_/    \\____/ /_/|_|         ");
    println("                                                                                              [v0.1 beta]");
    println("  _______________________________________________________________________________________________________");
    println("  [version 0.1 beta - Nader Khammassi - TU Delft, QuTech - 2016 - report bugs to: n.khammassi@tudelft.nl]");
    println("  [released under Apache License 2.0 terms, license copy at:  http://www.apache.org/licenses/LICENSE-2.0]");
    println("  ======================================================================================================= ");
    println("");
    // clang-format off
    if (argc != 2) {
        println("error : you must specify a circuit file !");
        println("usage: \n   ", argv[0], " file.qc");
        return -1;
    }

    std::string file_name = argv[1];

    qx::quantum_code_parser qcp(file_name);

    println("[+] loading circuit from '", file_name, "' ...");

    qcp.parse();
    // qcp.dump();

    xpu::init();

    println("[+] creating quantum register of ", qcp.qubits(), " qubits... ");
    qx::qu_register reg(qcp.qubits());

    qx::circuits_t circuits;

    uint32_t total_errors = 0;

    // check whether an error model is specified
    if (qcp.get_error_model() == qx::__depolarizing_channel__) {
        println("[+] generating noisy circuits...");
        double error_probability = qcp.get_error_probability();
        double error_probability = qcp.get_error_probability();
        double error_probability = qcp.get_error_probability();
        qx::circuits_t perfect_circuits = qcp.get_circuits();
        for (uint32_t i = 0; i < perfect_circuits.size(); i++) {
            println("[>] processing circuit '", perfect_circuits[i]->id(),
                    "'...");
            qx::depolarizing_channel dep_ch(perfect_circuits[i], qcp.qubits(),
                                            error_probability);
            circuits.push_back(dep_ch.inject(true));
            total_errors += dep_ch.get_total_errors();
        }
        println("[+] total errors injected in all circuits : ", total_errors);

    } else
        circuits = qcp.get_circuits();

    // qcp.execute(reg);
    for (uint32_t i = 0; i < circuits.size(); i++)
        circuits[i]->execute(reg);

    /*
       // print density matrix
       if (qcp.qubits() < 8)
       {
          qx::qu_register env(qcp.qubits());
          qx::circuits_t perfect_circuits = qcp.get_circuits();
          for (uint32_t i=0; i<perfect_circuits.size(); i++)
             perfect_circuits[i]->execute(env);
          qx::density_operator dop;
          dop.add_state(&reg,0.9f);
          dop.add_state(&env,0.1f);
          qx::linalg::cmatrix_t * dm = dop.density_matrix();
          dm->dump();
          println("[+] purity : " , dop.purity());
          delete dm;
       }
    */

    return 0;
}
