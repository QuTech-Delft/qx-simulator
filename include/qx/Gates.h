#pragma once

#include "qx/Core.h"

namespace qx {
namespace gates {

template <std::size_t N> using UnitaryMatrix = core::DenseUnitaryMatrix<N>;

using namespace std::complex_literals;

// All those matrices can currently be constexpr with GCC, but not Clang.

#if !defined(_MSC_VER) && !defined(__clang__)
#define __CONSTEXPR__ constexpr
#else
#define __CONSTEXPR__
#endif

static __CONSTEXPR__ long double PI = 3.141592653589793238462643383279502884L;
static __CONSTEXPR__ double SQRT_2 = 1.414213562373095048801688724209698078L;

static __CONSTEXPR__ UnitaryMatrix<2> IDENTITY = UnitaryMatrix<2>::identity();

static __CONSTEXPR__ UnitaryMatrix<2> X({{{0, 1}, {1, 0}}});

static __CONSTEXPR__ UnitaryMatrix<2> Y({{{0, -1i}, {1i, 0}}});

static __CONSTEXPR__ UnitaryMatrix<2> Z({{{1, 0}, {0, -1}}});

static __CONSTEXPR__ UnitaryMatrix<2> S({{{1, 0}, {0, 1i}}});

static __CONSTEXPR__ UnitaryMatrix<2> SDAG = S.dagger();

static __CONSTEXPR__ UnitaryMatrix<2>
    T({{{1, 0}, {0, 1 / SQRT_2 + 1i / SQRT_2}}});

static __CONSTEXPR__ UnitaryMatrix<2> TDAG = T.dagger();

__CONSTEXPR__ UnitaryMatrix<2> RX(double theta) {
    return UnitaryMatrix<2>(
        {{{std::cos(theta / 2), -1i * std::sin(theta / 2)},
          {-1i * std::sin(theta / 2), std::cos(theta / 2)}}});
}

static __CONSTEXPR__ auto X90 = RX(PI / 2);
static __CONSTEXPR__ auto MX90 = RX(-PI / 2);

__CONSTEXPR__ UnitaryMatrix<2> RY(double theta) {
    return UnitaryMatrix<2>({{{std::cos(theta / 2), -std::sin(theta / 2)},
                              {std::sin(theta / 2), std::cos(theta / 2)}}});
}

static __CONSTEXPR__ auto Y90 = RY(PI / 2);
static __CONSTEXPR__ auto MY90 = RY(-PI / 2);

__CONSTEXPR__ UnitaryMatrix<2> RZ(double theta) {
    return UnitaryMatrix<2>(
        {{{std::cos(theta / 2) - 1i * std::sin(theta / 2), 0},
          {0, std::cos(theta / 2) + 1i * std::sin(theta / 2)}}});
}

static __CONSTEXPR__ UnitaryMatrix<2>
    H({{{1 / SQRT_2, 1 / SQRT_2}, {1 / SQRT_2, -1 / SQRT_2}}});

static __CONSTEXPR__ UnitaryMatrix<4>
    CNOT({{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}}});

static __CONSTEXPR__ UnitaryMatrix<4>
    SWAP({{{1, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 1}}});

static __CONSTEXPR__ UnitaryMatrix<4>
    CZ({{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, -1}}});

__CONSTEXPR__ UnitaryMatrix<4> CR(double theta) {
    return UnitaryMatrix<4>(
        {{{1, 0, 0, 0},
          {0, 1, 0, 0},
          {0, 0, 1, 0},
          {0, 0, 0, std::cos(theta) + 1i * std::sin(theta)}}});
}

static __CONSTEXPR__ UnitaryMatrix<8> TOFFOLI({{{1, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 1, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 1, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 1, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 1, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 1, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 1},
                                                {0, 0, 0, 0, 0, 0, 1, 0}}});

// TODO: make this even more user-friently with ctrl operator and multiplication
// by double.

#if !defined(_MSC_VER) && !defined(__clang__)
static_assert(T * T == S);
static_assert(H * H == UnitaryMatrix<2>::identity());
static_assert(S * S == Z);
#endif

} // namespace gates
} // namespace qx