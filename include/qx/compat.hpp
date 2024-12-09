#pragma once

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
