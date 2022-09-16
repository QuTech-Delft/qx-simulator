/**
 * @file    qx_simulator.h
 * @author	Nader Khammassi
 *          Imran Ashraf
 * @date	   23-12-16
 * @brief	qx simulator interface
 */

#ifndef QX_SIMULATOR_H
#define QX_SIMULATOR_H

#include "qx/core/circuit.h"
#include "qx/libqasm_interface.h"
#include "qx/representation.h"
#include "qx/version.h"
#include <qasm_semantic.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

/**
 * i/o helpers
 */

template <typename... Args> constexpr void printImpl(Args... x) {
    (void)int[]{0, (std::cout << x, 0)...};
}

template <typename... Args> constexpr void println(Args... x) {
    std::cout << "[QXELERATOR]" << __FILE__ << ":" << __LINE__ << " ";

    printImpl(x...);

    std::cout << std::endl;
}

template <typename... Args> constexpr void error(Args... x) {
    std::cerr << "[QXELERATOR]" << __FILE__ << ":" << __LINE__ << " Error:";

    printImpl(x...);

    std::cout << std::endl;
}

namespace qx {

/**
 * simulator
 */
class simulator {
protected:
    std::unique_ptr<qx::qu_register> reg;
    compiler::QasmRepresentation ast;

public:
    simulator() : reg(nullptr) { /*xpu::init();*/
    }
    ~simulator() { /*xpu::clean();*/
    }

    void set(std::string file_path) {
        FILE *qasm_file = fopen(file_path.c_str(), "r");
        if (!qasm_file) {
            error("Could not open " << file_path);
        }

        // construct libqasm parser and safely parse input file
        compiler::QasmSemanticChecker *parser;
        try {
            parser = new compiler::QasmSemanticChecker(qasm_file);
            ast = parser->getQasmRepresentation();
        } catch (std::exception &e) {
            error("parsing file " << file_path);
            error(e.what());
        }
    }

    /**
     * execute qasm file
     */
    void execute(size_t navg) {
        // quantum state and circuits
        size_t qubits = ast.numQubits();
        std::vector<std::shared_ptr<qx::circuit>> circuits;
        std::vector<std::shared_ptr<qx::circuit>> noisy_circuits;
        std::vector<std::shared_ptr<qx::circuit>> perfect_circuits;

        // error model parameters
        size_t total_errors = 0;
        double error_probability = 0;
        qx::error_model_t error_model = qx::__unknown_error_model__;

        // create the quantum state
        println("Creating quantum register of ", qubits, " qubits... ");
        try {
            reg = std::make_unique<qx::qu_register>(qubits);
        } catch (std::bad_alloc &exception) {
            std::cerr << "Not enough memory, aborting" << std::endl;
            // xpu::clean();
        } catch (std::exception &exception) {
            std::cerr << "Unexpected exception (" << exception.what()
                      << "), aborting" << std::endl;
            // xpu::clean();
        }

        // convert libqasm ast to qx internal representation
        std::vector<compiler::SubCircuit> subcircuits =
            ast.getSubCircuits().getAllSubCircuits();
        for (auto subcircuit : subcircuits) {
            try {
                perfect_circuits.push_back(load_cqasm_code(qubits, subcircuit));
            } catch (std::string type) {
                std::cerr << "Encountered unsupported gate: " << type
                          << std::endl;
                // xpu::clean();
            }
        }

        println("Loaded ", perfect_circuits.size(), " circuits.");

        // check whether an error model is specified
        if (ast.getErrorModelType() == "depolarizing_channel") {
            error_probability = ast.getErrorModelParameters().at(0);
            error_model = qx::__depolarizing_channel__;
        }

        // measurement averaging
        if (navg) {
            if (error_model == qx::__depolarizing_channel__) {
                qx::measure m;
                for (size_t s = 0; s < navg; ++s) {
                    reg->reset();
                    for (size_t i = 0; i < perfect_circuits.size(); i++) {
                        if (perfect_circuits[i]->size() == 0)
                            continue;
                        size_t iterations =
                            perfect_circuits[i]->get_iterations();
                        if (iterations > 1) {
                            for (size_t it = 0; it < iterations; ++it) {
                                qx::noisy_dep_ch(perfect_circuits[i],
                                                 error_probability,
                                                 total_errors)
                                    ->execute(*reg, false, true);
                            }
                        } else
                            qx::noisy_dep_ch(perfect_circuits[i],
                                             error_probability, total_errors)
                                ->execute(*reg, false, true);
                    }
                    m.apply(*reg);
                }
            } else {
                qx::measure m;
                for (size_t s = 0; s < navg; ++s) {
                    reg->reset();
                    for (size_t i = 0; i < perfect_circuits.size(); i++)
                        perfect_circuits[i]->execute(*reg, false, true);
                    m.apply(*reg);
                }
            }

            println("Average measurement after ", navg, " shots:");
            reg->dump(true);
        } else {
            if (error_model == qx::__depolarizing_channel__) {
                // println("[+] generating noisy circuits (p=",
                // qxr.getErrorProbability(), ")...");
                for (size_t i = 0; i < perfect_circuits.size(); i++) {
                    if (perfect_circuits[i]->size() == 0)
                        continue;
                    // println("[>] processing circuit '",
                    // perfect_circuits[i]->id(), "'...");
                    size_t iterations = perfect_circuits[i]->get_iterations();
                    if (iterations > 1) {
                        for (size_t it = 0; it < iterations; ++it)
                            circuits.push_back(qx::noisy_dep_ch(
                                perfect_circuits[i], error_probability,
                                total_errors));
                    } else {
                        circuits.push_back(qx::noisy_dep_ch(perfect_circuits[i],
                                                            error_probability,
                                                            total_errors));
                    }
                }
                // println("[+] total errors injected in all circuits : ",
                // total_errors);
            } else
                circuits = perfect_circuits; // qxr.circuits();

            for (size_t i = 0; i < circuits.size(); i++) {
                circuits[i]->execute(*reg);
            }
        }
    }

    bool move(size_t q) { return reg->get_measurement(q); }

    std::string get_state() { return reg->get_state(); }
};
} // namespace qx

#endif // QX_SIMULATOR_H
