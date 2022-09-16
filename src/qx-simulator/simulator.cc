/**
 * @file	simulator.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date	01-01-16
 * @brief
 */

#include "qx/libqasm_interface.h"
#include "qx/representation.h"
#include <qasm_semantic.hpp>
#ifdef USE_GPERFTOOLS
#include <gperftools/profiler.h>
#endif

#include <iostream>
#include <memory>

#include "qx/version.h"

void print_banner() {
    // clang-format off
    println("");
    println("  =================================================================================================== "); 
    println("        _______                                                                                       ");
    println("       /  ___   \\   _  __      ____   ____   __  ___  __  __   __    ___  ______  ____    ___         ");
    println("      /  /   /  |  | |/_/     / __/  /  _/  /  |/  / / / / /  / /   / _ |/_  __/ / __ \\  / _ \\        ");
    println("     /  /___/  /  _>  <      _\\ \\   _/ /   / /|_/ / / /_/ /  / /__ / __ | / /   / /_/ / / , _/        ");
    println("     \\______/\\__\\ /_/|_|    /___/  /___/  /_/  /_/  \\____/  /____//_/ |_|/_/    \\____/ /_/|_|         ");
    println("                                                                                                      ");
    println("     version " , QX_VERSION , " - QuTech - " , QX_RELEASE_YEAR , " - report bugs and suggestions to: nader.khammassi@gmail.com");
    println("  =================================================================================================== ");
    println("");
    // clang-format on
}

/**
 * simulator
 */
int main(int argc, char **argv) {
    std::string file_path;
    size_t navg = 0;
    print_banner();

    if (!(argc == 2 || argc == 3 || argc == 4)) {
        println("error : you must specify a circuit file !");
        println("usage: \n   ", argv[0], " file.qc [iterations] [num_cpu]");
        return -1;
    }

    // parse arguments and initialise xpu cores
    file_path = argv[1];
    if (argc > 2)
        navg = (atoi(argv[2]));

    // size_t ncpu = 0;
    // if (argc > 3)
    //     ncpu = (atoi(argv[3]));
    // if (ncpu && ncpu < 128) xpu::init(ncpu);
    // else xpu::init();

    // parse file and create abstract syntax tree
    println("[+] loading circuit from '", file_path, "' ...");
    FILE *qasm_file = fopen(file_path.c_str(), "r");
    if (!qasm_file) {
        std::cerr << "[x] error: could not open " << file_path << std::endl;
        // xpu::clean();
        return -1;
    }

    compiler::QasmRepresentation ast;
    try {
        ast = compiler::QasmSemanticChecker(qasm_file).getQasmRepresentation();
    } catch (std::exception &e) {
        std::cerr << "error while parsing file " << file_path << ": "
                  << std::endl;
        std::cerr << e.what() << std::endl;
        // xpu::clean();
        return -1;
    }

    // quantum state and circuits
    size_t qubits = ast.numQubits();
    std::shared_ptr<qx::qu_register> reg;
    std::vector<std::shared_ptr<qx::circuit>> circuits;
    std::vector<std::shared_ptr<qx::circuit>> noisy_circuits;
    std::vector<std::shared_ptr<qx::circuit>> perfect_circuits;
    // error model parameters
    size_t total_errors = 0;
    double error_probability = 0;
    qx::error_model_t error_model = qx::__unknown_error_model__;

    // create the quantum state
    println("[+] creating quantum register of ", qubits, " qubits... ");
    try {
        reg = std::make_shared<qx::qu_register>(qubits);
    } catch (std::bad_alloc &exception) {
        std::cerr << "[x] not enough memory, aborting" << std::endl;
        // xpu::clean();
        return -1;
    } catch (std::exception &exception) {
        std::cerr << "[x] unexpected exception (" << exception.what()
                  << "), aborting" << std::endl;
        // xpu::clean();
        return -1;
    }

    // convert libqasm ast to qx internal representation
    // qx::QxRepresentation qxr = qx::QxRepresentation(qubits);
    std::vector<compiler::SubCircuit> subcircuits =
        ast.getSubCircuits().getAllSubCircuits();
    for (auto &subcircuit : subcircuits) {
        try {
            // qxr.circuits().push_back(load_cqasm_code(qubits, *subcircuit));
            perfect_circuits.push_back(load_cqasm_code(qubits, subcircuit));
        } catch (std::string type) {
            std::cerr << "[x] encountered unsuported gate: " << type
                      << std::endl;
            // xpu::clean();
            return -1;
        }
    }

    println("[i] loaded ", perfect_circuits.size(), " circuits.");

    // check whether an error model is specified
    if (ast.getErrorModelType() == "depolarizing_channel") {
        error_probability = ast.getErrorModelParameters().at(0);
        error_model = qx::__depolarizing_channel__;
    }

    // measurement averaging
    if (navg) {
#ifdef USE_GPERFTOOLS
        ProfilerStart("profile.log");
#endif
        if (error_model == qx::__depolarizing_channel__) {
            qx::measure m;
            for (size_t s = 0; s < navg; ++s) {
                reg->reset();
                for (auto perfect_circuit : perfect_circuits) {
                    if (perfect_circuit->size() == 0)
                        continue;
                    size_t iterations = perfect_circuit->get_iterations();
                    if (iterations > 1) {
                        for (size_t it = 0; it < iterations; ++it) {
                            qx::noisy_dep_ch(perfect_circuit, error_probability,
                                             total_errors)
                                ->execute(*reg, false, true);
                        }
                    } else
                        qx::noisy_dep_ch(perfect_circuit, error_probability,
                                         total_errors)
                            ->execute(*reg, false, true);
                }
                m.apply(*reg);
            }
        } else {
            qx::measure m;
            for (size_t s = 0; s < navg; ++s) {
                reg->reset();
                for (auto perfect_circuit : perfect_circuits) {
                    perfect_circuit->execute(*reg, false, true);
                }
                m.apply(*reg);
            }
        }
#ifdef USE_GPERFTOOLS
        ProfilerStop();
#endif
        println("[+] average measurement after ", navg, " shots:");
        reg->dump(true);
    } else {
        // if (qxr.getErrorModel() == qx::__depolarizing_channel__)
        if (error_model == qx::__depolarizing_channel__) {
            // println("[+] generating noisy circuits (p=" ,
            // qxr.getErrorProbability() , ")...");
            for (auto perfect_circuit : perfect_circuits) {
                if (perfect_circuit->size() == 0)
                    continue;
                // println("[>] processing circuit '" ,
                // perfect_circuit->id() , "'...");
                size_t iterations = perfect_circuit->get_iterations();
                if (iterations > 1) {
                    for (size_t it = 0; it < iterations; ++it)
                        circuits.push_back(qx::noisy_dep_ch(
                            perfect_circuit, error_probability, total_errors));
                } else {
                    circuits.push_back(qx::noisy_dep_ch(
                        perfect_circuit, error_probability, total_errors));
                }
            }
            // println("[+] total errors injected in all circuits : " ,
            // total_errors);
        } else {
            circuits = perfect_circuits;
        }

        for (auto circuit : circuits) {
            circuit->execute(*reg);
        }
    }

    // exit(0);
    // xpu::clean();

    return 0;
}
