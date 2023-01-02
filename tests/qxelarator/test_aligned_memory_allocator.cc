#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/xpu/aligned_memory_allocator.h"
#include "qx/xpu/complex.h"
#include "doctest/doctest.h"

namespace qx {

bool is_aligned(const void* ptr, std::uintptr_t alignment) {
    auto iptr = reinterpret_cast<std::uintptr_t>(ptr);
    return !(iptr % alignment);
}

TEST_CASE("Allocate/deallocate and check alignment") {
    ::xpu::aligned_memory_allocator<::xpu::complex_d, 64ul> allocator;

    auto p = allocator.allocate(1);
    CHECK(is_aligned(p, 64ul));
    allocator.deallocate(p, 0);

    p = allocator.allocate(2);
    CHECK(is_aligned(p, 64ul));
    allocator.deallocate(p, 0);

    p = allocator.allocate(456);
    CHECK(is_aligned(p, 64ul));
    allocator.deallocate(p, 0);
}

TEST_CASE("Nested allocations") {
    ::xpu::aligned_memory_allocator<::xpu::complex_d, 64ul> allocator;

    auto p1 = allocator.allocate(1);
    auto p2 = allocator.allocate(3);
    auto p3 = allocator.allocate(2);
    auto p4 = allocator.allocate(234235);
    allocator.deallocate(p3, 0);
    allocator.deallocate(p2, 0);
    allocator.deallocate(p1, 0);
    allocator.deallocate(p4, 0);
}
}