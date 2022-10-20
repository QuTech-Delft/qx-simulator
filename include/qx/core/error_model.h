#pragma once

#include "qx/core/depolarizing_channel.h"
#include "qx/core/error_injector.h"

namespace qx {
/**
 * error_model_t
 */
typedef enum __error_model_t {
    __depolarizing_channel__,
    __amplitude_damping__,
    __unknown_error_model__
} error_model_t;

std::shared_ptr<qx::circuit> noisy_dep_ch(std::shared_ptr<qx::circuit> const& c,
                                          double p, size_t &total_errors) {
    if (c) {
        qx::depolarizing_channel dep_ch(c, c->get_qubit_count(), p);
        auto noisy_c = dep_ch.inject();
        total_errors += dep_ch.get_total_errors();
        return noisy_c;
    }
    return {};
}

}; // namespace qx