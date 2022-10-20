#pragma once

#include <cstdlib>
#include <string>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <mm_malloc.h>
#define QX_ALIGNED_MEMORY_MALLOC _mm_malloc
#define QX_ALIGNED_MEMORY_FREE _mm_free
#else
#define QX_ALIGNED_MEMORY_MALLOC _aligned_malloc
#define QX_ALIGNED_MEMORY_FREE _aligned_free
#endif

#include <new>

namespace xpu {

template <typename T, std::size_t N = 16> class aligned_memory_allocator {
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    typedef T *pointer;
    typedef const T *const_pointer;

    typedef T &reference;
    typedef const T &const_reference;

public:
    inline aligned_memory_allocator() throw() {}

    template <typename T2>
    inline aligned_memory_allocator(
        const aligned_memory_allocator<T2, N> &) throw() {}

    inline ~aligned_memory_allocator() throw() {}

    inline pointer address(reference r) { return &r; }

    inline const_pointer address(const_reference r) const { return &r; }

    inline pointer allocate(size_type n) {
        pointer rv =
            (pointer)QX_ALIGNED_MEMORY_MALLOC(n * sizeof(value_type), N);
        if (NULL == rv) {
            throw std::bad_alloc();
        }
        return rv;
    }

    inline void deallocate(pointer p, size_type) { QX_ALIGNED_MEMORY_FREE(p); }

    inline void construct(pointer p, const value_type &wert) {
        new (p) value_type(wert);
    }

    inline void destroy(pointer p) { p->~value_type(); }

    inline size_type max_size() const throw() {
        return size_type(-1) / sizeof(value_type);
    }

    template <typename T2> struct rebind {
        typedef aligned_memory_allocator<T2, N> other;
    };

    bool operator!=(const aligned_memory_allocator<T, N> &other) const {
        return !(*this == other);
    }

    // Returns true if and only if storage allocated from *this
    // can be deallocated from other, and vice versa.
    // Always returns true for stateless allocators.
    bool operator==(const aligned_memory_allocator<T, N> &other) const {
        return true;
    }
};

} // namespace xpu