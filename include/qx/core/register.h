/**
 * @file		register.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		01-10-15
 * @brief		quantum register implementation
 *
 */

#pragma once

#include <cassert>
#include <cfloat>
#include <complex>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include <random>

#include "qx/core/linalg.h"
#include "qx/xpu/timer.h"

// #define SAFE_MODE 1  // state norm check
#define QUBIT_ERROR_THRESHOLD (1e-10)
#define RAND_RANGE

using namespace qx::linalg;

#define __flt_format(x) (std::abs(x) < DBL_MIN ? 0 : x)
#define __cpl_format(x) __flt_format(x.real()) << ", " << __flt_format(x.imag())
#define __format_bin(x)                                                        \
    (x == __state_0__ ? '0' : (x == __state_1__ ? '1' : 'X'))

namespace qx {
/**
 * state_t
 */
enum state_t { __state_0__, __state_1__, __state_unknown__ };

using measurement_prediction_t = std::vector<state_t>;
using measurement_register_t = std::bitset<MAX_QB_N>;

/**
 * \brief quantum register implementation.
 */
class qu_register {
private:
    cvector_t data;
    cvector_t aux;
    measurement_prediction_t measurement_prediction;
    measurement_register_t measurement_register;

    uint64_t n_qubits;

    std::default_random_engine rgenerator;
    std::uniform_real_distribution<double> udistribution;

    /**
     * \brief collapse
     */
    uint64_t collapse(uint64_t entry);

    /**
     * \brief convert to binary
     */
    void to_binary(uint64_t state, uint64_t nq);

    /**
     * \brief set from binary
     */
    // void set_binary(uint64_t state, uint64_t nq);
    void set_measurement_prediction(uint64_t state, uint64_t nq);

public:
    /**
     * \brief quantum register of n_qubit
     */
    qu_register(uint64_t n_qubits);

    /**
     * reset
     */
    void reset();

    /**
     * \brief data getter
     */
    cvector_t &get_data();

    cvector_t &get_aux();

    /**
     * \brief data setter
     */
    void set_data(const cvector_t& d);

    /**
     * \brief size getter
     */
    uint64_t size() const;

    /**
     * \brief get states
     */
    uint64_t states() const;

    /**
     * \brief assign operator
     */
    cvector_t &operator=(cvector_t d);

    /**
     * \brief return ith amplitude
     */
    complex_t &operator[](uint64_t i);

    /**
     * \brief qubit validity check
     *   moduls squared of qubit entries must equal 1.
     *
     */
    bool check();

    /**
     * uniform rndom number generator
     */
    double rand() { return udistribution(rgenerator); }

    /**
     * \brief measure the entire quantum register
     */
    int64_t measure();

    /**
     * \brief dump
     */
    void dump(bool only_binary);

    /**
     * \brief return the quantum state as a string
     */
    std::string get_state(bool only_binary = false);

    std::string to_binary_string(uint64_t state, uint64_t nq);

    /**
     * \brief set the regiter to <state>
     */
    void set_measurement_prediction(uint64_t state);

    /**
     * \brief setter
     * set bit <q>  to the state <s>
     */
    // void set_binary(uint64_t q, state_t s);
    void set_measurement_prediction(uint64_t q, state_t s);

    /**
     * \brief set measurement outcome
     */
    void set_measurement(measurement_register_t const& meas);

    /**
     * \brief set measurement outcome
     */
    void set_measurement(uint64_t q, bool m);

    /**
     * \brief getter
     * \return the measurement prediction of qubit <q>
     */
    state_t get_measurement_prediction(uint64_t q) const;

    /**
     * \brief getter
     * \return the measurement outcome of qubit <q>
     */
    bool get_measurement(uint64_t q);

    /**
     * \brief getter
     * \return a reference to the whole measurement register
     */
    const std::bitset<MAX_QB_N>& get_measurement_register() const;

    /**
     * \brief test bit <q> of the binary register
     * \return true if bit <q> is 1
     */
    bool test(uint64_t q); // throw (qubit_not_measured_exception)  // trow exception
                      // if qubit value is unknown (never measured) !!!!

    /**
     * \brief flip binary
     */
    void flip_binary(uint64_t q);

    /**
     * \brief flip measurement
     */
    void flip_measurement(uint64_t q);

    /**
     * \brief return a string describing the quantum state
     */
    std::string quantum_state() {
        std::stringstream ss;
        ss << std::fixed;
        ss << "START\n";
        for (std::size_t i = 0; i < data.size(); ++i) {
            if (data[i] != complex_t(0, 0)) {
                ss << "   " << std::fixed << data[i] << " |";
                uint64_t nq = n_qubits;
                uint64_t state = i;
                while (nq--)
                    ss << (((state >> nq) & 1) ? "1" : "0");
                ss << "> +\n";
            }
        }
        ss << "END\n";
        return ss.str();
    }

    /**
     * \brief renormalize the quantum state
     */
    void normalize() {
        double length = 0;
        for (size_t k = 0; k < data.size(); k++)
            length += data[k].norm(); // std::norm(data[k]);
        length = std::sqrt(length);
        for (size_t k = 0; k < data.size(); k++)
            data[k] /= length;
    }

    /**
     * \brief returns a string describing the binary register
     */
    std::string binary_register() {
        std::stringstream ss;
        ss << "START\n";
        // for (int i=binary.size()-1; i>=0; --i)
        // ss << " | " << __format_bin(binary[i]);
        for (int i = measurement_prediction.size() - 1; i >= 0; --i) {
            ss << " | " << (measurement_register[i] ? "1" : "0");
        }
        // ss << " | " << __format_bin(measurement_prediction[i]);
        ss << " | \n";
        ss << "END\n";
        return ss.str();
    }
};

/**
 * \brief fidelity
 */
double fidelity(qu_register &s1, qu_register &s2);

} // namespace qx