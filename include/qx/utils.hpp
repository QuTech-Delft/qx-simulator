#pragma once

#include <array>
#include <cassert>
#include <climits>
#include <string>


namespace qx::utils {

static inline bool get_bit(std::size_t x, std::size_t index) {
    return (x >> index) & 1;
}

}  // namespace qx::utils
