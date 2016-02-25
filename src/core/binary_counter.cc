
#include <stdint.h>
#include <iostream>
#include <bitset>
#include <cassert>


#define println(x) std::cout << x << std::endl
#define print(x) std::cout << x 



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

void non_nul_operations(uint32_t n, uint32_t qubit)
{
   uint32_t k = qubit;
   uint32_t rows = (1 << n);
   binary_counter bc(n);
   for (uint32_t r=0; r<rows; r++)
   {
      bc = r;
      bc.unset(n-k);
      uint32_t c1 = bc.value();
      bc.set(n-k);
      uint32_t c2 = bc.value();
      println(c1 << "," << c2);
   }
}

int main(int argc, char**argv)
{
   binary_counter bc(8);

   bc.dump();
   println("set bit 3:");
   bc.set(3);
   bc.dump();
   println("set bit 5:");
   bc.set(5);
   bc.dump();
   println("unset bit 3:");
   bc.unset(3);
   bc.dump();
   println("reset:");
   bc.reset();
   bc.dump();
   println("increment:");
   bc++;
   bc.dump();
   println("set value to 33:");
   bc = 33;
   bc.dump();

   println("single qubit operation (qubits=4, target = 2) :");
   non_nul_operations(4,2);

   println("single qubit operation (qubits=2, target = 0) :");
   non_nul_operations(2,1);

   return 0;
}
