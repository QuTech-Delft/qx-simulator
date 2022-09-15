#pragma once

#if defined(_MSC_VER)
#define QX_ALIGNED(x) __declspec(align(x))
#else
#define QX_ALIGNED(x) __attribute__((aligned(x)))
#endif

// MSVC doesn't define __SSE__, so just assume it is available...
#if defined(_MSC_VER)
#define __SSE__
#endif

// Availability of M_PI is iffy, so just define our own.
#define QX_PI 3.141592653589793238462643383279502884L

// Really?
// https://stackoverflow.com/questions/5004858/why-is-stdmin-failing-when-windows-h-is-included
#if defined(_MSC_VER)
#define NOMINMAX
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

// srand48 doesn't exist on Windows.
#ifdef _MSC_VER
#define QX_SRAND srand
#else
#define QX_SRAND srand48
#endif
