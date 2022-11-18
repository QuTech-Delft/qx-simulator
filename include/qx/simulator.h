/**
 * @file    qx_simulator.h
 * @author	Nader Khammassi
 *          Imran Ashraf
 * @date	   23-12-16
 * @brief	qx simulator interface
 */

#pragma once

#include "qx/core/circuit.h"
#include "qx/libqasm_interface.h"
#include "qx/representation.h"
#include "qx/version.h"
#include "qx/core/simulation_result.h"
#include "cqasm.hpp"

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

namespace cq = ::cqasm::v1;
/**
 * simulator
 */
class simulator {
    std::string json_output_filename = "";

protected:
    std::unique_ptr<qx::qu_register> reg;
    cq::ast::One<cq::semantic::Program> program;

public:
    simulator() = default;

    void set_json_output_path(std::string filename) {
        json_output_filename = filename;
    }

    bool set(std::string file_path) {
        auto analyzer = cq::default_analyzer("1.2");

        try {
            program = analyzer.analyze(file_path).unwrap();
        } catch (const cq::analyzer::AnalysisFailed &e) {
            error("Cannot parse and analyze file ", file_path);
            return false;
        }

        if (program.empty()) {
            error("Cannot parse and analyze file ", file_path);
            return false;
        }

        return true;
    }

    /**
     * execute qasm file
     */
    void execute(size_t number_of_runs) {
        if (program.empty()) {
            error("No circuit loaded, call set(...) first");
            return;
        }

        // quantum state and circuits
        size_t qubits = program->num_qubits;
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
        auto const& subcircuits = program->subcircuits;
        for (auto const& subcircuit : subcircuits) {
            try {
                perfect_circuits.push_back(load_cqasm_code(qubits, *subcircuit));
            } catch (std::runtime_error const& exception) {
                error("Error while creating circuits: ", exception.what());
                throw exception;
            }
        }

        log("Loaded ", perfect_circuits.size(), " circuits.");

        // check whether an error model is specified
        if (!program->error_model.empty() && program->error_model->name == "depolarizing_channel") {
            error_probability = program->error_model->parameters[0]->as_const_real()->value;
            error_model = qx::__depolarizing_channel__;
        }

        // measurement averaging
        if (number_of_runs >= 1) {
            MeasurementAveraging measurementAveraging(qubits);

            if (error_model == qx::__depolarizing_channel__) {
                for (size_t s = 0; s < number_of_runs; ++s) {
                    reg->reset();
                    for (auto& perfect_circuit: perfect_circuits) {
                        if (perfect_circuit->size() == 0) {
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
                    measurementAveraging.append(reg->get_measurement_register());
                }
            } else {
                for (size_t s = 0; s < number_of_runs; ++s) {
                    reg->reset();
                    for (auto& perfect_circuit: perfect_circuits) {
                        perfect_circuit->execute(*reg, false, true);
                    }
                    measurementAveraging.append(reg->get_measurement_register());
                }
            }

            measurementAveraging.dump();
            if (json_output_filename != "") {
                measurementAveraging.dump_json(json_output_filename);
            }
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

            ExactQuantumState quantumState(*reg);
            quantumState.dump();
            if (json_output_filename != "") {
                quantumState.dump_json(json_output_filename);
            }
        }
    }

    bool get_measurement_outcome(size_t q) { return reg->get_measurement(q); }

    std::string get_state() { return reg->get_state(); }
};
} // namespace qx