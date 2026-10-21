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

inline constexpr double PI = std::numbers::pi_v<double>;
inline constexpr double SQRT_2 = std::numbers::sqrt2_v<double>;
inline constexpr double SQRT_3 = std::numbers::sqrt3_v<double>;

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

inline UnitaryMatrix U(double theta, double phi, double lambda) {
    return UnitaryMatrix{
        Matrix{
            { std::cos(theta / 2), -std::exp(1i * lambda) * std::sin(theta / 2) },
            { std::exp(1i * phi) * std::sin(theta / 2),
                std::exp(1i * (phi + lambda)) * std::cos(theta / 2) }
        },
        false
    };
}

inline UnitaryMatrix RN(double nx, double ny, double nz, double theta, double phi) {
    return UnitaryMatrix{
        Matrix{
            { 
                std::exp(1i * phi) * (std::cos(theta / 2) - 1i * nz * std::sin(theta / 2)),
                std::exp(1i * phi) * (-ny * std::sin(theta / 2) - 1i * nx * std::sin(theta / 2)) 
            },
            { 
                std::exp(1i * phi) * (ny * std::sin(theta / 2) - 1i * nx * std::sin(theta / 2)),
                std::exp(1i * phi) * (std::cos(theta / 2) + 1i * nz * std::sin(theta / 2))
            }
        },
        false
    };
}

static UnitaryMatrix X90{
    Matrix{
        { 1/2. + 1i/2., 1/2. - 1i/2. },
        { 1/2. - 1i/2., 1/2. + 1i/2. }
    },
    false
};
static UnitaryMatrix MX90{
    Matrix{
        { 1/2. - 1i/2., 1/2. + 1i/2. },
        { 1/2. + 1i/2., 1/2. - 1i/2. }
    },
    false
};
static UnitaryMatrix Y90{
    Matrix{
        { 1/2. + 1i/2., -1/2. - 1i/2. },
        { 1/2. + 1i/2.,  1/2. + 1i/2. }
    },
    false
};
static UnitaryMatrix MY90{
    Matrix{
        {  1/2. - 1i/2., 1/2. - 1i/2. },
        { -1/2. + 1i/2., 1/2. - 1i/2. }
    },
    false
};
static UnitaryMatrix Z90 = S;
static UnitaryMatrix MZ90 = SDAG;


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

static UnitaryMatrix CV{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, (1. + 1i) / 2., (1. - 1i) / 2. },
        { 0, 0, (1. - 1i) / 2., (1. + 1i) / 2. }
    },
    false
};

static UnitaryMatrix CY{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, -1i },
        { 0, 0, 1i, 0 }
    },
    false
};

static UnitaryMatrix DCNOT{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
        { 0, 1, 0, 0 }
    },
    false
};

static UnitaryMatrix ECR{
    Matrix{
        { 0, 0, 1 / SQRT_2,  1i / SQRT_2 },
        { 0, 0, 1i / SQRT_2, 1 / SQRT_2 },
        { 1 / SQRT_2, -1i / SQRT_2, 0, 0 },
        { -1i / SQRT_2, 1 / SQRT_2, 0, 0 }
    },
    false
};

static UnitaryMatrix INVSQRTSWAP{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, (1. - 1i) / 2., (1. + 1i) / 2., 0 },
        { 0, (1. + 1i) / 2., (1. - 1i) / 2., 0 },
        { 0, 0, 0, 1 }
    },
    false
};

static UnitaryMatrix ISWAP{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, 0, 1i, 0 },
        { 0, 1i, 0, 0 },
        { 0, 0, 0, 1 }
    },
    false
};

static UnitaryMatrix M{
    Matrix{
        { 1 / SQRT_2,  1i / SQRT_2, 0, 0 },
        { 0, 0, 1i / SQRT_2, 1 / SQRT_2 },
        { 0, 0, 1i / SQRT_2, -1 / SQRT_2 },
        { 1 / SQRT_2, -1i / SQRT_2, 0, 0 }
    },
    false
};

static UnitaryMatrix MS{
    Matrix{
        { 1 / SQRT_2, 0, 0, 1i / SQRT_2 },
        { 0, 1 / SQRT_2, 1i / SQRT_2, 0 },
        { 0, 1i / SQRT_2, 1 / SQRT_2, 0 },
        { 1i / SQRT_2, 0, 0, 1 / SQRT_2 }
    },
    false
};

static UnitaryMatrix SQRTISWAP{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, 1 / SQRT_2, 1i / SQRT_2, 0 },
        { 0, 1i / SQRT_2, 1 / SQRT_2, 0 },
        { 0, 0, 0, 1 }
    },
    false
};

static UnitaryMatrix SQRTSWAP{
    Matrix{
        { 1, 0, 0, 0 },
        { 0, (1. + 1i) / 2., (1. - 1i) / 2., 0 },
        { 0, (1. - 1i) / 2., (1. + 1i) / 2., 0 },
        { 0, 0, 0, 1 }
    },
    false
};

static UnitaryMatrix CSWAP{
    Matrix{
        { 1, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 1 }
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
    { "CCNOT", [](const auto&) { return gates::TOFFOLI; } },
    { "CCX", [](const auto&) { return gates::TOFFOLI; } },
    { "CZ", [](const auto&) { return gates::CZ; } },
    { "CSWAP", [](const auto&) { return gates::CSWAP; } },
    { "CV", [](const auto&) { return gates::CV; } },
    { "CY", [](const auto&) { return gates::CY; } },
    { "DCNOT", [](const auto&) { return gates::DCNOT; } },
    { "ECR", [](const auto&) { return gates::ECR; } },
    { "H", [](const auto&) { return gates::H; } },
    { "I", [](const auto&) { return gates::IDENTITY; } },
    { "InvSqrtSWAP", [](const auto&) { return gates::INVSQRTSWAP; } },
    { "ISWAP", [](const auto&) { return gates::ISWAP; } },
    { "M", [](const auto&) { return gates::M; } },
    { "MS", [](const auto&) { return gates::MS; } },
    { "mX90", [](const auto&) { return gates::MX90; } },
    { "mY90", [](const auto&) { return gates::MY90; } },
    { "mZ90", [](const auto&) { return gates::MZ90; } },
    { "Rx", [](const auto& parameters) { return gates::RX(parameters[0]->as_const_float()->value); } },
    { "Ry", [](const auto& parameters) { return gates::RY(parameters[0]->as_const_float()->value); } },
    { "Rz", [](const auto& parameters) { return gates::RZ(parameters[0]->as_const_float()->value); } },
    { "Rn", [](const auto& parameters) { return gates::RN(
        parameters[0]->as_const_float()->value,
        parameters[1]->as_const_float()->value,
        parameters[2]->as_const_float()->value,
        parameters[3]->as_const_float()->value,
        parameters[4]->as_const_float()->value); } },
    { "S", [](const auto&) { return gates::S; } },
    { "Sdag", [](const auto&) { return gates::SDAG; } },
    { "SqrtISWAP", [](const auto&) { return gates::SQRTISWAP; } },
    { "SqrtSWAP", [](const auto&) { return gates::SQRTSWAP; } },
    { "SWAP", [](const auto&) { return gates::SWAP; } },
    { "T", [](const auto&) { return gates::T; } },
    { "Tdag", [](const auto&) { return gates::TDAG; } },
    { "TOFFOLI", [](const auto&) { return gates::TOFFOLI; } },
    { "Z90", [](const auto&) { return gates::Z90; } },
    { "U", [](const auto& parameters) { return gates::U(
        parameters[0]->as_const_float()->value,
        parameters[1]->as_const_float()->value,
        parameters[2]->as_const_float()->value); } },
    { "X", [](const auto&) { return gates::X; } },
    { "X90", [](const auto&) { return gates::X90; } },
    { "Y", [](const auto&) { return gates::Y; } },
    { "Y90", [](const auto&) { return gates::Y90; } },
    { "Z", [](const auto&) { return gates::Z; } }
};

// clang-format on

}  // namespace qx::gates
