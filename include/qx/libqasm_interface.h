#ifndef LIBQASM_INTERFACE_H
#define LIBQASM_INTERFACE_H

#include "qx/compat.h"
#include "qx/core/circuit.h"
#include <qasm_ast.hpp>

#define __for_in(e, l) for (auto e = l.begin(); e != l.end(); e++)

#define __ret_gate_1(__g) \
{\
   if (!pg)\
      if (!bc)\
         return new __g(sqid(operation));\
      else\
         return new qx::bin_ctrl(bv, new __g(sqid(operation)));\
   else\
   {\
      if (!bc)\
      {\
         for (auto q : qv)\
            pg->add(new __g(q));\
      }\
      else\
      {\
         for (auto q : qv)\
            pg->add(new qx::bin_ctrl(bv, new __g(q)));\
      }\
      return pg;\
   }\
}\


#define __ret_bin_gate(__g) \
{\
   if (!pg && (bv.size() > 1))\
      pg = new qx::parallel_gates();\
   if (!pg)\
      return new __g(bid(operation));\
   else\
   {\
      for (auto b : bv)\
         pg->add(new __g(b));\
      return pg;\
   }\
}\



#define __ret_gate_2(__g) \
{\
   const std::vector<size_t> & qv0 = operation.getQubitsInvolved(1).getSelectedQubits().getIndices();\
   const std::vector<size_t> & qv1 = operation.getQubitsInvolved(2).getSelectedQubits().getIndices();\
   \
   if (qv0.size() != qv1.size())\
      throw ("[x] error : parallel "+type+" args have different sizes !"); \
   \
   if (qv0.size() == 1)\
   {\
      if (!bc)\
         return new __g(qv0[0], qv1[0]);\
      else\
         return new qx::bin_ctrl(bv, new __g(qv0[0], qv1[0]));\
   }\
   else\
   {\
      pg = new qx::parallel_gates();\
      if (!bc)\
      {\
         for (size_t i=0; i<qv0.size(); ++i)\
            pg->add(new __g(qv0[i],qv1[i]));\
      }\
      else\
      {\
         for (size_t i=0; i<qv0.size(); ++i)\
            pg->add(new qx::bin_ctrl(bv, new __g(qv0[i],qv1[i])));\
      }\
      return pg;\
   }\
}\


int sqid(compiler::Operation &operation)
{
  return operation
    .getQubitsInvolved()
    .getSelectedQubits()
    .getIndices()[0];
}

int qid(compiler::Operation &operation, int id)
{
  return operation
    .getQubitsInvolved(id)
    .getSelectedQubits()
    .getIndices()[0];
}

int bid(compiler::Operation &operation)
{
  return operation
    .getControlBits()
    .getSelectedBits()
    .getIndices()[0];
}

qx::gate *gateLookup(compiler::Operation &operation)
{
   // operation.printOperation();
   bool  bc = false;
   const std::vector<size_t> & qv = operation.getQubitsInvolved().getSelectedQubits().getIndices();
   const std::vector<size_t> & bv = operation.getControlBits().getSelectedBits().getIndices();
   qx::parallel_gates * pg = NULL;

   if (bv.size())
      bc = true;
   if (qv.size() > 1)
      pg = new qx::parallel_gates();

   //if (bc) { std::cout << operation.getControlBits().printMembers(); }

   std::string type = operation.getType();
   
   if (type == "toffoli")
   {
      const std::vector<size_t> & qv0 = operation.getQubitsInvolved(1).getSelectedQubits().getIndices();
      const std::vector<size_t> & qv1 = operation.getQubitsInvolved(2).getSelectedQubits().getIndices();
      const std::vector<size_t> & qv2 = operation.getQubitsInvolved(3).getSelectedQubits().getIndices();

      if ((qv0.size() != qv1.size()) || (qv0.size() != qv2.size()))
         throw ("[x] error : parallel toffoli args have different sizes !"); 

      if (qv0.size() == 1)
      {
         if (!bc)
            return new qx::toffoli(qv0[0], qv1[0], qv2[0]);
         else
            return new qx::bin_ctrl(bv, new qx::toffoli(qv0[0], qv1[0], qv2[0]));
      }
      else
      {
         pg = new qx::parallel_gates();
         if (!bc)
            for (size_t i=0; i<qv0.size(); ++i)
               pg->add(new qx::toffoli(qv0[i],qv1[i],qv2[i]));
         else
            for (size_t i=0; i<qv0.size(); ++i)
               pg->add(new qx::bin_ctrl(bv, new qx::toffoli(qv0[i],qv1[i],qv2[i])));
         return pg;
      }      
   }

   ///////// common sq gates //////
   if (type == "i")
      __ret_gate_1(qx::identity)
   if (type == "x")
      __ret_gate_1(qx::pauli_x)
   if (type == "y")
      __ret_gate_1(qx::pauli_y)
   if (type == "z")
      __ret_gate_1(qx::pauli_z)
   if (type == "h")
      __ret_gate_1(qx::hadamard)
   if (type == "s")
      __ret_gate_1(qx::phase_shift)
   if (type == "sdag")
      __ret_gate_1(qx::s_dag_gate)
   if (type == "t")
      __ret_gate_1(qx::t_gate)
   if (type == "tdag")
      __ret_gate_1(qx::t_dag_gate)

   /////////// classical /////////

   if (type == "not")
      __ret_bin_gate(qx::classical_not)

   /////////// rotations /////////
   if (type == "rx")
   {
      double angle = operation.getRotationAngle();
      if (!pg)
         if (!bc)
            return new qx::rx(sqid(operation), angle);
          else
            return new qx::bin_ctrl(bv, new qx::rx(sqid(operation), angle));
      else
      {
         if (!bc)
            for (auto q : qv)
               pg->add(new qx::rx(q,angle));
         else
            for (auto q : qv)
               pg->add(new qx::bin_ctrl(bv, new qx::rx(q,angle)));
         return pg;
      }
   }
   if (type == "ry")
   {
      double angle = operation.getRotationAngle();
      if (!pg)
         if (!bc)
            return new qx::ry(sqid(operation), angle);
         else
            return new qx::bin_ctrl(bv, new qx::ry(sqid(operation), angle));
      else
      {
         if (!bc)
            for (auto q : qv)
               pg->add(new qx::ry(q,angle));
         else
            for (auto q : qv)
               pg->add(new qx::bin_ctrl(bv, new qx::ry(q,angle)));
         return pg;
      }
   }
   if (type == "rz")
   {
      double angle = operation.getRotationAngle();
      if (!pg)
         if (!bc)
            return new qx::rz(sqid(operation), angle);
         else
            return new qx::bin_ctrl(bv, new qx::rz(sqid(operation), angle));
      else
      {
         if (!bc)
            for (auto q : qv)
               pg->add(new qx::rz(q,angle));
         else
            for (auto q : qv)
               pg->add(new qx::bin_ctrl(bv, new qx::rz(q,angle)));
         return pg;
      }
   }

   //////////// two qubits gates //////////////
   if (type == "cnot")
      __ret_gate_2(qx::cnot)
   if (type == "cz")
      __ret_gate_2(qx::cphase)
   if (type == "swap")
      __ret_gate_2(qx::swap)
   
   ///////////// prep gates //////////////////
   if (type == "prep_z")
      __ret_gate_1(qx::prepz)
   if (type == "prep_y")
      __ret_gate_1(qx::prepy)
   if (type == "prep_x")
      __ret_gate_1(qx::prepx)
   
   ////////// measurements //////////////////
   if (type == "measure" || type == "measure_z")
   {
      if (!pg)
         return new qx::measure(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::measure(q));
         return pg;
      }
   }
   if (type == "measure_all")
      return new qx::measure();

   if (type == "measure_x")
   {
      if (!pg)
         return new qx::measure_x(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::measure_x(q));
         return pg;
      }
   }
   if (type == "measure_y")
   {
      if (!pg)
         return new qx::measure_y(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::measure_y(q));
         return pg;
      }
   }

   ////////////// display /////////////////
   if (type == "display")
      return new qx::display();
   if (type == "display_binary")
      return new qx::display(true);

   /////////////// x90 //////////////////
   if (type == "x90")
   {
      double angle = QX_PI/2;
      if (!pg)
         if (!bc)
            return new qx::rx(sqid(operation), angle);
         else
            return new qx::bin_ctrl(bv, new qx::rx(sqid(operation), angle));
      else
      {
         if (!bc)
            for (auto q : qv)
               pg->add(new qx::rx(q,angle));
         else
            for (auto q : qv)
               pg->add(new qx::bin_ctrl(bv, new qx::rx(q,angle)));
         return pg;
      }
   }
   if (type == "mx90")
   {
      double angle = -QX_PI/2;
      if (!pg)
         if (!bc)
            return new qx::rx(sqid(operation), angle);
         else
            return new qx::bin_ctrl(bv, new qx::rx(sqid(operation), angle));
      else
      {
         if (!bc)
            for (auto q : qv)
               pg->add(new qx::rx(q,angle));
         else
            for (auto q : qv)
               pg->add(new qx::bin_ctrl(bv, new qx::rx(q,angle)));
         return pg;
      }
   }
   if (type == "y90")
   {
      double angle = QX_PI/2;
      if (!pg)
         if (!bc)
            return new qx::ry(sqid(operation), angle);
         else
            return new qx::bin_ctrl(bv, new qx::ry(sqid(operation), angle));
      else
      {
         if (!bc)
            for (auto q : qv)
               pg->add(new qx::ry(q,angle));
         else
            for (auto q : qv)
               pg->add(new qx::bin_ctrl(bv, new qx::ry(q,angle)));
         return pg;
      }
   }
   if (type == "my90")
   {
      double angle = -QX_PI/2;
      if (!pg)
         if (!bc)
            return new qx::ry(sqid(operation), angle);
         else
            return new qx::bin_ctrl(bv, new qx::ry(sqid(operation), angle));
      else
      {
         if (!bc)
            for (auto q : qv)
               pg->add(new qx::ry(q,angle));
         else
            for (auto q : qv)
               pg->add(new qx::bin_ctrl(bv, new qx::ry(q,angle)));
         return pg;
      }
   }
   if (type == "c-x")
      return new qx::bin_ctrl(
            bid(operation),
            new qx::pauli_x(sqid(operation)));
   if (type == "c-z")
      return new qx::bin_ctrl(
            bid(operation),
            new qx::pauli_z(sqid(operation))); 

   if (type == "cr")
   {
      double angle = operation.getRotationAngle();
      const std::vector<size_t> & qv0 = operation.getQubitsInvolved(1).getSelectedQubits().getIndices();
      const std::vector<size_t> & qv1 = operation.getQubitsInvolved(2).getSelectedQubits().getIndices();

      if (qv0.size() != qv1.size())
         throw ("[x] error : parallel 'cr' args have different sizes !"); 

      if (qv0.size() == 1)
      {
         if (!bc)
            return new qx::ctrl_phase_shift(qv0[0], qv1[0],angle);
         else
            return new qx::bin_ctrl(bv, new qx::ctrl_phase_shift(qv0[0], qv1[0],angle));
      }
      else
      {
         pg = new qx::parallel_gates();
         if (!bc)
            for (size_t i=0; i<qv0.size(); ++i)
               pg->add(new qx::ctrl_phase_shift(qv0[i],qv1[i],angle));
         else
            for (size_t i=0; i<qv0.size(); ++i)
               pg->add(new qx::bin_ctrl(bv, new qx::ctrl_phase_shift(qv0[i],qv1[i],angle)));
         return pg;
      }      
   }
   if (type == "crk")
   {
      size_t angle = operation.getRotationAngle();
      const std::vector<size_t> & qv0 = operation.getQubitsInvolved(1).getSelectedQubits().getIndices();
      const std::vector<size_t> & qv1 = operation.getQubitsInvolved(2).getSelectedQubits().getIndices();

      if (qv0.size() != qv1.size())
         throw ("[x] error : parallel 'crk' args have different sizes !"); 

      if (qv0.size() == 1)
      {
         if (!bc)
            return new qx::ctrl_phase_shift(qv0[0], qv1[0],angle);
         else
            return new qx::bin_ctrl(bv, new qx::ctrl_phase_shift(qv0[0], qv1[0],angle));
      }
      else
      {
         pg = new qx::parallel_gates();
         if (!bc)
            for (size_t i=0; i<qv0.size(); ++i)
               pg->add(new qx::ctrl_phase_shift(qv0[i],qv1[i],angle));
         else
            for (size_t i=0; i<qv0.size(); ++i)
               pg->add(new qx::bin_ctrl(bv, new qx::ctrl_phase_shift(qv0[i],qv1[i],angle)));
         return pg;
      }      
   }
   
   return NULL;
}

qx::circuit * load_cqasm_code(uint64_t qubits_count, compiler::SubCircuit &subcircuit)
{
  uint64_t iterations = subcircuit.numberIterations();
  std::string name = subcircuit.nameSubCircuit();

  qx::circuit *circuit = new qx::circuit(qubits_count, name, iterations);

  
  const std::vector<compiler::OperationsCluster*>& clusters
    = subcircuit.getOperationsCluster();

  __for_in(p_cluster, clusters)
  {
    const std::vector<compiler::Operation*> operations
      = (*p_cluster)->getOperations();
    __for_in(p_operation, operations)
    {
       qx::gate * g;
       try
       {
          g = gateLookup(**p_operation);
       }
       catch (const char * error)
       {
          std::cerr << error << std::endl;
          exit(-1);
       }
       if (!g)
       {
          throw (*p_operation)->getType();
       }
       else
       {
          circuit->add(g);
       }
    }
  }
  // circuit->dump();
  return circuit;
}

#endif
