#pragma once

#include "qx/core/register.h"

namespace qx {
/**
 * \brief density operator
 */
class density_operator {
private:
    std::vector<double> probabilities;
    std::vector<qx::qu_register *> states;
    size_t state_dim;

public:
    /**
     * \brief constructor
     */
    density_operator() : state_dim(0) {}

    /**
     * \brief add a new state
     */
    bool add_state(qx::qu_register *state, double probability) {
        if ((state_dim != 0) && (state->states() != state_dim)) {
            println("[x] error : density_operator: cannot add the new state, "
                    "mixed state dimensions are incompatible !");
            return false;
        }

        if (state_dim == 0)
            state_dim = state->states();

        states.push_back(state);
        probabilities.push_back(probability);

        return true;
    }

    /**
     * \brief number of mixed states
     */
    size_t num_states() { return states.size(); }

    /**
     * \brief return density matrix
     */
    linalg::matrix<complex_t> *density_matrix() {
        linalg::matrix<complex_t> *dm =
            new linalg::matrix<complex_t>(state_dim, state_dim);
        linalg::matrix<complex_t> &rdm = *dm;
        for (size_t i = 0; i < states.size(); ++i) {
            cvector_t &state = states[i]->get_data();
            double p = probabilities[i];
            for (size_t j = 0; j < state_dim; ++j)
                for (size_t k = 0; k < state_dim; ++k) {
                    // println("[", j, ",", k, "] -> ", "[",
                    // state[j], " x ", std::conj(state[k]), "]", (p *
                    // state[j] * std::conj(state[k])));
                    rdm(j, k) += p * state[j] * std::conj(state[k]);
                }
        }
        return dm;
    }

    /**
     * \brief return density matrix element (i,j)
     */
    complex_t density_matrix(size_t r, size_t c) {
        assert(r < state_dim);
        assert(c < state_dim);
        complex_t res = 0.0;
        for (size_t i = 0; i < states.size(); ++i) {
            cvector_t &state = states[i]->get_data();
            double p = probabilities[i];
            res += p * state[r] * std::conj(state[c]);
        }
        return res;
    }

    /**
     * \brief return purity
     */
    double purity() {
        double f = 0;
        for (size_t i = 0; i < states.size(); ++i)
            f += probabilities[i] * probabilities[i];
        for (size_t i = 0; i < states.size(); ++i) {
            linalg::cvector_t &vi = states[i]->get_data();
            double pi = probabilities[i];
            for (size_t j = 0; j < states.size(); ++j) {
                complex_t ip = 0.0;
                complex_t g = 0.0;
                linalg::cvector_t &vj = states[j]->get_data();
                double pj = probabilities[j];

                for (size_t s = 0; s < state_dim; ++s)
                    ip += vi[s] * vj[s];

                for (size_t s = 0; s < state_dim; ++s) {
                    g = pi * pj * ip * vi[s] * std::conj(vj[s]);
                    f += 2 * g.real();
                }
            }
        }

        return f;
    }
};

} // namespace qx