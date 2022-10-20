/**
 * @file        qx_server.h
 * @author      Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date        25-02-16
 */

#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <map>

#include "qx/compat.h"

#include "qx/xpu/net/tcp_server_socket.h"
//#include <qcode/quantum_code_loader.h>

#include "qx/core/circuit.h"
#include "qx/core/error_model.h"
#include "qx/qcode/qx_strings.h"

using namespace str;

#define MAX_QUBITS 35

namespace qx {
/**
 * error codes
 */
// syntax errors
#define QX_ERROR_INVALID_QUBIT_ID 0x01
#define QX_ERROR_INVALID_BIT_ID 0x02
#define QX_ERROR_UNKNOWN_COMMAND 0x03
#define QX_ERROR_MALFORMED_CMD 0x04
#define QX_ERROR_QUBITS_NUM_ALREADY_DEFINED 0x05
#define QX_ERROR_TOO_MANY_QUBITS 0x06
#define QX_ERROR_INVALID_ERROR_MODEL 0x07
// semantic errors
#define QX_ERROR_QUBITS_NOT_YET_DEFINED 0x09
#define QX_ERROR_QUBIT_OUT_OF_RANGE 0x0A
#define QX_ERROR_TOFFOLI_REQUIRES_3_QUBITS 0x0B
#define QX_ERROR_CIRCUIT_NOT_FOUND 0x0C
#define QX_ERROR_UNKNOWN_ERROR_MODEL 0x0D

/**
 * \brief qx server
 */
class qx_server {

    using map_t = std::map<std::string, std::string>;
    using circuits_t = std::vector<std::shared_ptr<qx::circuit>>;

public:
    /**
     * ctor
     */
    qx_server(uint32_t port = 5555)
        : port(port), sock(0), qubits_count(0),
          syntax_error(false), semantic_error(false),
          error_model(__unknown_error_model__), error_probability(0) {}

    // #define __buf_size 8192
#define __buf_size 4096

#undef __verbose__
#define __verbose__ 1

    /**
     * start
     */
    void start() {
        char buf[__buf_size];
        xpu::tcp_server_socket server(port);
        println("[+] server listening on port ", port, "...");
        sock = server.accept();
        println("[+] client connected : ", sock->get_foreign_address(), ":",
                sock->get_foreign_port());
        // sock->send("welcom to server !", 19);
        bool stop = false;
        while (!stop) {
            clear(buf, __buf_size);
            size_t bytes = sock->recv(buf, __buf_size);
            buf[bytes] = '\0';
            std::string cmd = buf;
            if (__verbose__)
                std::cout << "[+] received command: " << cmd << std::endl;
            format_line(cmd);
            strings words = word_list(cmd, " ");
            if (words[0] == "stop") {
                // sock->send("[+] stopping server...\n", 23);
                sock->send("OK\n", 3);
                println("[+] stopping server...");
                println("[+] done.");
                return;
            } else if (words[0] == "circuits") {
                std::string response =
                    int_to_str(circuits.size()) + " circuit(s) found: ";
                sock->send(response.c_str(), response.length());
                for (int i = 0; i < circuits.size(); ++i) {
                    sock->send(circuits[i]->id().c_str(),
                               circuits[i]->id().length());
                    sock->send(", ", 2);
                }
                sock->send("\n", 1);
                continue;
            } else if (words[0] == "reset") {
                println("[+] removing quantum register...");
                qubits_count = 0;
                println("[+] deleting circuits...");
                circuits.clear();
                println("[+] reset done.");
                sock->send("OK\n", 3);
                continue;
            } else if (words[0] == "run") {
                if (qubits_count == 0) {
                    std::string error_code =
                        "E" + int_to_str(QX_ERROR_QUBITS_NOT_YET_DEFINED) +
                        "\n";
                    sock->send(error_code.c_str(), error_code.length() + 1);
                } else if (words.size() != 2) {
                    std::string error_code =
                        "E" + int_to_str(QX_ERROR_MALFORMED_CMD) + "\n";
                    sock->send(error_code.c_str(), error_code.length() + 1);
                } else {
                    std::shared_ptr<qx::circuit> c{};
                    println("[+] trying to execute '", words[1], "'");
                    for (int i = 0; i < circuits.size(); ++i) {
                        if (words[1] == circuits[i]->id())
                            c = circuits[i];
                    }
                    if (c) {
                        qx::qu_register &r = *reg;
                        // c->dump();
                        c->execute(r);
                        sock->send("OK\n", 3);
                    } else {
                        println("[!] circuit not found !");
                        std::string error_code =
                            "E" + int_to_str(QX_ERROR_CIRCUIT_NOT_FOUND) + "\n";
                        sock->send(error_code.c_str(), error_code.length() + 1);
                    }
                }
                continue;
            } else if (words[0] == "run_noisy") // noisy circuit execution
            {
                if (qubits_count == 0) {
                    std::string error_code =
                        "E" + int_to_str(QX_ERROR_QUBITS_NOT_YET_DEFINED) +
                        "\n";
                    sock->send(error_code.c_str(), error_code.length() + 1);
                } else if ((words.size() != 4) && (words.size() != 5)) {
                    std::string error_code =
                        "E" + int_to_str(QX_ERROR_MALFORMED_CMD) + "\n";
                    sock->send(error_code.c_str(), error_code.length() + 1);
                }
                // else if (words.size() == 4)
                else {
                    std::shared_ptr<qx::circuit> c{};
                    size_t iterations = 1;
                    if (words.size() == 5)
                        iterations = atoi(words[4].c_str());
                    for (int i = 0; i < circuits.size(); ++i) {
                        if (words[1] == circuits[i]->id())
                            c = circuits[i];
                    }
                    if (c) {
                        if (words[2] != "depolarizing_channel") {
                            std::string error_code =
                                "E" + int_to_str(QX_ERROR_UNKNOWN_ERROR_MODEL) +
                                "\n";
                            sock->send(error_code.c_str(),
                                       error_code.length() + 1);
                        } else {
                            double error_probability = atof(words[3].c_str());
                            println("[+] executing '", words[1], "' (",
                                    iterations,
                                    " iterations) under depolarizing noise...");
                            qx::qu_register &r = *reg;
                            qx::depolarizing_channel dch(c, r.size(),
                                                         error_probability);
                            while (iterations--) {
                                // println("[+] execution of '" , words[1] ,
                                // "' under depolarizing noise...");
                                auto nc = dch.inject(false);
                                nc->execute(r);
                            }
                            println("[+] done.");
                            sock->send("OK\n", 3);
                        }
                    } else {
                        println("[!] circuit not found !");
                        std::string error_code =
                            "E" + int_to_str(QX_ERROR_CIRCUIT_NOT_FOUND) + "\n";
                        sock->send(error_code.c_str(), error_code.length() + 1);
                    }
                }
                continue;
            }
            /**
             * parallel gates
             */
            else if ((words[0] == "{") && (words[words.size() - 1] == "}")) {
                std::string pg_line = cmd;
                format_line(pg_line);
                replace_all(pg_line, "{", "");
                replace_all(pg_line, "}", "");
                // println("pg_line : " , pg_line);
                strings gates = word_list(pg_line, "|");
                auto _pgs = std::make_shared<qx::parallel_gates>();
                for (size_t i = 0; i < gates.size(); ++i) {
                    // println("processing '" , gates[i] , "'...");
                    process_line(gates[i], _pgs.get());
                }
                current_sub_circuit(qubits_count)->add(_pgs);
                sock->send("OK\n", 3);
                continue;
            }
            // check if it is a batch command
            // remove_comment(cmd);
            bool batch = (cmd.find(";") < cmd.size());
            bool cmd_error = false;
            if (batch) {
                strings cmds = word_list(cmd, ";");
                for (size_t i = 0; i < cmds.size(); ++i) {
                    int32_t res = process_line(cmds[i]);
                    if (res) {
                        std::string error_code = "E" + int_to_str(res) + "\n";
                        sock->send(error_code.c_str(), error_code.length() + 1);
                        cmd_error = true;
                    }
                    // else
                    // sock->send("OK\n", 3);
                }
                if (!cmd_error)
                    sock->send("OK\n", 3);
            } else {
                int32_t res = process_line(cmd);
                if (res) {
                    std::string error_code = "E" + int_to_str(res) + "\n";
                    sock->send(error_code.c_str(), error_code.length() + 1);
                } else
                    sock->send("OK\n", 3);
            }
        }
        return;
    }

private:
    void clear(char *buf, size_t size) {
        for (uint32_t i = 0; i < size; i++)
            buf[i] = '\0';
    }

#define print_syntax_error(err, code)                                          \
    {                                                                          \
        std::cerr << "[x] syntax error at line " << line_index << " : " << err \
                  << std::endl;                                                \
        std::cerr << "   +--> code: \"" << original_line << "\"" << std::endl; \
        syntax_error = true;                                                   \
        return code;                                                           \
    }

#define print_semantic_error(err, code)                                        \
    {                                                                          \
        std::cerr << "[x] semantic error at line " << line_index << " : "      \
                  << err << std::endl;                                         \
        std::cerr << "   +--> code: \"" << original_line << "\"" << std::endl; \
        semantic_error = true;                                                 \
        return code;                                                           \
    }

private:
    /**
     * \brief check if the circuit label is valid
     */
    bool is_label(std::string &str) {
        if (!is_dot(str[0]))
            return false;
        for (uint32_t i = 1; i < str.size(); ++i) {
            if ((!is_letter(str[i])) && (!is_digit(str[i])))
                return false;
        }
        return true;
    }

    /**
     * \brief check if <str> is a natural bit
     */
    bool is_bit(std::string &str) { return (str[0] == 'b'); }

    /**
     * \brief translate user-defined qubit/bit name to qubit/bit identifier
     */
    void translate(std::string &str) {
        // search in the qubit map first
        map_t::iterator it = definitions.find(str);
        if (it != definitions.end()) {
            std::string id = it->second;
            // println(" $ translate : " << str << " -> " << id);
            str.replace(0, str.size(), id);
        }
    }

    /**
     * \brief retrieve qubit number <N> from a string "qN"
     */
    uint32_t qubit_id(std::string &str) {
        std::string &original_line = str;
        std::string qubit;
        if (str[0] != 'q') {
            // search in the qubit map first
            map_t::iterator it = definitions.find(str);
            if (it != definitions.end()) {
                qubit = it->second;
                // println(" def[" << str << "] -> " << qubit);
                if (qubit[0] != 'q')
                    print_syntax_error(
                        " invalid qubit identifier : qubit name not defined, "
                        "you should use 'map' to name qubit before using it !",
                        QX_ERROR_INVALID_QUBIT_ID);
                str = qubit;
            } else
                print_syntax_error(" invalid qubit identifier !",
                                   QX_ERROR_INVALID_QUBIT_ID);
        }
        std::string id = str.substr(1);
        for (int i = 0; i < id.size(); ++i) {
            if (!is_digit(id[i]))
                print_syntax_error(" invalid qubit identifier !",
                                   QX_ERROR_INVALID_QUBIT_ID);
        }
        // println(" qubit id -> " << id);
        return (atoi(id.c_str()));
    }

    uint32_t bit_id(std::string &str) {
        std::string &original_line = str;
        if (str[0] != 'b')
            print_syntax_error(" invalid bit identifier !",
                               QX_ERROR_INVALID_BIT_ID);
        std::string id = str.substr(1);
        for (int i = 0; i < id.size(); ++i) {
            if (!is_digit(id[i]))
                print_syntax_error(" invalid bit identifier !",
                                   QX_ERROR_INVALID_BIT_ID);
        }
        return (atoi(id.c_str()));
    }

    /**
     * \return
     *    a pointer to the current sub-circuit
     */
    std::shared_ptr<qx::circuit> current_sub_circuit(uint32_t qubits_count) {
        if (circuits.size()) {
            return circuits.back();
        }
        circuits.push_back(
            std::make_shared<qx::circuit>(qubits_count, "default"));
        return circuits.back();
    }

    int32_t process_line(std::string &line, qx::parallel_gates *pg) {
        std::string original_line = line;
        format_line(line);

        strings words = word_list(line, " ");
        // println("line : " << line << " (" << words.size() << ")");
        // for (size_t i=0; i<words.size(); ++i)
        // println("\t" << words[i]);

        if (words[0] == "h") // hadamard gate
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            pg->add(std::make_shared<qx::hadamard>(q));
        } else if (words[0] == "rx90") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            pg->add(std::make_shared<qx::rx>(q, QX_PI / 2));
        } else if (words[0] == "mrx90") // mrx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            pg->add(std::make_shared<qx::rx>(q, -QX_PI / 2));
        } else if (words[0] == "rx180") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            pg->add(std::make_shared<qx::rx>(q, QX_PI));
        } else if (words[0] == "ry90") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            pg->add(std::make_shared<qx::ry>(q, QX_PI / 2));
        } else if (words[0] == "mry90") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            pg->add(std::make_shared<qx::ry>(q, -QX_PI / 2));
        } else if (words[0] == "ry180") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            pg->add(std::make_shared<qx::ry>(q, QX_PI));
        } else if (words[0] == "cnot") // cnot gate
        {
            strings params = word_list(words[1], ",");
            uint32_t cq = qubit_id(params[0]);
            uint32_t tq = qubit_id(params[1]);
            if (cq > (qubits_count - 1))
                print_semantic_error(" control qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (tq > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => cnot gate : ctrl_qubit=" << cq << ", target_qubit="
            // << tq);
            pg->add(std::make_shared<qx::cnot>(cq, tq));
        } else if (words[0] == "swap") // cnot gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q1 = qubit_id(params[0]);
            uint32_t q2 = qubit_id(params[1]);
            if ((q1 > (qubits_count - 1)) || (q1 > (qubits_count - 1)))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => swap gate : qubit_1=" << q1 << ", qubit_2=" << q2);
            pg->add(std::make_shared<qx::swap>(q1, q2));
        }

        /**
         * controlled phase shift
         */
        else if (words[0] == "cr") {
            strings params = word_list(words[1], ",");
            uint32_t q1 = qubit_id(params[0]);
            uint32_t q2 = qubit_id(params[1]);
            if ((q1 > (qubits_count - 1)) || (q1 > (qubits_count - 1)))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => controlled phase shift gate : ctrl_qubit=" << q1 <<
            // ", target_qubit=" << q2);
            pg->add(std::make_shared<qx::ctrl_phase_shift>(q1, q2));
        }
        /**
         * cphase
         */
        else if (words[0] == "cphase") {
            strings params = word_list(words[1], ",");
            uint32_t q1 = qubit_id(params[0]);
            uint32_t q2 = qubit_id(params[1]);
            if ((q1 > (qubits_count - 1)) || (q1 > (qubits_count - 1)))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => controlled phase shift gate : ctrl_qubit=" << q1 <<
            // ", target_qubit=" << q2);
            pg->add(std::make_shared<qx::cphase>(q1, q2));
        }

        /**
         * pauli gates
         */
        else if (words[0] == "x") // x gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => pauli x gate on: " << q);
            pg->add(std::make_shared<qx::pauli_x>(q));
        } else if (words[0] == "cx") // x gate
        {
            strings params = word_list(words[1], ",");
            translate(params[0]);
            bool bit = is_bit(params[0]);
            uint32_t ctrl = (bit ? bit_id(params[0]) : qubit_id(params[0]));
            uint32_t target = qubit_id(params[1]);

            if (ctrl > (qubits_count - 1))
                print_semantic_error(" ctrl qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (target > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (bit) {
                // println(" => binary controlled pauli_x gate (ctrl=" << ctrl
                // << ", target=" << target << ")");
                pg->add(std::make_shared<qx::bin_ctrl>(
                    ctrl, std::make_shared<qx::pauli_x>(target)));
            } else {
                // println(" => controlled pauli_x gate (ctrl=" << ctrl << ",
                // target=" << target << ")");
                pg->add(std::make_shared<qx::cnot>(ctrl, target));
            }
        } else if (words[0] == "y") // y gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => pauli y gate on: " << atoi(words[1].c_str()));
            pg->add(std::make_shared<qx::pauli_y>(q));
        } else if (words[0] == "z") // z gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => pauli z gate on: " << atoi(words[1].c_str()));
            pg->add(std::make_shared<qx::pauli_z>(q));
        } else if (words[0] == "cz") // z gate
        {
            strings params = word_list(words[1], ",");
            translate(params[0]);
            bool bit = is_bit(params[0]);
            uint32_t ctrl = (bit ? bit_id(params[0]) : qubit_id(params[0]));
            uint32_t target = qubit_id(params[1]);

            if (ctrl > (qubits_count - 1))
                print_semantic_error(" ctrl qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (target > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (bit) {
                // println(" => binary controlled pauli_z gate (ctrl=" << ctrl
                // << ", target=" << target << ")");
                pg->add(std::make_shared<qx::bin_ctrl>(
                    ctrl, std::make_shared<qx::pauli_z>(target)));
            } else {
                pg->add(std::make_shared<qx::cphase>(ctrl, target));
                // println(" => controlled pauli_z gate (ctrl=" << ctrl << ",
                // target=" << target << ")");
                //  pg->add(std::make_shared<qx::cnot>(ctrl,target));
            }
        }
        /**
         * T gate
         */
        else if (words[0] == "t") // T gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => t gate on: " << q);
            pg->add(std::make_shared<qx::t_gate>(q));
        }
        /**
         * Tdag gate
         */
        else if (words[0] == "tdag") // Tdag gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => t gate on: " << q);
            pg->add(std::make_shared<qx::t_dag_gate>(q));
        }

        /**
         * prepz
         */
        else if (words[0] == "prepz") // prepre in state 0 gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => t gate on: " << q);
            pg->add(std::make_shared<qx::prepz>(q));
        }

        /**
         * rotations gates
         */
        else if (words[0] == "rx") // rx gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q = qubit_id(params[0]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => rx gate on " << process_qubit(params[0]) << "
            // (angle=" << params[1] << ")");
            pg->add(std::make_shared<qx::rx>(q, atof(params[1].c_str())));
        } else if (words[0] == "ry") // ry gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q = qubit_id(params[0]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => ry gate on " << process_qubit(params[0]) << "
            // (angle=" << params[1] << ")");
            pg->add(std::make_shared<qx::ry>(q, atof(params[1].c_str())));
        } else if (words[0] == "rz") // rz gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q = qubit_id(params[0]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => rz gate on " << process_qubit(params[0]) << "
            // (angle=" << params[1] << ")");
            pg->add(std::make_shared<qx::rz>(q, atof(params[1].c_str())));
        }

        /**
         * phase
         */
        else if (words[0] == "ph") // phase shift gate
        {
            // strings params = word_list(words[1],",");
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => phase gate on " << process_qubit(words[1]));
            pg->add(std::make_shared<qx::phase_shift>(q));
        } else if (words[0] == "s") // phase shift gate
        {
            // strings params = word_list(words[1],",");
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => phase gate on " << process_qubit(words[1]));
            pg->add(std::make_shared<qx::phase_shift>(q));
        }

        /**
         * measurement
         */
        else if (words[0] == "measure") // measurement
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => measure qubit " << atoi(words[1].c_str()));
            // println(" => measure qubit " << q);
            pg->add(std::make_shared<qx::measure>(q));
        }
        /**
         * toffoli
         */
        else if (words[0] == "toffoli") // rx gate
        {
            strings params = word_list(words[1], ",");
            if (params.size() != 3)
                print_semantic_error(" toffoli gate requires 3 qubits !",
                                     QX_ERROR_TOFFOLI_REQUIRES_3_QUBITS);
            uint32_t q0 = qubit_id(params[0]);
            uint32_t q1 = qubit_id(params[1]);
            uint32_t q2 = qubit_id(params[2]);
            if (q0 > (qubits_count - 1))
                print_semantic_error(" first control qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (q1 > (qubits_count - 1))
                print_semantic_error(" scond control qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (q2 > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => toffoli gate on " << process_qubit(params[2]) << "
            // (ctrl_q1=" << params[0] << ", ctrl_q2=" << params[1] << ")");
            pg->add(std::make_shared<qx::toffoli>(q0, q1, q2));
        } else if (words[0] == "nop") {
            // current_sub_circuit(qubits_count)->add(std::make_shared<qx::measure_all>());
        } else if (words[0] == "qwait") {
            // current_sub_circuit(qubits_count)->add(std::make_shared<qx::measure_all>());
        }
        return 0;
    }

    /**
     * \brief
     *   process line
     */
    int32_t process_line(std::string &line) {
        // println("processing line " << line_index << " ...");
        format_line(line);
        if (is_empty(line))
            return 0;
        if (line[0] == '#') // skip comments
        {
            // println("   comment.");
            return 0;
        }
        std::string original_line(line);
        remove_comment(line, '#'); // remove inline comment
        format_line(line);
        if (is_label(line)) {
            if (qubits_count == 0) {
                print_semantic_error(" qubits number must be defined first !",
                                     QX_ERROR_QUBITS_NOT_YET_DEFINED);
            } else {
                // check if the circuit exist and not empty
                std::string name = line.substr(1);
                bool exist = false;
                uint32_t index = 0;
                for (uint32_t i = 0; i < circuits.size(); ++i) {
                    if (circuits[i]->id() == name) {
                        exist = true;
                        index = i;
                        break;
                    }
                }
                // println("[!] warning : circuit '" , name , "' already exist
                // ! Old circuit and its gates will be deleted !");
                if (exist) {
                    circuits.erase(circuits.begin() + index);
                }
                println("[!] warning : circuit '", name, "' reinitialized !");
                // println("label : new circuit '" , line , "' created.");
                circuits.push_back(std::make_shared<qx::circuit>(
                    qubits_count, line.substr(1)));
                return 0;
            }
        }

        strings words = word_list(line, " ");
        // process display commands
        if (words.size() == 1) {
            if (words[0] == "display") {
                // current_sub_circuit(qubits_count)->add(std::make_shared<qx::display>());
                std::string qstate = reg->quantum_state();
                sock->send(qstate.c_str(), qstate.length());
            } else if (words[0] == "display_binary") {
                // current_sub_circuit(qubits_count)->add(std::make_shared<qx::display>(true));
                std::string breg = reg->binary_register();
                sock->send(breg.c_str(), breg.length());
            } else if (words[0] == "get_quantum_state") // equivalent to display
                                                        // (redundant !)
            {
                std::string qstate = reg->quantum_state();
                sock->send(qstate.c_str(), qstate.length());
            } else if (words[0] ==
                       "get_measurements") // equivalent to display_binary
            {
                std::string breg = reg->binary_register();
                sock->send(breg.c_str(), breg.length());
            } else if (words[0] == "measure") {
                current_sub_circuit(qubits_count)
                    ->add(std::make_shared<qx::measure_all>());
            } else if (words[0] == "nop") {
                // current_sub_circuit(qubits_count)->add(std::make_shared<qx::measure_all>());
            } else if (words[0] == "qwait") {
                // current_sub_circuit(qubits_count)->add(std::make_shared<qx::measure_all>());
            } else
                print_syntax_error("unknown command !",
                                   QX_ERROR_UNKNOWN_COMMAND);
            return 0;
        }

        if (words.size() != 2)
            print_syntax_error("malformed code !", QX_ERROR_MALFORMED_CMD);

        if (words[0] == "qubits") // qubit definition
        {
            if (qubits_count)
                print_syntax_error("qubits number already defined !",
                                   QX_ERROR_QUBITS_NUM_ALREADY_DEFINED);

            qubits_count = atoi(words[1].c_str());

            if ((qubits_count > 0) && (qubits_count < MAX_QUBITS)) {
                reg = std::make_unique<qx::qu_register>(qubits_count);
                // println(" => qubits number: " , qubits_count);
            } else
                print_syntax_error(" too many qubits (" << qubits_count
                                                        << ") !",
                                   QX_ERROR_TOO_MANY_QUBITS);
        } else if (qubits_count == 0) {
            print_semantic_error(" qubits number must be defined first !",
                                 QX_ERROR_QUBITS_NOT_YET_DEFINED);
        } else if (words[0] == "map") // definitions
        {
            strings params = word_list(words[1], ",");
            uint32_t q = 0;
            if (params[0][0] == 'q')
                q = qubit_id(params[0]);
            else if (params[0][0] == 'b')
                q = bit_id(params[0]);
            else
                print_semantic_error(" invalid qubit/bit identifier !",
                                     QX_ERROR_INVALID_QUBIT_ID);
            std::string qubit = params[0];
            std::string name = params[1];
            if (q > (qubits_count - 1))
                print_semantic_error(" qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            definitions[name] = qubit;
            // println(" => map qubit " << name << " to " << qubit);
        } else if (words[0] == "h") // hadamard gate
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::hadamard>(q));
        } else if (words[0] == "rx90") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::rx>(q, QX_PI / 2));
        } else if (words[0] == "mrx90") // mrx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::rx>(q, -QX_PI / 2));
        } else if (words[0] == "rx180") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::rx>(q, QX_PI));
        } else if (words[0] == "ry90") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::ry>(q, QX_PI / 2));
        } else if (words[0] == "mry90") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::ry>(q, -QX_PI / 2));
        } else if (words[0] == "ry180") // rx90
        {
            uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => hadamard gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::ry>(q, QX_PI));
        } else if (words[0] == "cnot") // cnot gate
        {
            strings params = word_list(words[1], ",");
            uint32_t cq = qubit_id(params[0]);
            uint32_t tq = qubit_id(params[1]);
            if (cq > (qubits_count - 1))
                print_semantic_error(" control qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (tq > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => cnot gate : ctrl_qubit=" << cq << ", target_qubit="
            // << tq);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::cnot>(cq, tq));
        } else if (words[0] == "swap") // cnot gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q1 = qubit_id(params[0]);
            uint32_t q2 = qubit_id(params[1]);
            if ((q1 > (qubits_count - 1)) || (q1 > (qubits_count - 1)))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => swap gate : qubit_1=" << q1 << ", qubit_2=" << q2);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::swap>(q1, q2));
        }

        /**
         * controlled phase shift
         */
        else if (words[0] == "cr") {
            strings params = word_list(words[1], ",");
            uint32_t q1 = qubit_id(params[0]);
            uint32_t q2 = qubit_id(params[1]);
            if ((q1 > (qubits_count - 1)) || (q1 > (qubits_count - 1)))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => controlled phase shift gate : ctrl_qubit=" << q1 <<
            // ", target_qubit=" << q2);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::ctrl_phase_shift>(q1, q2));
        }
        /**
         * cphase
         */
        else if (words[0] == "cphase") {
            strings params = word_list(words[1], ",");
            uint32_t q1 = qubit_id(params[0]);
            uint32_t q2 = qubit_id(params[1]);
            if ((q1 > (qubits_count - 1)) || (q1 > (qubits_count - 1)))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => controlled phase shift gate : ctrl_qubit=" << q1 <<
            // ", target_qubit=" << q2);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::cphase>(q1, q2));
        }

        /**
         * pauli gates
         */
        else if (words[0] == "x") // x gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => pauli x gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::pauli_x>(q));
        } else if (words[0] == "cx") // x gate
        {
            strings params = word_list(words[1], ",");
            translate(params[0]);
            bool bit = is_bit(params[0]);
            uint32_t ctrl = (bit ? bit_id(params[0]) : qubit_id(params[0]));
            uint32_t target = qubit_id(params[1]);

            if (ctrl > (qubits_count - 1))
                print_semantic_error(" ctrl qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (target > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (bit) {
                // println(" => binary controlled pauli_x gate (ctrl=" << ctrl
                // << ", target=" << target << ")");
                current_sub_circuit(qubits_count)
                    ->add(std::make_shared<qx::bin_ctrl>(
                        ctrl, std::make_shared<qx::pauli_x>(target)));
            } else {
                // println(" => controlled pauli_x gate (ctrl=" << ctrl << ",
                // target=" << target << ")");
                current_sub_circuit(qubits_count)
                    ->add(std::make_shared<qx::cnot>(ctrl, target));
            }
        } else if (words[0] == "y") // y gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => pauli y gate on: " << atoi(words[1].c_str()));
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::pauli_y>(q));
        } else if (words[0] == "z") // z gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => pauli z gate on: " << atoi(words[1].c_str()));
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::pauli_z>(q));
        } else if (words[0] == "cz") // z gate
        {
            strings params = word_list(words[1], ",");
            translate(params[0]);
            bool bit = is_bit(params[0]);
            uint32_t ctrl = (bit ? bit_id(params[0]) : qubit_id(params[0]));
            uint32_t target = qubit_id(params[1]);

            if (ctrl > (qubits_count - 1))
                print_semantic_error(" ctrl qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (target > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (bit) {
                // println(" => binary controlled pauli_z gate (ctrl=" << ctrl
                // << ", target=" << target << ")");
                current_sub_circuit(qubits_count)
                    ->add(std::make_shared<qx::bin_ctrl>(
                        ctrl, std::make_shared<qx::pauli_z>(target)));
            } else {
                current_sub_circuit(qubits_count)
                    ->add(std::make_shared<qx::cphase>(ctrl, target));
                // println(" => controlled pauli_z gate (ctrl=" << ctrl << ",
                // target=" << target << ")");
                //  current_sub_circuit(qubits_count)->add(std::make_shared<qx::cnot>(ctrl,target));
            }
        }
        /**
         * T gate
         */
        else if (words[0] == "t") // T gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => t gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::t_gate>(q));
        }
        /**
         * Tdag gate
         */
        else if (words[0] == "tdag") // Tdag gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => t gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::t_dag_gate>(q));
        }

        /**
         * prepz
         */
        else if (words[0] == "prepz") // prepre in state 0 gate
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => t gate on: " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::prepz>(q));
        }

        /**
         * rotations gates
         */
        else if (words[0] == "rx") // rx gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q = qubit_id(params[0]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => rx gate on " << process_qubit(params[0]) << "
            // (angle=" << params[1] << ")");
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::rx>(q, atof(params[1].c_str())));
        } else if (words[0] == "ry") // ry gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q = qubit_id(params[0]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => ry gate on " << process_qubit(params[0]) << "
            // (angle=" << params[1] << ")");
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::ry>(q, atof(params[1].c_str())));
        } else if (words[0] == "rz") // rz gate
        {
            strings params = word_list(words[1], ",");
            uint32_t q = qubit_id(params[0]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => rz gate on " << process_qubit(params[0]) << "
            // (angle=" << params[1] << ")");
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::rz>(q, atof(params[1].c_str())));
        }

        /**
         * phase
         */
        else if (words[0] == "ph") // phase shift gate
        {
            // strings params = word_list(words[1],",");
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => phase gate on " << process_qubit(words[1]));
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::phase_shift>(q));
        } else if (words[0] == "s") // phase shift gate
        {
            // strings params = word_list(words[1],",");
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => phase gate on " << process_qubit(words[1]));
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::phase_shift>(q));
        }

        /**
         * measurement
         */
        else if (words[0] == "measure") // measurement
        {
            uint32_t q = qubit_id(words[1]);
            if (q > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => measure qubit " << atoi(words[1].c_str()));
            // println(" => measure qubit " << q);
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::measure>(q));
        }
        /**
         * toffoli
         */
        else if (words[0] == "toffoli") // rx gate
        {
            strings params = word_list(words[1], ",");
            if (params.size() != 3)
                print_semantic_error(" toffoli gate requires 3 qubits !",
                                     QX_ERROR_TOFFOLI_REQUIRES_3_QUBITS);
            uint32_t q0 = qubit_id(params[0]);
            uint32_t q1 = qubit_id(params[1]);
            uint32_t q2 = qubit_id(params[2]);
            if (q0 > (qubits_count - 1))
                print_semantic_error(" first control qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (q1 > (qubits_count - 1))
                print_semantic_error(" scond control qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            if (q2 > (qubits_count - 1))
                print_semantic_error(" target qubit out of range !",
                                     QX_ERROR_QUBIT_OUT_OF_RANGE);
            // println(" => toffoli gate on " << process_qubit(params[2]) , "
            // (ctrl_q1=" , params[0] , ", ctrl_q2=" , params[1] , ")");
            current_sub_circuit(qubits_count)
                ->add(std::make_shared<qx::toffoli>(q0, q1, q2));
        }
        /**
         * error model
         */
        else if (words[0] == "error_model") // operational errors
        {
            strings params = word_list(words[1], ",");
            if (params.size() != 2)
                print_syntax_error(" error mode should be specified according "
                                   "to the following syntax: 'error_model "
                                   "depolarizing_channel,0.01' ",
                                   QX_ERROR_INVALID_ERROR_MODEL);
            if (params[0] == "depolarizing_channel") {
                error_model = __depolarizing_channel__;
                error_probability = atof(params[1].c_str());
                println(" => error model:  (name=", params[0].c_str(),
                        ", error_probability=", error_probability, ")");
            } else
                print_semantic_error(" unknown error model !",
                                     QX_ERROR_INVALID_ERROR_MODEL);
        }

        /**
         * noise
         */
        else if (words[0] == "noise") // operational errors
        {
            strings params = word_list(words[1], ",");
            println(" => noise (theta=", params[0].c_str(),
                    ", phi=", params[1].c_str(), ")");
        } else if (words[0] == "decoherence") // decoherence
        {
            println(" => decoherence (dt=", words[1], ")");
        } else if (words[0] == "qec") // decoherence
        {
            println(" => quantum error correction scheme = ", words[1]);
        } else
            print_syntax_error(" unknown gate or command !",
                               QX_ERROR_UNKNOWN_COMMAND);

        return 0;
    }

private:
    int line_index;
    bool syntax_error;
    bool semantic_error;

    // definitions
    map_t definitions;

    uint32_t qubits_count;

    // error model
    qx::error_model_t error_model;
    double error_probability;

    uint32_t port;
    xpu::tcp_socket *sock;
    circuits_t circuits;
    std::unique_ptr<qu_register> reg;
};
} // namespace qx