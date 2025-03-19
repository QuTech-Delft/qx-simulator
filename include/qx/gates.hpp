#pragma once

#include <cmath>  // pow
#include <functional>  // function
#include <numbers>  // pi_v, sqrt2_v
#include <unordered_map>

#include "qx/cqasm_v3x.hpp"
#include "qx/dense_unitary_matrix.hpp"

namespace qx::gates {

using Matrix = core::Matrix;
using UnitaryMatrix = core::DenseUnitaryMatrix;

using namespace std::complex_literals;  // i

// clang-format off

inline constexpr long double PI = std::numbers::pi_v<long double>;
inline constexpr double SQRT_2 = std::numbers::sqrt2_v<long double>;

static UnitaryMatrix IDENTITY = UnitaryMatrix::identity(2);

static UnitaryMatrix X{
    Matrix{
        { 0, 1 },
        { 1, 0 }
    },
    false
};

static UnitaryMatrix Y{
    Matrix{
        {  0, -1i },
        { 1i,   0 }
    },
    false
};

static UnitaryMatrix Z{
    Matrix{
        { 1,  0 },
        { 0, -1 }
    },
    false
};

static UnitaryMatrix S{
    Matrix{
        { 1,  0 },
        { 0, 1i }
    },
    false
};

static UnitaryMatrix SDAG = S.dagger();

static UnitaryMatrix T{
    Matrix{
        { 1,                        0 },
        { 0, 1 / SQRT_2 + 1i / SQRT_2 }
    },
    false
};

static UnitaryMatrix TDAG = T.dagger();

inline UnitaryMatrix RX(double theta) {
    return UnitaryMatrix{
        Matrix{
            {       std::cos(theta / 2), -1i * std::sin(theta / 2) },
            { -1i * std::sin(theta / 2),       std::cos(theta / 2) }
        },
        false
    };
}

inline UnitaryMatrix RY(double theta) {
    return UnitaryMatrix{
        Matrix{
            { std::cos(theta / 2), -std::sin(theta / 2) },
            { std::sin(theta / 2),  std::cos(theta / 2) }
        },
        false
    };
}

inline UnitaryMatrix RZ(double theta) {
    return UnitaryMatrix{
        Matrix{
            { std::cos(theta / 2) - 1i * std::sin(theta / 2),                                              0 },
            {                                              0, std::cos(theta / 2) + 1i * std::sin(theta / 2) }
        },
        false
    };
}

static auto X90 = RX(PI / 2);
static auto Y90 = RY(PI / 2);
static auto Z90 = RZ(PI / 2);
static auto MX90 = RX(-PI / 2);
static auto MY90 = RY(-PI / 2);
static auto MZ90 = RZ(-PI / 2);

static UnitaryMatrix H{
    Matrix{
        { 1 / SQRT_2,  1 / SQRT_2 },
        { 1 / SQRT_2, -1 / SQRT_2 }
    },
    false
};

static UnitaryMatrix CNOT{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 1 },
        { 0, 0, 1, 0 }
    },
    false
};

static UnitaryMatrix SWAP{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 1 }
    },
    false
};

static UnitaryMatrix CZ{
    Matrix{
        { 1, 0, 0,  0 },
        { 0, 1, 0,  0 },
        { 0, 0, 1,  0 },
        { 0, 0, 0, -1 }
    },
    false
};

inline UnitaryMatrix CR(double theta) {
    return UnitaryMatrix{
        Matrix{
            { 1, 0, 0,                                      0 },
            { 0, 1, 0,                                      0 },
            { 0, 0, 1,                                      0 },
            { 0, 0, 0, std::cos(theta) + 1i * std::sin(theta) }
        },
        false
    };
}

static UnitaryMatrix TOFFOLI{
    Matrix{
        { 1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 1 },
        { 0, 0, 0, 0, 0, 0, 1, 0 }
    },
    false
};

using GateName = std::string;
using UnitaryMatrixGenerator = std::function<gates::UnitaryMatrix(CqasmV3xParameters)>;

static std::unordered_map<GateName, UnitaryMatrixGenerator> default_gates = {
    { "CNOT", [](const auto&) { return gates::CNOT; } },
    { "CR", [](const auto& parameters) { return gates::CR(parameters[0]->as_const_float()->value); } },
    { "CRk", [](const auto& parameters) {
        return gates::CR(static_cast<double>(gates::PI) / std::pow(2, parameters[0]->as_const_int()->value - 1));
    } },
    { "CZ", [](const auto&) { return gates::CZ; } },
    { "H", [](const auto&) { return gates::H; } },
    { "I", [](const auto&) { return gates::IDENTITY; } },
    { "mX90", [](const auto&) { return gates::MX90; } },
    { "mY90", [](const auto&) { return gates::MY90; } },
    { "Rx", [](const auto& parameters) { return gates::RX(parameters[0]->as_const_float()->value); } },
    { "Ry", [](const auto& parameters) { return gates::RY(parameters[0]->as_const_float()->value); } },
    { "Rz", [](const auto& parameters) { return gates::RZ(parameters[0]->as_const_float()->value); } },
    { "S", [](const auto&) { return gates::S; } },
    { "Sdag", [](const auto&) { return gates::SDAG; } },
    { "SWAP", [](const auto&) { return gates::SWAP; } },
    { "T", [](const auto&) { return gates::T; } },
    { "Tdag", [](const auto&) { return gates::TDAG; } },
    { "TOFFOLI", [](const auto&) { return gates::TOFFOLI; } },
    { "X", [](const auto&) { return gates::X; } },
    { "X90", [](const auto&) { return gates::X90; } },
    { "Y", [](const auto&) { return gates::Y; } },
    { "Y90", [](const auto&) { return gates::Y90; } },
    { "Z", [](const auto&) { return gates::Z; } }
};

// clang-format on

}  // namespace qx::gates
