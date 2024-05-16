#pragma once

#include <stdexcept>  // runtime_error
#include <string>


namespace qx {

struct SimulationError : public std::runtime_error {
    explicit SimulationError(const std::string &message)
        : std::runtime_error{ message }
    {}
};

}  // namespace qx
