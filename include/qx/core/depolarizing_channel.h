#pragma once

#include "qx/compat.h"
#include "qx/core/error_injector.h"
#include "qx/core/random.h"
#include <memory>
#include <random>

namespace qx {

typedef std::vector<size_t> histogram_t;

#define __third__ (0.33333333333333333333333333333f)

typedef enum { __x_error__, __z_error__, __y_error__ } error_type_t;
typedef std::pair<error_type_t, size_t> error_t;

/**
 * \brief depolarizing channel implementation
 */
class depolarizing_channel : public error_injector {
public:
    depolarizing_channel(std::shared_ptr<qx::circuit> const& c, size_t nq, double pe)
        : nrg(0, __third__), circuit(c), nq(nq), pe(pe), xp(__third__), yp(__third__),
          zp(__third__), overall_error_probability(0), total_errors(0),
          error_histogram(nq + 1, 0), error_recording(false) {
        for (size_t i = 1; i < (nq + 1); ++i) {
            double pp = error_probability(nq, i, pe);
            overall_error_probability += pp;
        }

        QX_SRAND(xpu::timer().current());
    }

    depolarizing_channel(std::shared_ptr<qx::circuit> const& c, size_t nq, double pe,
                         double xp, double yp, double zp)
        : nrg(0, __third__), circuit(c), nq(nq), pe(pe), xp(xp), yp(yp), zp(zp),
          overall_error_probability(0), total_errors(0),
          error_histogram(nq + 1, 0), error_recording(false) {
        for (size_t i = 1; i < (nq + 1); ++i) {
            double pp = error_probability(nq, i, pe);
            overall_error_probability += pp;
        }

        QX_SRAND(xpu::timer().current());
    }

    /**
     * check whether a qubit is idle
     */
    bool is_idle(size_t q, std::vector<size_t> used) {
        for (int i = 0; i < used.size(); ++i)
            if (q == used[i])
                return false;
        return true;
    }

    /**
     * return idle qubits
     */
    std::vector<size_t> idle_qubits(size_t n, std::vector<size_t> &used) {
        std::vector<size_t> r;
        for (size_t i = 0; i < n; i++) {
            if (is_idle(i, used))
                r.push_back(i);
        }
        return r;
    }

    bool is_measurement(qx::gate const *g, size_t q) const {
        if (g->type() == __measure_reg_gate__) {
            return true;
        }
        if (g->type() == __measure_gate__) {
            assert(dynamic_cast<const qx::measure*>(g));
            auto* measure_g = static_cast<const qx::measure*>(g);
            return measure_g->measured_qubit() == q;
        }
        if (g->type() == __measure_reg_gate__) {
            return true;
        }
        if (g->type() == __parallel_gate__) {
            return ((qx::parallel_gates *)g)->has([this, q](auto const *gate) {
                return is_measurement(gate, q);
            });
        }
        return false;
    }

    /**
     * \brief enable error recording
     */
    void enable_error_recording() { error_recording = true; }

    /**
     * \brief disable error recording
     */
    void disable_error_recording() { error_recording = false; }

    const char error_type[3] = {'x', 'z', 'y'};

    /**
     * \brief report injected errors
     */
    void report_errors() {
        if (errors.empty())
            return;
        println("[x] injected errors report : ");
        for (size_t i = 0; i < errors.size(); ++i) {
            println("     ", error_location[i], " : ",
                    error_type[errors[i].first], " on qubit ",
                    errors[i].second);
        }
        println("[+] stats : ", x_errors, " x | ", z_errors, " z | ", y_errors,
                " y ");
    }

#define __verbose__ if (verbose)

    /**
     * \brief inject errors
     * \return noisy circuit
     */
    std::shared_ptr<qx::circuit> inject(bool verbose = false) {
        x_errors = 0;
        z_errors = 0;
        y_errors = 0;

        __verbose__ println(
            "    [e] depolarizing_channel : injecting errors in circuit '",
            circuit->id(), "'...");
        size_t steps = circuit->size();
        auto noisy_circuit = std::make_shared<qx::circuit>(nq, circuit->id() + "(noisy)");

        errors.clear();
        error_location.clear();
        total_errors = 0;

        __verbose__ println("    [+] circuit steps : ", steps);
        for (size_t p = 0; p < steps; ++p) {
            qx::gate_type_t gt = circuit->get(p)->type();
            if ((gt == qx::__display__) || (gt == qx::__display_binary__)) {
                noisy_circuit->add(circuit->get(p));
                continue;
            }

            double x = uniform_rand();

            if (x < overall_error_probability) {
                size_t affected_qubits = 1;

                for (size_t i = 2; i < (nq + 1); ++i) {
                    if (x > error_probability(nq, i, pe))
                        break;
                    affected_qubits++;
                }

                total_errors += affected_qubits;
                error_histogram[affected_qubits]++;

                __verbose__ println(
                    "    [>>>] error injection step ", p,
                    " : number of affected qubits: ", affected_qubits);
                if (error_recording) {
                    for (size_t e = 0; e < affected_qubits; ++e)
                        error_location.push_back(p);
                }

                if (affected_qubits == 1) {
                    size_t q = rand() % nq;

                    if (is_measurement(circuit->get(p).get(), q)) {
                        __verbose__ print("      |--> error on qubit ", q,
                                          " (potential measurement error) ");
                        noisy_circuit->add(measurement_error(q, verbose));
                        noisy_circuit->add(circuit->get(p));
                    } else {
                        __verbose__ print("      |--> error on qubit  ", q);
                        noisy_circuit->add(circuit->get(p));
                        noisy_circuit->add(single_qubit_error(q, verbose));
                    }

                } else {
                    auto g = std::make_shared<qx::parallel_gates>();
                    bool measurement = false;

                    std::vector<bool> v(nq, 0);
                    for (size_t i = 0; i < affected_qubits; ++i) {
                        size_t q = rand() % nq;
                        while (v[q]) {
                            q = rand() % nq;
                        }
                        v[q] = 1;

                        if (is_measurement(circuit->get(p).get(), q))
                            measurement = true;
                        __verbose__ print("      |--> error on qubit  ", q,
                                          (measurement
                                               ? "(potential measurement error)"
                                               : ""));
                        g->add(single_qubit_error(q, verbose));
                    }

                    if (measurement) {
                        //__verbose__  println("      |--> (!) potential
                        // measurement error.");
                        noisy_circuit->add(g);
                        noisy_circuit->add(circuit->get(p));
                    } else {
                        noisy_circuit->add(circuit->get(p));
                        noisy_circuit->add(g);
                    }
                }
            } else {
                noisy_circuit->add(circuit->get(p));
            }
        }
        __verbose__ println("    [+] total injected errors in circuit '",
                            circuit->id(), "': ", total_errors);

        return noisy_circuit;
    }

    /**
     * \brief total errors
     * \return total errors
     */
    size_t get_total_errors() { return total_errors; }

    /**
     * overall probability of error
     */
    double get_overall_error_probability() { return overall_error_probability; }

    /**
     * dump
     */
    void dump() {
        println("   [+] depolarizing channel :");
        println("   [-] single qubit error probability : ", pe);
        for (size_t i = 1; i < (nq + 1); ++i) {
            double pp = error_probability(nq, i, pe);
            println("   [i] simultaneous error(s) probability of ", i,
                    " qubits out of ", nq, " : ", pp);
        }
        println("   [-] overall probability of errors: ",
                overall_error_probability);
        println("   [-] probability of (x) errors: ", xp);
        println("   [-] probability of (z) errors: ", yp);
        println("   [-] probability of (y) errors: ", zp);
    }

private:
    /**
     * uniform random number generator
     */
    double uniform_rand() { return urg.next(); }

    /**
     * normal random number generator
     */
    double normal_rand() {
        double r = 1 - std::abs(nrg.next());
        r = (r < 0 ? 0 : (r > __limit__ ? __limit__ : r));
        return r;
    }

    /**
     * single qubit error
     */
    std::shared_ptr<qx::gate> single_qubit_error(size_t q,
                                                 bool verbose = false) {
        double p = uniform_rand();
        if (p < xp) {
            __verbose__ println(" (x error) ");
            if (error_recording)
                errors.push_back(error_t(__x_error__, q));
            x_errors++;
            return std::make_shared<qx::pauli_x>(q);
        } else if (p < (zp + xp)) {
            __verbose__ println(" (z error) ");
            if (error_recording)
                errors.push_back(error_t(__z_error__, q));
            z_errors++;
            return std::make_shared<qx::pauli_z>(q);
        } else {
            __verbose__ println(" (y error) ");
            if (error_recording)
                errors.push_back(error_t(__y_error__, q));
            y_errors++;
            return std::make_shared<qx::pauli_y>(q);
        }
    }

    /**
     * measurement error
     */
    std::shared_ptr<qx::gate> measurement_error(size_t q,
                                                bool verbose = false) {
        __verbose__ println(" (measurement error) ");
        if (error_recording)
            errors.push_back(error_t(__x_error__, q));
        return std::make_shared<qx::pauli_x>(q);
    }

    /**
     * factorial
     */
    size_t factorial(size_t n) {
        return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
    }

    /**
     * combinations
     */
    size_t combinations(size_t n, size_t k) {
        return (factorial(n) / (factorial(k) * factorial(n - k)));
    }

    /**
     * nq : total number of qubits
     * ne : number of simultaneously affected qubits
     * p  : probability of single error
     */
    double error_probability(size_t nq, size_t ne, double p) {
        double c = (double)combinations(nq, ne);
        double res = c * std::pow(p, ne) * std::pow(1 - p, nq - ne);
        return res;
    }

private:
    /**
     * parameters
     */

    qx::normal_random_number_generator nrg;
    qx::uniform_random_number_generator urg;

    std::shared_ptr<qx::circuit> const& circuit;
    size_t nq;

    double pe;
    double xp;
    double yp;
    double zp;

    double overall_error_probability;
    size_t total_errors;
    histogram_t error_histogram;

    bool error_recording;
    std::vector<error_t> errors;
    std::vector<size_t> error_location;
    size_t x_errors = 0;
    size_t y_errors = 0;
    size_t z_errors = 0;
};
} // namespace qx