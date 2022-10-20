#pragma once

#include <iostream>

template <typename... Args> constexpr void print(Args... x) {
    int dummy[] = {0, (std::cout << x, 0)...};
    (void) dummy;
}

template <typename... Args> constexpr void println(Args... x) {
    print(x...);
    std::cout << std::endl;
}
