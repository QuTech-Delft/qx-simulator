#ifndef LIBQASM_INTERFACE_H
#define LIBQASM_INTERFACE_H

#include "qx/compat.h"
#include "qx/core/circuit.h"
#include <qasm_ast.hpp>

#define __ret_gate_1(__g)                                                      \
    {                                                                          \
        if (!pg)                                                               \
            if (!bc)                                                           \
                return std::make_shared<__g>(sqid(operation));                 \
            else                                                               \
                return std::make_shared<qx::bin_ctrl>(                         \
                    bv, std::make_shared<__g>(sqid(operation)));               \
        else {                                                                 \
            if (!bc) {                                                         \
                for (auto q : qv)                                              \
                    pg->add(std::make_shared<__g>(q));                         \
            } else {                                                           \
                for (auto q : qv)                                              \
                    pg->add(std::make_shared<qx::bin_ctrl>(                    \
                        bv, std::make_shared<__g>(q)));                        \
            }                                                                  \
            return std::move(pg);                                              \
        }                                                                      \
    }

#define __ret_bin_gate(__g)                                                    \
    {                                                                          \
        if (!pg && (bv.size() > 1))                                            \
            pg = std::make_shared<qx::parallel_gates>();                       \
        if (!pg)                                                               \
            return std::make_shared<__g>(bid(operation));                      \
        else {                                                                 \
            for (auto b : bv)                                                  \
                pg->add(std::make_shared<__g>(b));                             \
            return pg;                                                         \
        }                                                                      \
    }

#define __ret_gate_2(__g)                                                      \
    {                                                                          \
        const std::vector<size_t> &qv0 =                                       \
            operation.getQubitsInvolved(1).getSelectedQubits().getIndices();   \
        const std::vector<size_t> &qv1 =                                       \
            operation.getQubitsInvolved(2).getSelectedQubits().getIndices();   \
                                                                               \
        if (qv0.size() != qv1.size())                                          \
            throw("[x] error : parallel " + type +                             \
                  " args have different sizes !");                             \
                                                                               \
        if (qv0.size() == 1) {                                                 \
            if (!bc)                                                           \
                return std::make_shared<__g>(qv0[0], qv1[0]);                  \
            else                                                               \
                return std::make_shared<qx::bin_ctrl>(                         \
                    bv, std::make_shared<__g>(qv0[0], qv1[0]));                \
        } else {                                                               \
            pg = std::make_shared<qx::parallel_gates>();                       \
            if (!bc) {                                                         \
                for (size_t i = 0; i < qv0.size(); ++i)                        \
                    pg->add(std::make_shared<__g>(qv0[i], qv1[i]));            \
            } else {                                                           \
                for (size_t i = 0; i < qv0.size(); ++i)                        \
                    pg->add(std::make_shared<qx::bin_ctrl>(                    \
                        bv, std::make_shared<__g>(qv0[i], qv1[i])));           \
            }                                                                  \
            return pg;                                                         \
        }                                                                      \
    }

int sqid(compiler::Operation &operation) {
    return operation.getQubitsInvolved().getSelectedQubits().getIndices()[0];
}

int qid(compiler::Operation &operation, int id) {
    return operation.getQubitsInvolved(id).getSelectedQubits().getIndices()[0];
}

int bid(compiler::Operation &operation) {
    return operation.getControlBits().getSelectedBits().getIndices()[0];
}

std::shared_ptr<qx::gate> gateLookup(compiler::Operation &operation) {
    // operation.printOperation();
    bool bc = false;
    const std::vector<size_t> &qv =
        operation.getQubitsInvolved().getSelectedQubits().getIndices();
    const std::vector<size_t> &bv =
        operation.getControlBits().getSelectedBits().getIndices();
    std::shared_ptr<qx::parallel_gates> pg;

    if (bv.size())
        bc = true;
    if (qv.size() > 1)
        pg = std::make_shared<qx::parallel_gates>();

    // if (bc) { std::cout << operation.getControlBits().printMembers(); }

    std::string type = operation.getType();

    if (type == "toffoli") {
        const std::vector<size_t> &qv0 =
            operation.getQubitsInvolved(1).getSelectedQubits().getIndices();
        const std::vector<size_t> &qv1 =
            operation.getQubitsInvolved(2).getSelectedQubits().getIndices();
        const std::vector<size_t> &qv2 =
            operation.getQubitsInvolved(3).getSelectedQubits().getIndices();

        if ((qv0.size() != qv1.size()) || (qv0.size() != qv2.size()))
            throw("[x] error : parallel toffoli args have different sizes !");

        if (qv0.size() == 1) {
            if (!bc)
                return std::make_shared<qx::toffoli>(qv0[0], qv1[0], qv2[0]);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::toffoli>(qv0[0], qv1[0], qv2[0]));
        } else {
            pg = std::make_shared<qx::parallel_gates>();
            if (!bc)
                for (size_t i = 0; i < qv0.size(); ++i)
                    pg->add(
                        std::make_shared<qx::toffoli>(qv0[i], qv1[i], qv2[i]));
            else
                for (size_t i = 0; i < qv0.size(); ++i)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv,
                        std::make_shared<qx::toffoli>(qv0[i], qv1[i], qv2[i])));
            return std::move(pg);
        }
    }

    ///////// common sq gates //////
    if (type == "i")
        __ret_gate_1(qx::identity);
    if (type == "x")
        __ret_gate_1(qx::pauli_x);
    if (type == "y")
        __ret_gate_1(qx::pauli_y);
    if (type == "z")
        __ret_gate_1(qx::pauli_z);
    if (type == "h")
        __ret_gate_1(qx::hadamard);
    if (type == "s")
        __ret_gate_1(qx::phase_shift);
    if (type == "sdag")
        __ret_gate_1(qx::s_dag_gate);
    if (type == "t")
        __ret_gate_1(qx::t_gate);
    if (type == "tdag")
        __ret_gate_1(qx::t_dag_gate);

    /////////// classical /////////

    if (type == "not")
        __ret_bin_gate(qx::classical_not);

    /////////// rotations /////////
    if (type == "rx") {
        double angle = operation.getRotationAngle();
        if (!pg)
            if (!bc)
                return std::make_shared<qx::rx>(sqid(operation), angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::rx>(sqid(operation), angle));
        else {
            if (!bc)
                for (auto q : qv)
                    pg->add(std::make_shared<qx::rx>(q, angle));
            else
                for (auto q : qv)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::rx>(q, angle)));
            return pg;
        }
    }
    if (type == "ry") {
        double angle = operation.getRotationAngle();
        if (!pg)
            if (!bc)
                return std::make_shared<qx::ry>(sqid(operation), angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::ry>(sqid(operation), angle));
        else {
            if (!bc)
                for (auto q : qv)
                    pg->add(std::make_shared<qx::ry>(q, angle));
            else
                for (auto q : qv)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::ry>(q, angle)));
            return pg;
        }
    }
    if (type == "rz") {
        double angle = operation.getRotationAngle();
        if (!pg)
            if (!bc)
                return std::make_shared<qx::rz>(sqid(operation), angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::rz>(sqid(operation), angle));
        else {
            if (!bc)
                for (auto q : qv)
                    pg->add(std::make_shared<qx::rz>(q, angle));
            else
                for (auto q : qv)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::rz>(q, angle)));
            return pg;
        }
    }

    //////////// two qubits gates //////////////
    if (type == "cnot")
        __ret_gate_2(qx::cnot);
    if (type == "cz")
        __ret_gate_2(qx::cphase);
    if (type == "swap")
        __ret_gate_2(qx::swap);

    ///////////// prep gates //////////////////
    if (type == "prep_z")
        __ret_gate_1(qx::prepz);
    if (type == "prep_y")
        __ret_gate_1(qx::prepy);
    if (type == "prep_x")
        __ret_gate_1(qx::prepx);

    ////////// measurements //////////////////
    if (type == "measure" || type == "measure_z") {
        if (!pg) {
            return std::make_shared<qx::measure>(sqid(operation));
        }
        else {
            for (auto q : qv) {
                pg->add(std::make_shared<qx::measure>(q));
            }
            return pg;
        }
    }
    if (type == "measure_all") {
        return std::make_shared<qx::measure>();
    }

    if (type == "measure_x") {
        if (!pg) {
            return std::make_shared<qx::measure_x>(sqid(operation));
        }
        else {
            for (auto q : qv) {
                pg->add(std::make_shared<qx::measure_x>(q));
            }
            return pg;
        }
    }
    if (type == "measure_y") {
        if (!pg) {
            return std::make_shared<qx::measure_y>(sqid(operation));
        }
        else {
            for (auto q : qv) {
                pg->add(std::make_shared<qx::measure_y>(q));
            }
            return pg;
        }
    }

    ////////////// display /////////////////
    if (type == "display")
        return std::make_shared<qx::display>();
    if (type == "display_binary")
        return std::make_shared<qx::display>(true);

    /////////////// x90 //////////////////
    if (type == "x90") {
        double angle = QX_PI / 2;
        if (!pg)
            if (!bc)
                return std::make_shared<qx::rx>(sqid(operation), angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::rx>(sqid(operation), angle));
        else {
            if (!bc)
                for (auto q : qv)
                    pg->add(std::make_shared<qx::rx>(q, angle));
            else
                for (auto q : qv)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::rx>(q, angle)));
            return pg;
        }
    }
    if (type == "mx90") {
        double angle = -QX_PI / 2;
        if (!pg)
            if (!bc)
                return std::make_shared<qx::rx>(sqid(operation), angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::rx>(sqid(operation), angle));
        else {
            if (!bc)
                for (auto q : qv)
                    pg->add(std::make_shared<qx::rx>(q, angle));
            else
                for (auto q : qv)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::rx>(q, angle)));
            return pg;
        }
    }
    if (type == "y90") {
        double angle = QX_PI / 2;
        if (!pg)
            if (!bc)
                return std::make_shared<qx::ry>(sqid(operation), angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::ry>(sqid(operation), angle));
        else {
            if (!bc)
                for (auto q : qv)
                    pg->add(std::make_shared<qx::ry>(q, angle));
            else
                for (auto q : qv)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::ry>(q, angle)));
            return pg;
        }
    }
    if (type == "my90") {
        double angle = -QX_PI / 2;
        if (!pg)
            if (!bc)
                return std::make_shared<qx::ry>(sqid(operation), angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::ry>(sqid(operation), angle));
        else {
            if (!bc)
                for (auto q : qv)
                    pg->add(std::make_shared<qx::ry>(q, angle));
            else
                for (auto q : qv)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::ry>(q, angle)));
            return pg;
        }
    }
    if (type == "c-x")
        return std::make_shared<qx::bin_ctrl>(
            bid(operation), std::make_shared<qx::pauli_x>(sqid(operation)));
    if (type == "c-z")
        return std::make_shared<qx::bin_ctrl>(
            bid(operation), std::make_shared<qx::pauli_z>(sqid(operation)));

    if (type == "cr") {
        double angle = operation.getRotationAngle();
        const std::vector<size_t> &qv0 =
            operation.getQubitsInvolved(1).getSelectedQubits().getIndices();
        const std::vector<size_t> &qv1 =
            operation.getQubitsInvolved(2).getSelectedQubits().getIndices();

        if (qv0.size() != qv1.size())
            throw("[x] error : parallel 'cr' args have different sizes !");

        if (qv0.size() == 1) {
            if (!bc)
                return std::make_shared<qx::ctrl_phase_shift>(qv0[0], qv1[0],
                                                              angle);
            else
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::ctrl_phase_shift>(qv0[0], qv1[0],
                                                               angle));
        } else {
            pg = std::make_shared<qx::parallel_gates>();
            if (!bc)
                for (size_t i = 0; i < qv0.size(); ++i)
                    pg->add(std::make_shared<qx::ctrl_phase_shift>(
                        qv0[i], qv1[i], angle));
            else
                for (size_t i = 0; i < qv0.size(); ++i)
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::ctrl_phase_shift>(
                                qv0[i], qv1[i], angle)));
            return pg;
        }
    }
    if (type == "crk") {
        size_t angle = operation.getRotationAngle();
        const std::vector<size_t> &qv0 =
            operation.getQubitsInvolved(1).getSelectedQubits().getIndices();
        const std::vector<size_t> &qv1 =
            operation.getQubitsInvolved(2).getSelectedQubits().getIndices();

        if (qv0.size() != qv1.size())
            throw("[x] error : parallel 'crk' args have different sizes !");

        if (qv0.size() == 1) {
            if (!bc) {
                return std::make_shared<qx::ctrl_phase_shift>(qv0[0], qv1[0],
                                                              angle);
            } else {
                return std::make_shared<qx::bin_ctrl>(
                    bv, std::make_shared<qx::ctrl_phase_shift>(qv0[0], qv1[0],
                                                               angle));
            }
        } else {
            pg = std::make_shared<qx::parallel_gates>();
            if (!bc)
                for (size_t i = 0; i < qv0.size(); ++i) {
                    pg->add(std::make_shared<qx::ctrl_phase_shift>(
                        qv0[i], qv1[i], angle));
                }
            else
                for (size_t i = 0; i < qv0.size(); ++i) {
                    pg->add(std::make_shared<qx::bin_ctrl>(
                        bv, std::make_shared<qx::ctrl_phase_shift>(
                                qv0[i], qv1[i], angle)));
                }
            return pg;
        }
    }

    return NULL;
}

std::shared_ptr<qx::circuit> load_cqasm_code(uint64_t qubits_count,
                                             compiler::SubCircuit const &subcircuit) {
    uint64_t iterations = subcircuit.numberIterations();
    std::string name = subcircuit.nameSubCircuit();

    auto circuit =
        std::make_shared<qx::circuit>(qubits_count, name, iterations);

    const std::vector<compiler::OperationsCluster *> &clusters =
        subcircuit.getOperationsCluster();

    for (auto p_cluster : clusters) {
        const std::vector<compiler::Operation *> operations =
            p_cluster->getOperations();
        for (auto p_operation : operations) {
            std::shared_ptr<qx::gate> g;
            try {
                if (p_operation->getType() == "barrier" ||
                    p_operation->getType() == "skip" ||
                    p_operation->getType() == "wait") {
                    continue;
                }
                g = gateLookup(*p_operation);
            } catch (const char *error) {
                std::cerr << error << std::endl;
                exit(-1);
            }
            if (!g) {
                throw std::runtime_error("Cannot create gate of type " + p_operation->getType());
            } else {
                circuit->add(g);
            }
        }
    }
    // circuit->dump();
    return circuit;
}

#endif
