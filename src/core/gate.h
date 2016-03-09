/**
 * @file		gate.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		02-10-15
 * @brief		
 */
#ifndef QX_GATE_H
#define QX_GATE_H

#include <map>

#include <xpu.h>

#include <core/hash_set.h>
#include <core/linalg.h>
#include <core/register.h>

#include <core/binary_counter.h>
#include <core/kronecker.h>

#ifndef __BUILTIN_LINALG__
#include <boost/numeric/ublas/matrix.hpp>
#endif

#define SQRT_2   (1.4142135623730950488016887242096980785696718753769480731766797379f)
#define R_SQRT_2 (0.7071067811865475244008443621048490392848359376884740365883398690f)

//#define SQRT_2   (1.41421356237309504880f)
//#define R_SQRT_2 (0.70710678118654752440f)

namespace qx
{
   /**
    * gates coeffecients
    */
   const complex_t cnot_c     [] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0 }; /* CNOT */
   const complex_t swap_c     [] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 }; /* SWAP */

   const complex_t identity_c [] = { 1, 0, 0, 1 };                               /* I */
   const complex_t pauli_x_c  [] = { complex_t(0, 0) , complex_t(1, 0), complex_t(1, 0) , complex_t(0, 0) };         /* X */
   const complex_t pauli_y_c  [] = { complex_t(0, 0) , complex_t(0,-1), complex_t(0, 1) , complex_t(0, 0) };         /* Y */
   const complex_t pauli_z_c  [] = { complex_t(1, 0) , complex_t(0, 0), complex_t(0, 0) , complex_t(-1,0) };         /* Z */
   const complex_t phase_c    [] = { complex_t(1, 0) , complex_t(0, 0), complex_t(0, 0) , complex_t(0, 1) };         /* S */
   const complex_t t_gate_c   [] = { complex_t(1, 0) , complex_t(0, 0), complex_t(0, 0) , complex_t(cos(M_PI/4),sin(M_PI/4)) };         /* T */
   const complex_t tdag_gate_c[] = { complex_t(1, 0) , complex_t(0, 0), complex_t(0, 0) , complex_t(cos(M_PI/4),-sin(M_PI/4)) };         /* T */
   const complex_t hadamard_c [] = { R_SQRT_2,  R_SQRT_2, R_SQRT_2, -R_SQRT_2 }; /* H */

   /**
    * \brief common abstract gate interface for
    *   all gates implementation.
    */
   class gate
   {
	 public:
	   
	   virtual int32_t                apply(qu_register& qureg) = 0;
	   virtual std::vector<uint32_t>  qubits() = 0;
	   virtual void                   dump() = 0;
	   virtual                        ~gate() { };                

	   virtual void                   set_duration(uint64_t d) { duration = d; }
	   virtual uint64_t               set_duration() { return duration; }
	 
	 protected:

	   uint64_t                       duration;

   };


   /**
    * \brief rotation in the x-z plane with a given 
    *     angle theta (see "Large scale simulation of 
    *     error-prone quantum systems" p.39" [Niwa 2002])
    */
   inline cmatrix_t rotation(double theta)
   {
	 cmatrix_t r(2,2);
	 r(0,0) = complex_t(cos(theta),0); r(0,1) = complex_t(-sin(theta),0);
	 r(1,0) = complex_t(sin(theta),0); r(1,1) = complex_t(cos(theta),0);
	 return r;
   }

   /**
    * \brief phase shift for a given angle phi
    */
   inline cmatrix_t phase(double phi)
   {
	 cmatrix_t p(2,2);
	 p(0,0) = complex_t(1,0); p(0,1) = complex_t(0,0);
	 p(1,0) = complex_t(0,0); p(1,1) = complex_t(cos(phi),sin(phi));
	 return p;
   }

   /**
    * \brief generate noisy hadamard gate
    */
   cmatrix_t noisy_hadamard(double epsilon1=0, double epsilon2=0)
   {
#ifdef __BUILTIN_LINALG__
	 return mxm(rotation(M_PI/4 + epsilon1), phase(M_PI + epsilon2));
#else
	 cmatrix_t rz = rotation(M_PI/4 + epsilon1);
	 cmatrix_t p  = phase(M_PI + epsilon2);
	 return mxm(rz,p);
#endif 
   }


   /**
    * \brief build n x n matrix from an array
    */
   cmatrix_t build_matrix(const complex_t * c, uint32_t n)
   {
	 cmatrix_t m(n,n);
	 for (int i=0; i<n; i++)
	    for (int j=0; j<n; j++) 
		  m(i,j) = c[i*n+j];
	 return m;
   }


   /**
    * sqg_apply
    */
   #ifdef QX_COMPACT_GATE_OP
 
   inline void sqg_apply(cmatrix_t & cm, uint32_t qubit, qu_register& qureg)
   {
	 uint32_t n  = qureg.size();

	 matrix_t m(2,row_t(2,0));
	 m[0][0] = cm(0,0); m[0][1] = cm(0,1);
	 m[1][0] = cm(1,0); m[1][1] = cm(1,1);

	 if (qubit == 0)
	 {
	    identity       id(1 << (n-1));
	    unitary_matrix um(cm.size1(),m);
	    kronecker      k(&id, &um);
	    cvector_t      r(qureg.get_data());
	    mulmv(k,qureg.get_data(),r);
	    qureg = r;
	 }
	 else if (qubit == n-1)
	 {
	    identity       id(1 << (n-1));
	    unitary_matrix um(cm.size1(),m);
	    kronecker      k(&um, &id);
	    cvector_t      r(qureg.get_data());
	    mulmv(k,qureg.get_data(),r);
	    qureg = r;
	 }
	 else
	 {
	    identity       id1(1 << (qubit));
	    identity       id2(1 << (n-qubit-1));
	    unitary_matrix um(cm.size1(),m);
	    kronecker      k(&id2, &um, &id1);
	    cvector_t      r(qureg.get_data());
	    mulmv(k,qureg.get_data(),r);
	    qureg = r;
	 }
   }

   /**
    * u on the kth qubit :
    * non-null value in each row of the kronocker matrix:
    *     for each row r :    
    *            c1 = r || 000100   // 1 at the n-k bit
    *            c2 = r || 000000
    */

   // #elif QX_SPARSE_MV_MUL
   #else // QX_SPARSE_MV_MUL
   
   uint32_t rw_process(int is, int ie, int s, uint32_t n, uint32_t qubit, kronecker * m, cvector_t * v, cvector_t * res)
   {
      uint32_t k = n-qubit;
      // println("run : " << is << " .. " << ie);

      for (uint32_t r=is; r<ie; ++r)
      {
	 binary_counter bc(n);
	 complex_t s = 0;
	 bc = r;
	 bc.unset(n-k);
	 uint32_t c1 = bc.value();
	 bc.set(n-k);
	 uint32_t c2 = bc.value();
	 s += v->at(c1)*(m->get(r,c1));
	 s += v->at(c2)*(m->get(r,c2));
	 res->at(r) = s;
      }

      return 0;
   }
      

   void sparse_mulmv(uint32_t n, uint32_t qubit, kronecker& m, cvector_t& v, cvector_t& res)
   {
      uint32_t k = n-qubit;
      uint32_t rows = (1 << n);
      uint32_t z = 0;

      xpu::task rw_t(rw_process,0,0,0,n,qubit,&m,&v,&res);
      xpu::parallel_for process(z,rows,1,&rw_t);

      process.run();

/*
      #pragma omp parallel for schedule(static)
      for (uint32_t r=0; r<rows; r++)
      {
	 binary_counter bc(n);
	 complex_t s = 0;
	 bc = r;
	 bc.unset(n-k);
	 uint32_t c1 = bc.value();
	 bc.set(n-k);
	 uint32_t c2 = bc.value();
	 //println("r=" << r << " : " << c1 << "," << c2);
	 //println("v=" << v[c1] << " x m=" << m.get(r,c1));
	 //println("v=" << v[c2] << " x m=" << m.get(r,c2));
	 s += v[c1]*(m.get(r,c1));
	 s += v[c2]*(m.get(r,c2));
	 //println("s=" << s);
	 res[r] = s;
      }
 */
   }



   inline void sqg_apply(cmatrix_t & cm, uint32_t qubit, qu_register& qureg)
   {
	 uint32_t n  = qureg.size();

	 matrix_t m(2,row_t(2,0));
	 m[0][0] = cm(0,0); m[0][1] = cm(0,1);
	 m[1][0] = cm(1,0); m[1][1] = cm(1,1);

	 if (qubit == 0)
	 {
	    identity       id(1 << (n-1));
	    unitary_matrix um(cm.size1(),m);
	    kronecker      k(&id, &um);
	    cvector_t      r(qureg.get_data());
	    sparse_mulmv(n,qubit,k,qureg.get_data(),r);
	    qureg = r;
	 }
	 else if (qubit == n-1)
	 {
	    identity       id(1 << (n-1));
	    unitary_matrix um(cm.size1(),m);
	    kronecker      k(&um, &id);
	    cvector_t      r(qureg.get_data());
	    sparse_mulmv(n,qubit,k,qureg.get_data(),r);
	    qureg = r;
	 }
	 else
	 {
	    identity       id1(1 << (qubit));
	    identity       id2(1 << (n-qubit-1));
	    unitary_matrix um(cm.size1(),m);
	    kronecker      k(&id2, &um, &id1);
	    cvector_t      r(qureg.get_data());
	    sparse_mulmv(n,qubit,k,qureg.get_data(),r);
	    qureg = r;
	 }
   }

#endif // remove naive tensor computation

/*
   #else

   inline void sqg_apply(cmatrix_t &m, uint32_t qubit, qu_register& qureg)
   {
	 uint32_t n  = qureg.size();
	 
	 if (qubit == 0)
	 {
	    cidentity_t i(1 << (n-1));
	    qureg = mxv(tensor(i,m), qureg.get_data());
	 }
	 else if (qubit == n-1)
	 {
	    cidentity_t i(1 << (n-1));
	    qureg = mxv(tensor(m,i), qureg.get_data());
	 }
	 else
	 {
	    cidentity_t i1(1 << (qubit));
	    cidentity_t i2(1 << (n-qubit-1));
	    qureg = mxv(tensor(tensor(i2,m),i1), qureg.get_data());
	 }
   }


   #endif // QX_COMPACT_GATE_OP

*/

   /**
    * \brief hadamard gate:
    *
    *               | 1   1|
    *     1/sqrt(2) |      |
    *               | 1  -1|
    */
   class hadamard : public gate
   {
	 private:

	   uint32_t                  qubit;
	   cmatrix_t  m;

	 public:
	   
	   hadamard(uint32_t qubit) : qubit(qubit)
	   {
		 m = build_matrix(hadamard_c,2);
	   }

	   int32_t apply(qu_register& qureg)
	   {
		 sqg_apply(m,qubit,qureg);
		 qureg.set_binary(qubit,__state_unknown__);
		 return 0;
	   }

	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

	   void dump()
	   {
		 println("  [-] hadamard(q=" << qubit << ")");
	   }

   };

   /**
    * \brief controlled-not gate:
    *
    *    | 1  0  0  0 | 
    *    | 0  1  0  0 |
    *    | 0  0  0  1 |
    *    | 0  0  1  1 |  
    */
   class cnot : public gate
   {
	 private:

	   uint32_t control_qubit;
	   uint32_t target_qubit;

	   cmatrix_t  m;

	 public:

	   cnot(uint32_t ctrl_q, uint32_t target_q) : control_qubit(ctrl_q), 
	                                              target_qubit(target_q)
	   {
		 m = build_matrix(cnot_c,4);
	   }
	   
	   // #define CG_HASH_SET
	   //#define CG_MATRIX
	   #ifndef CG_BC 
           #ifndef CG_MATRIX 
             #define CG_BC
           #endif
           #endif // CG_BC

	   int32_t apply(qu_register& qreg)
	   {
		 #ifdef CG_MATRIX
		 uint32_t sn = qreg.states();
		 uint32_t qn = qreg.size();
		 uint32_t cq = control_qubit;
		 uint32_t tq = target_qubit;

		 cmatrix_t i = cidentity_t(sn);
		 perm_t    p = perms(qn,cq,tq);
		 
		 // dump_matrix(i);

		 for (perm_t::iterator it = p.begin(); it != p.end(); it++)
		 {
		    i(it->first,it->second)  = 1;
		    i(it->second,it->first)  = 1;
		    i(it->first, it->first)  = 0;
		    i(it->second,it->second) = 0;
		 }

		 // dump_matrix(i);

	         qreg = mxv(i, qreg.get_data());

		 #elif defined(CG_BC)

		 uint32_t sn = qreg.states();
		 uint32_t qn = qreg.size();
		 uint32_t cq = control_qubit;
		 uint32_t tq = target_qubit;

		 cvector_t& amp = qreg.get_data();

		 //perm_t p = perms(qn,cq,tq);
		 perms(qn,cq,tq,amp);
/*
		 // std::vector<uint32_t> done(sn, 0);

		 
		 uint32_t p1,p2;

		 for (perm_t::iterator it = p.begin(); it != p.end(); it++)
		 {
		    p1 = it->first;
		    p2 = it->second;
		    // if (!(done[p1] || done[p2]))
		    // {
			  std::swap(amp[p1],amp[p2]);
			  //done[p1] = 1;
			  //done[p2] = 1;
		    //}
		 }
*/


		 // for (uint32_t i=0; i<sn; i++)


		 #elif defined(CG_HASH_SET)
		  
		 uint32_t j = control_qubit+1;
		 uint32_t k = target_qubit+1;

		 uint32_t k2 = (1 << (k-1));
		 uint32_t j2 = (1 << (j-1));
		 
		 uint32_t r_size = qreg.states(); 

		 xpu::container::hash_set<uint32_t> swap_set;

		 // find swap pairs
		 for (uint32_t t = 0; t < r_size; t++) 
		 {
		    if ((t & j2) <= 0) 
			  continue;

		    if (swap_set.find(t-k2) == swap_set.end())
			  swap_set.insert(t);
		 }
		 
		 int32_t t2;
		 cvector_t& amp = qreg.get_data();
		 complex_t c1, c2;

		 for (xpu::container::hash_set<uint32_t>::iterator t = swap_set.begin(); t != swap_set.end(); ++t) 
		 {
		    int32_t _t = *t;
		    t2 = (_t + k2 < r_size) ? _t + k2 : _t - k2;
		    c1 = amp(_t);
		    c2 = amp(t2);
		    std::swap(c1, c2);
		    amp(_t) = c1;
		    amp(t2) = c2;
		 }
		 //qreg=amp;
		 
		 #endif // CG_HASH_SET

		 if (qreg.get_binary(control_qubit) == __state_1__)
		    qreg.flip_binary(target_qubit);
		 else if (qreg.get_binary(control_qubit) == __state_unknown__)
		    qreg.set_binary(target_qubit,__state_unknown__);
		 
		 return 0;
	   }


	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(control_qubit);
		 r.push_back(target_qubit);
		 return r;
	   }
	   
	   void dump()
	   {
		 println("  [-] cnot(ctrl_qubit=" << control_qubit << ", target_qubit=" << target_qubit << ")");
	   }

   };


   /**
    * \brief toffoli gate:
    *
    *    | 1  0  0  0 | 
    *    | 0  1  0  0 |
    *    | 0  0  0  1 |
    *    | 0  0  1  1 |  
    */
   class toffoli : public gate
   {
	 private:

	   uint32_t control_qubit_1;
	   uint32_t control_qubit_2;
	   uint32_t target_qubit;

	 public:

	   toffoli(uint32_t ctrl_q1, uint32_t ctrl_q2, uint32_t target_q) : control_qubit_1(ctrl_q1), 
	                                                                 control_qubit_2(ctrl_q2),
									 target_qubit(target_q)
	   {
	   }
	   

	   int32_t apply(qu_register& qreg)
	   {
		 uint32_t sn  = qreg.states();
		 uint32_t qn  = qreg.size();
		 uint32_t cq1 = control_qubit_1;
		 uint32_t cq2 = control_qubit_2;
		 uint32_t tq = target_qubit;

		 cvector_t& amp = qreg.get_data();

		 std::vector<uint32_t> done(sn, 0);
		 perm_t p = perms(qn,cq1,cq2,tq);
		 
		 uint32_t p1,p2;

		 for (perm_t::iterator it = p.begin(); it != p.end(); it++)
		 {
		    p1 = it->first;
		    p2 = it->second;
		    if (!(done[p1] || done[p2]))
		    //if (!(done[p1]))
		    {
			  // std::swap(amp(p1),amp(p2)); // ublas
			  std::swap(amp[p1],amp[p2]);
			  done[p1] = 1;
			  done[p2] = 1;
		    }
		 }

		 if ((qreg.get_binary(control_qubit_1) == __state_1__) && 
		     (qreg.get_binary(control_qubit_2) == __state_1__) )
		 {
		    qreg.flip_binary(target_qubit);
		 }
		 else if ((qreg.get_binary(control_qubit_1) == __state_unknown__) ||
		          (qreg.get_binary(control_qubit_2) == __state_unknown__)  )
		 {
		    qreg.set_binary(target_qubit,__state_unknown__);
		 }
		 
		 return 0;
	   }


	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(control_qubit_1);
		 r.push_back(control_qubit_2);
		 r.push_back(target_qubit);
		 return r;
	   }
	   
	   void dump()
	   {
		 println("  [-] toffoli(ctrl_qubit_1=" << control_qubit_1 << ", ctrl_qubit_2=" << control_qubit_2 << ", target_qubit=" << target_qubit << ")");
	   }

   };


   
   /**
    * \brief  pauli-x :
    *
    *    | 0  1 |
    *    | 1  0 | 
    *
    */
   class pauli_x : public gate
   {
	 private:

	   uint32_t                  qubit;
	   cmatrix_t  m;

	 public:

	   pauli_x(uint32_t qubit) : qubit(qubit)
	   {
		 m = build_matrix(pauli_x_c,2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 qreg.flip_binary(qubit);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] pauli-x(qubit=" << qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };

   /**
    * \brief  pauli-y :
    *
    *    | 0 -i |
    *    | i  0 | 
    */
   class pauli_y : public gate
   {
	 private:

	   uint32_t                  qubit;
	   cmatrix_t  m;

	 public:

	   pauli_y(uint32_t qubit) : qubit(qubit)
	   {
		 m = build_matrix(pauli_y_c,2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] pauli-y(qubit=" << qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };

   /**
    * \brief  pauli-z :
    *
    *    | 1  0 |
    *    | 0 -1 | 
    */
   class pauli_z : public gate
   {
	 private:

	   uint32_t                  qubit;
	   cmatrix_t  m;

	 public:

	   pauli_z(uint32_t qubit) : qubit(qubit)
	   {
		 m = build_matrix(pauli_z_c,2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] pauli-z(qubit=" << qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };

   /**
    * \brief  phase :
    *     
    *    | 1  0 |
    *    | 0  i |
    */ 
   class phase_shift : public gate
   {
	 private:

	   uint32_t                  qubit;
	   cmatrix_t  m;

	 public:

	   phase_shift(uint32_t qubit) : qubit(qubit)
	   {
		 m = build_matrix(phase_c,2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] phase(qubit=" << qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };


   /**
    * \brief T gate
    */
   class t_gate : public gate
   {
	 private:

	   uint32_t   qubit;
	   cmatrix_t  m;

	 public:

	   t_gate(uint32_t qubit) : qubit(qubit)
	   {
		 m = build_matrix(t_gate_c,2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] t_gate(qubit=" << qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };


   /**
    * \brief T dag gate
    */
   class t_dag_gate : public gate
   {
	 private:

	   uint32_t   qubit;
	   cmatrix_t  m;

	 public:

	   t_dag_gate(uint32_t qubit) : qubit(qubit)
	   {
		 m = build_matrix(tdag_gate_c,2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] t_dag_gate(qubit=" << qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };






   /**
    * \brief  rotation-x :
    */ 
   class rx : public gate
   {
	 private:

	   uint32_t   qubit;
	   double     angle;
	   cmatrix_t  m;

	 public:

	   rx(uint32_t qubit, double angle) : qubit(qubit), angle(angle)
	   {
		 m.resize(2,2);
		 m(0,0) = cos(angle/2);      m(0,1) = complex_t(0,-sin(angle/2));
		 m(1,0) = complex_t(0,-sin(angle/2)); m(1,1) = cos(angle/2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 qreg.set_binary(qubit,__state_unknown__);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] rx(qubit=" << qubit << ", angle=" << angle << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };


   /**
    * \brief  rotation-y :
    */ 
   class ry : public gate
   {
	 private:

	   uint32_t   qubit;
	   double     angle;
	   cmatrix_t  m;

	 public:

	   ry(uint32_t qubit, double angle) : qubit(qubit), angle(angle)
	   {
		 m.resize(2,2);
		 m(0,0) = cos(angle/2); m(0,1) = -sin(angle/2);
		 m(1,0) = sin(angle/2); m(1,1) = cos(angle/2);
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 qreg.set_binary(qubit,__state_unknown__);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] ry(qubit=" << qubit << ", angle=" << angle << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };


   /**
    * \brief  rotation-z :
    */ 
   class rz : public gate
   {
	 private:

	   uint32_t   qubit;
	   double     angle;
	   cmatrix_t  m;

	 public:

	   rz(uint32_t qubit, double angle) : qubit(qubit), angle(angle)
	   {
		 m.resize(2,2);
		 m(0,0) = complex_t(cos(-angle/2), sin(-angle/2));   m(0,1) = 0;
		 m(1,0) = 0;  m(1,1) =  complex_t(cos(angle/2), sin(angle/2));
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 sqg_apply(m,qubit,qreg);
		 qreg.set_binary(qubit,__state_unknown__);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] rz(qubit=" << qubit << ", angle=" << angle << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };


   /**
    * \brief  controlled phase shift by (pi/(2^(ctrl-target)))
    *     
    */ 
   class ctrl_phase_shift : public gate
   {
	 private:

	   uint32_t                  ctrl_qubit;
	   uint32_t                  target_qubit;
	   complex_t                 z;

	 public:

	   ctrl_phase_shift(uint32_t ctrl_qubit, uint32_t target_qubit) : ctrl_qubit(ctrl_qubit), 
	                                                                  target_qubit(target_qubit), 
		      						          z(cos(M_PI/(1 << (ctrl_qubit - target_qubit))), 
								            sin(M_PI/(1 << (ctrl_qubit - target_qubit))))
	   {
	   }

	   int32_t apply(qu_register& qreg)
	   {
	      uint32_t nn = (1 << qreg.size());
	      uint32_t p = 0;
	      std::bitset<MAX_QB_N> b;

	      b.reset();
	      b.set(ctrl_qubit);  
	      b.set(target_qubit);  

	      uint32_t   bc    = b.to_ulong();
	      cvector_t& state = qreg.get_data();

	      while (bc < nn)
	      {
		 b.set(ctrl_qubit);  
		 b.set(target_qubit);  
		 p = b.to_ulong();
		 state[p] *= z;
		 b = inc(b);
		 bc = b.to_ulong();
	      }

	      return 0;
	   }

	   void dump()
	   {
		 println("  [-] ctrl_phase_shift(ctrl_qubit=" << ctrl_qubit << ", target_qubit: " << target_qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(ctrl_qubit);
		 r.push_back(target_qubit);
		 return r;
	   }

   };



   /**
    * \brief  swap :
    *
    *    | 1  0  0  0 | 
    *    | 0  0  1  0 |
    *    | 0  1  0  0 |
    *    | 0  0  0  1 |  
    */
   class swap : public gate
   {
	 private:
	   
	   uint32_t qubit1;
	   uint32_t qubit2;
	   
	   cmatrix_t m;

	 public:

	   swap(uint32_t qubit1, uint32_t qubit2) : qubit1(qubit1), qubit2(qubit2)
	   {
		 m =  build_matrix(swap_c,4);
	   }

	   int32_t apply(qu_register& qreg)
	   {
	      cnot(qubit1,qubit2).apply(qreg);
	      cnot(qubit2,qubit1).apply(qreg);
	      cnot(qubit1,qubit2).apply(qreg);
	      return 0;
	   }


	   void    dump()
	   {
		 println("  [-] swap(q1=" << qubit1 << ", q2=" << qubit2 << ")");
	   }

	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit1);
		 r.push_back(qubit2);
		 return r;
	   }

   };
   
   
   /**
    * \brief cphase
    */
   class cphase : public gate
   {
	 private:
	   
	   uint32_t ctrl_qubit;
	   uint32_t target_qubit;
	   
	 public:

	   cphase(uint32_t ctrl_qubit, uint32_t target_qubit) : ctrl_qubit(ctrl_qubit), target_qubit(target_qubit)
	   {
	   }

	   int32_t apply(qu_register& qreg)
	   {
	      hadamard(target_qubit).apply(qreg);
	      cnot(ctrl_qubit,target_qubit).apply(qreg);
	      hadamard(target_qubit).apply(qreg);
	      return 0;
	   }

	   void dump()
	   {
		 println("  [-] cphase(ctrl_qubit=" << ctrl_qubit << ", target_qubit=" << target_qubit << ")");
	   }

	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(ctrl_qubit);
		 r.push_back(target_qubit);
		 return r;
	   }

   };




   /**
    * \brief  custom matrix gate
    *     
    */
   class custom : public gate
   {
	 private:

#ifdef __BUILTIN_LINALG__
           std::vector<uint32_t> qubits;
#else
	   ublas::vector<uint32_t>  qubits;
#endif 
	   cmatrix_t m;

	 public:

#ifdef __BUILTIN_LINALG__
	   custom(std::vector<uint32_t>  qubits, cmatrix_t m) : qubits(qubits), m(m)
#else
	   custom(ublas::vector<uint32_t>  qubits, cmatrix_t m) : qubits(qubits), m(m)
#endif 
	   {
		 uint32_t size = 1 << qubits.size();
		 if (size != m.size1() || size != m.size2())
		    println("[x] error: cutom gate : the matrix size do not match the number of qubits !");
		 // verify also that the matrix is unitary
#ifdef __BUILTIN_LINALG__
		 cmatrix_t ctr(m.size2(),m.size1());
		 for (uint32_t i=0; i<m.size2(); ++i)
		    for (uint32_t j=0; j<m.size1(); ++j)
		       ctr(i,j) = std::conj(m(j,i));
		 cmatrix_t mxctr = mxm(m,ctr);
		 qx::linalg::identity_matrix<complex_t> id(m.size1());
#else
		 cmatrix_t mxctr = mxm(m,ublas::trans(conj(m)));
		 ublas::identity_matrix<complex_t> id(m.size1());
#endif

#ifdef __BUILTIN_LINALG__
		 if (qx::linalg::equals(mxctr,id))
#else
		 if (equals(mxctr,id))
#endif
		    println("[x] error: custom gate : the specified matrix is not unitary !");
	   }

	   int32_t apply(qu_register& qreg)
	   {
	      println("[x] custom matrix not supported !");
	      return 0;
	   }

	   void    dump()
	   {
		 println("  [-] custom matrix. ");
		 // println("  [-] custom(qubits=" << qubits << ", matrix=" << m << ")");
	   }

   };

   
   /**
    * measure
    */
   class measure : public gate
   {
	 private:

	   uint32_t  qubit;
	   bool      measure_all;

	 public:

	   measure(uint32_t qubit) : qubit(qubit), measure_all(false)
	   {
	   }
	   
	   measure() : qubit(0), measure_all(true)
	   {
	   }

	   int32_t apply(qu_register& qreg)
	   {
	      if (measure_all)
	      {
		 qreg.measure();
		 return 0;
	      }

	      //std::srand(std::time(0));          // get it out of there
	      //double f = ((double)std::rand())/((double)RAND_MAX);
	      double f = qreg.rand();
	      double p=0;
	      int32_t k, l, m;
	      int32_t j = qubit;
	      int32_t value;
	      double  fvalue;
	      cvector_t& data = qreg.get_data();
	      uint32_t size = qreg.size(); // data.size();
	      
	      /*
	      for (k = 0; k < (1 << size); k += (1 << j)) 
	      {
		 for (l = 0; l < (1 << (j-1)); l++) 
		 {
		    m = k + l;
		    p += std::norm(data[m]);
		    println(m);
		 }
	      }
	      println("f="<<f);
	      println("p="<<p);

	      if ( f < p ) {
		 value = 0;         
	      } else {
		 value = 1;
	      }

	      for (k = 0; k < (1 << size); k += (1 << j)) 
	      {
		 for (l = 0; l < (1 << (j-1)); l++) 
		 {
		    m = k + l + (1 ^ value) * (1 << (j-1));
		    data[m] = 0;
		    m = k + l + (1 & value) * (1 << (j-1));
		    if (std::fabs(std::norm(data[m])) > QUBIT_ERROR_THRESHOLD)
		       data[m] = 1;
		 }
	      }
	      */
	      uint32_t n = (1 << size);
	      std::bitset<MAX_QB_N> b;
	      b.reset();
	      b.set(qubit);
	      uint32_t bc = b.to_ulong();

	      while (bc < n)
	      {
		 bc =  b.to_ulong();
		 p += std::norm(data[bc]);
		 b = inc(b);
		 b.set(qubit);  
		 bc =  b.to_ulong();
	      }

	      //println("prob=" << p << ", rand=" << f);
	      if (f<p) value = 1;
	      else value = 0;

	      //fvalue = (double)value;
	      
	      #define __bit_test(x,pos) ((x) & (1<<(pos)))

	      if (value)   // 1
	      {  // reset all states where the qubit is 0
		 for (uint32_t i=0; i<(1 << size); ++i)
		 {
		    if (!__bit_test(i,qubit))
		       data[i] = 0;
		 }
	      }
	      else
	      {
		 for (uint32_t i=0; i<(1 << size); ++i)
		 {
		    if (__bit_test(i,qubit))
		       data[i] = 0;
		 }
	      }
	    
	      // renormalize
	      double length = 0;
	      for (k = 0; k < (1 << size); k++) 
		 length += std::norm(data[k]);
	      length = std::sqrt(length);

	      for (k = 0; k < (1 << size); k++) 
		 data[k] /= length;

	      // println("  [>] measured value : " << value);

	      qreg.set_binary(qubit,(value == 1 ? __state_1__ : __state_0__));

	      return value;
	   }

	   void dump()
	   {
		 println("  [-] measure(qubit=" << qubit << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 r.push_back(qubit);
		 return r;
	   }

   };


   /**
    * \brief   generic binary controlled gate
    */
   class bin_ctrl : public gate
   {
	 private:

	   uint32_t bit;
	   gate *   g;

	 public:

	   bin_ctrl(uint32_t bit, gate * g) : bit(bit), g(g)
	   {
	   }

	   int32_t apply(qu_register& qreg)
	   {
	      if (qreg.test(bit))
		 g->apply(qreg);
	      return 0;
	   }

	   void dump()
	   {
		 print("  [-] bin_ctrl: \n bit=" << bit << " -> ");
		 g->dump();
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 return g->qubits();
	   }

   };

   #define bin_ctrl_pauli_x(b,q) bin_ctrl(b,new pauli_x(q))
   #define bin_ctrl_pauli_y(b,q) bin_ctrl(b,new pauli_y(q))
   #define bin_ctrl_pauli_z(b,q) bin_ctrl(b,new pauli_z(q))


   class lookup_gate_table : public gate 
   {
      private:

        std::vector<uint32_t>      ctrl_bits;
	std::map<uint32_t,gate *>  gates;   

      public:

        lookup_gate_table(uint32_t b0)
	{
	   ctrl_bits.push_back(b0);
	}


        lookup_gate_table(uint32_t b0, uint32_t b1)
	{
	   ctrl_bits.push_back(b0);
	   ctrl_bits.push_back(b1);
	}

        
	lookup_gate_table(uint32_t b0, uint32_t b1, uint32_t b2)
	{
	   ctrl_bits.push_back(b0);
	   ctrl_bits.push_back(b1);
	   ctrl_bits.push_back(b2);
	}


	lookup_gate_table(std::vector<uint32_t> ctrl_bits) : ctrl_bits(ctrl_bits)
	{
	}

	
	void add_gate(uint32_t cond, gate * g)
	{
	   assert(cond < (1<< ctrl_bits.size()));
	   gates[cond] = g;
	}

	
	int32_t apply(qu_register& qreg)
	{
	   uint32_t k = 0;

	   for (uint32_t i=0; i<ctrl_bits.size(); i++)
	   {
	      //println(qreg.get_binary(i));
	      if (qreg.test(ctrl_bits[i]))
		 k = k * 2 + 1;
	      else
		 k *= 2;
	   }

	   // println("[+] lookup table : cond = " << k);

	   std::map<uint32_t,gate*>::iterator it = gates.find(k);

	   if (it != gates.end())
	      (*it).second->apply(qreg);

	    return 0;
	}

	std::vector<uint32_t>  qubits()
	{
	   std::vector<uint32_t> r;
	   // to do
	   return r;
	}


	void dump()
	{
	   println("  [-] lookup gate table : ");
	}

        

   
   };

   /**
    * \brief display : debug utility
    *    display intermediate quantum states of a 
    *   quantum register whithin a circuit.
    */
   class display : public gate
   {
	 private:
	 
	   bool only_binary;

	 public:

	   display(bool only_binary=false) : only_binary(only_binary)
	   {
	   }

	   int32_t apply(qu_register& qreg)
	   {
		 qreg.dump(only_binary);
		 return 0;
	   }

	   void dump()
	   {
		 println("  [-] display(only_binary=" << only_binary << ")");
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 return r;
	   }
   };



   /**
    * parallel gates
    */
   class parallel_gates : public gate
   {
	 public:
	   
	   parallel_gates()
	   {
	   }

	   int32_t apply(qu_register& qreg)
	   {
	      for (uint32_t i=0; i<gates.size(); i++)
		 gates[i]->apply(qreg);
	      return 0;
	   }

	   uint32_t add(gate * g)
	   {
		 gates.push_back(g);
		 return gates.size();
	   }
	   
	   std::vector<uint32_t>  qubits()
	   {
		 std::vector<uint32_t> r;
		 for (uint32_t i=0; i<gates.size(); i++)
		 {
		    std::vector<uint32_t> q = gates[i]->qubits();
		    r.insert(r.end(),q.begin(),q.end());	
		 }
		 return r;
	   }

	   void dump()
	   {
		 println("  [-] parallel_gates (" << gates.size() << " gates) : ");
		 for (uint32_t i=0; i<gates.size(); i++)
		    gates[i]->dump();
	   }
	

	 private:

	   std::vector<gate *> gates; // list of the parallel gates

   };

}

#endif // QX_GATE_H

