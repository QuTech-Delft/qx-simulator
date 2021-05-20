#pragma once

#if defined(_MSC_VER)
#define QX_ALIGNED(x) __declspec(align(x))
#else
#define QX_ALIGNED(x) __attribute__ ((aligned(x)))
#endif
