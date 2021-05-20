#ifndef QX_BINARY_COUNTER_H
#define QX_BINARY_COUNTER_H

#include <stdint.h>
#include <iostream>
#include <bitset>
#include <cassert>

#ifndef println
#define println(x) std::cout << x << std::endl
#define print(x) std::cout << x 
#endif // println

namespace qx {

   class binary_counter
   {
      private:

	 uint64_t __val;
	 uint64_t __max;
	 uint64_t __num;

      public:

	 binary_counter(uint64_t bit_count)
	 {
	    assert(bit_count <= 64);
	    __num = bit_count;
	    __max = 1 << bit_count;
	    __val = 0;
	 }

	 void set(uint32_t b)
	 {
	    __val |= (1 << b);
	 }

	 void unset(uint32_t b)
	 {
	    __val &= ~(1 << b);
	 }

	 uint64_t test(uint32_t b)
	 {
	    return (__val & (1 << b));
	 }

	 void reset()
	 {
	    __val = 0;
	 }

	 uint64_t value()
	 {
	    return __val;
	 }

	 void dump()
	 {
	    print("[ ");
	    for (uint32_t i=__num; i>0;i--)
	       print((test(i-1) ? '1' : '0'));
	    println(" : " << __val << "]");
	 }

	 binary_counter& operator=(uint64_t v)
	 {
	    __val = v;
	    return *this;
	 }

	 binary_counter& operator++()
	 {
	    __val++;
	    return *this;
	 }

	 binary_counter operator++(int) 
	 {
	    binary_counter tmp(*this); // copy
	    operator++(); // pre-increment
	    return tmp;   // return old value
	 }

   };
} // qx


#endif // QX_BINARY_COUNTER_H
