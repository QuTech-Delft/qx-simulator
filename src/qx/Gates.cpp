#include "qx/Gates.hpp"


namespace qx::gates {

__CONSTEXPR__ UnitaryMatrix<2> RX(double theta) {
    return UnitaryMatrix<2>(
        {{{std::cos(theta / 2), -1i * std::sin(theta / 2)},
            {-1i * std::sin(theta / 2), std::cos(theta / 2)}}});
}

__CONSTEXPR__ UnitaryMatrix<2> RY(double theta) {
    return UnitaryMatrix<2>({{{std::cos(theta / 2), -std::sin(theta / 2)},
                                {std::sin(theta / 2), std::cos(theta / 2)}}});
}

__CONSTEXPR__ UnitaryMatrix<2> RZ(double theta) {
    return UnitaryMatrix<2>(
        {{{std::cos(theta / 2) - 1i * std::sin(theta / 2), 0},
            {0, std::cos(theta / 2) + 1i * std::sin(theta / 2)}}});
}

__CONSTEXPR__ UnitaryMatrix<4> CR(double theta) {
    return UnitaryMatrix<4>(
        {{{1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, std::cos(theta) + 1i * std::sin(theta)}}});
}

}  // namespace qx::gates
