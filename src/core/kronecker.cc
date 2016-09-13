#include <iostream>
#include <vector>
#include <complex>
#include <cstring>

#include <kronecker.h>

#define println(x) std::cout << x << std::endl
#define print(x) std::cout << x 


using namespace qx::linalg;


int main(int argc, char **argv)
{
   matrix_t m(2,row_t(2,0.0));
   m[0][0] = 0.1; m[0][1] = 0.4;
   m[1][0] = 0.5; m[1][1] = 0.7;

   cmatrix_t cm(2,2);
   cm(0,0) = 0.1; cm(0,1) = 0.4;
   cm(1,0) = 0.5; cm(1,1) = 0.7;
   cm.dump();

   identity       id1(4);
   unitary_matrix gate(2,m);
   identity       id2(2);

   //kronecker k(&id1, &gate, &id2);
   kronecker k(&gate, &id1); //, &id2);

   size_t ks = 2*4; //*2;
   for (int i=0; i<ks; i++)
   {
      print("[ ");
      for (int j=0; j<ks; j++)
	 print(k.get(j,i).re << ", ");
      println("]");
   }

   println("-----------------");

   kronecker_ui kui(cm,2,4);
   for (int i=0; i<ks; i++)
   {
      print("[ ");
      for (int j=0; j<ks; j++)
	 print(kui.get(j,i).re << ", ");
      println("]");
   }

   cvector_t v(ks,0.0);
   cvector_t r(ks,0.0);

   v[0] = 1;

   // mulmv(k,v,r);
   mulmv(k,v,r,0,7,0,7);   // tile 1
   mulmv(k,v,r,0,7,8,15);  // tile 2
   mulmv(k,v,r,8,15,0,7);  // tile 3
   mulmv(k,v,r,8,15,8,15); // tile 4

   printv(v);
   printv(r);

   return 0;
}

