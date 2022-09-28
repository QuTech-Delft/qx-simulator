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

template <typename... Args> constexpr void printImpl(std::ostream& stream, Args... x) {
    int dummy[] = {0, (stream << x, 0)...};
    (void) dummy;
}

template <typename... Args> constexpr void log(Args... x) {
    std::cout << "[QXELERATOR] " << __FILE__ << ":" << __LINE__ << " ";

    printImpl(std::cout, x...);

    std::cout << std::endl;
}

template <typename... Args> constexpr void error(Args... x) {
    std::cerr << "[QXELERATOR] " << __FILE__ << ":" << __LINE__ << " Error: ";

    printImpl(std::cerr, x...);

    std::cerr << std::endl;
}

namespace qx {

/**
 * simulator
 */
class simulator {
protected:
    std::unique_ptr<qx::qu_register> reg;
    compiler::QasmRepresentation
        ast; // FIXME: this should be allocated on the heap, and should not be
             // copied from the parser object.

public:
    simulator() = default;

    bool set(std::string file_path) {
        FILE *qasm_file = fopen(file_path.c_str(), "r");
        if (!qasm_file) {
            error("Could not open ", file_path);
            return false;
        }

        try {
            ast = compiler::QasmSemanticChecker(qasm_file)
                      .getQasmRepresentation();
        } catch (std::exception &e) {
            error(e.what());
            error("Cannot parse file ", file_path);
            return false;
        }

        fclose(qasm_file);
        return true;
    }

    /**
     * execute qasm file
     */
    void execute(size_t number_of_runs) {
        // quantum state and circuits
        size_t qubits = ast.numQubits();
        std::vector<std::shared_ptr<qx::circuit>> circuits;
        std::vector<std::shared_ptr<qx::circuit>> perfect_circuits;

        // error model parameters
        size_t total_errors = 0;
        double error_probability = 0;
        qx::error_model_t error_model = qx::__unknown_error_model__;

        // create the quantum state
        log("Creating quantum register of ", qubits, " qubits...");
        try {
            reg = std::make_unique<qx::qu_register>(qubits);
        } catch (std::bad_alloc const& exception) {
            error("Not enough memory, aborting");
            throw exception;
        } catch (std::exception const& exception) {
            error("Unexpected exception (", exception.what(), "), aborting");
            throw exception;
        }

        // convert libqasm ast to qx internal representation
        auto const& subcircuits =
            ast.getSubCircuits().getAllSubCircuits();
        for (auto const& subcircuit : subcircuits) {
            try {
                perfect_circuits.push_back(load_cqasm_code(qubits, subcircuit));
            } catch (std::runtime_error const& exception) {
                error("Error while creating circuits: ", exception.what());
                throw exception;
            }
        }

        log("Loaded ", perfect_circuits.size(), " circuits.");

        // check whether an error model is specified
        if (ast.getErrorModelType() == "depolarizing_channel") {
            error_probability = ast.getErrorModelParameters().at(0);
            error_model = qx::__depolarizing_channel__;
        }

        // measurement averaging
        if (number_of_runs >= 2) {
            if (error_model == qx::__depolarizing_channel__) {
                qx::measure measure(CountMeasureInAverage::Yes);
                for (size_t s = 0; s < number_of_runs; ++s) {
                    reg->reset();
                    for (auto& perfect_circuit: perfect_circuits) {
                        if (perfect_circuit->size() == 0)
                        {
                            continue;
                        }
                        size_t iterations =
                            perfect_circuit->get_iterations();
                        assert(iterations >= 1);

                        for (size_t it = 0; it < iterations; ++it) {
                            qx::noisy_dep_ch(perfect_circuit,
                                                error_probability,
                                                total_errors)
                                ->execute(*reg, false, true);
                        }
                    }
                    measure.apply(*reg); // FIXME: does this work when the circuit already contains measure gates???
                }
            } else {
                qx::measure measure(CountMeasureInAverage::Yes);
                for (size_t s = 0; s < number_of_runs; ++s) {
                    reg->reset();
                    for (auto& perfect_circuit: perfect_circuits) {
                        perfect_circuit->execute(*reg, false, true);
                    }
                    measure.apply(*reg);
                }
            }

            log("Average measurement after ", number_of_runs, " shots:");
            reg->dump_measurement_averaging();
        } else {
            if (error_model == qx::__depolarizing_channel__) {
                for (auto& perfect_circuit: perfect_circuits) {
                    if (perfect_circuit->size() == 0) {
                        continue;
                    }

                    size_t iterations = perfect_circuit->get_iterations();
                    assert(iterations >= 1);
                    for (size_t it = 0; it < iterations; ++it)
                    {
                        circuits.push_back(qx::noisy_dep_ch(
                            perfect_circuit, error_probability,
                            total_errors));
                    }
                }
            } else {
                circuits = perfect_circuits;
            }

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
