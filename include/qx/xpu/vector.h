#pragma once

#include "qx/xpu/aligned_memory_allocator.h"
#include <cstring>
#include <iostream>

#define __xpu_bound_check__ 0

namespace xpu {
// Class vector makes an aligned dynamic array
template <typename __T, size_t __A = 16> class vector {
public:
    /**
     * constructor
     */
    inline vector() : __size(0), __data(0) {}

    /**
     * constructor
     */
    inline vector(size_t n) : __size(n) { __data = allocator.allocate(n); }

    /**
     * destructor
     */
    ~vector() {
        if (__data)
            allocator.deallocate(__data, __size);
    }

    /**
     * allocate buffer for future growth to num objects
     */
    inline void allocate(size_t n) {
        __size = n;
        __data = allocator.allocate(n);
    }

    /**
     * return element count
     */
    inline size_t size() { return __size; }

    /**
     * return data
     */
    inline __T *data() { return __data; }

    /**
     * return ith element
     */
    inline __T &operator[](size_t i) {
#if __xpu_bound_check__
        if (i >= __size) {
            std::cerr << "[x] xpu::vector : fatal error : index out of bound !"
                      << std::endl;
            exit(-1);
        }
#endif
        return __data[i];
    }

    /**
     * swap data pointer
     */
    inline void swap(vector<__T, __A> &v) {
        __T *__data_tmp = __data;
        size_t __size_tmp = __size;
        __data = v.__data;
        __size = v.__size;
        v.__data = __data_tmp;
        v.__size = __size_tmp;
    }

    /**
     * resize
     */
    void resize(size_t n) {
        if (__size != n) {
            if (__data)
                allocator.deallocate(__data, __size);
            __data = allocator.allocate(n);
            __size = n;
        }
    }

    /**
     * operator =
     */
    inline vector<__T, __A> &operator=(vector<__T, __A> &v) {
        std::cout << "xpu::vector::operator=" << std::endl;
        if (__data)
            allocator.deallocate(__data, __size);
        __data = allocator.allocate(v.__size);
        memcpy((void *)__data, (const void *)(v.__data),
               (v.__size * sizeof(__T)));
        __size = v.__size;
        return *this;
    }

protected:
    aligned_memory_allocator<__T, __A> allocator;

private:
    size_t __size;
    __T *__data;
};

} // namespace xpu