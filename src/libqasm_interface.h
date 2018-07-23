#ifndef LIBQASM_INTERFACE_H
#define LIBQASM_INTERFACE_H

#include <core/circuit.h>
#include <qasm_ast.hpp>

#define ITER_FOR_IN(e, l) for (auto e = l.begin(); e != l.end(); e++)

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
   const std::vector<size_t> & qv = operation.getQubitsInvolved().getSelectedQubits().getIndices();
   qx::parallel_gates * pg = NULL;
   if (qv.size() > 1) 
      pg = new qx::parallel_gates();
   std::string type = operation.getType();
   if (type == "h")
   {
      if (!pg)
         return new qx::hadamard(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::hadamard(q));
         return pg;
      }
   }
   if (type == "cnot")
      return new qx::cnot(
            qid(operation, 1),
            qid(operation, 2));
   if (type == "toffoli")
      return new qx::toffoli(
            qid(operation, 1),
            qid(operation, 2),
            qid(operation, 3));
   if (type == "x")
   {
      if (!pg)
         return new qx::pauli_x(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::pauli_x(q));
         return pg;
      }
   }
   if (type == "y")
   {
      if (!pg)
         return new qx::pauli_y(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::pauli_y(q));
         return pg;
      }
   }
   if (type == "z")
   {
      if (!pg)
         return new qx::pauli_z(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::pauli_z(q));
         return pg;
      }
   }
   if (type == "s")
   {
      if (!pg)
         return new qx::phase_shift(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::phase_shift(q));
         return pg;
      }
   }
   if (type == "sdag")
   {
      if (!pg)
         return new qx::s_dag_gate(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::s_dag_gate(q));
         return pg;
      }
   }
   if (type == "t")
   {
      if (!pg)
         return new qx::t_gate(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::t_gate(q));
         return pg;
      }
   }
   if (type == "tdag")
   {
      if (!pg)
         return new qx::t_dag_gate(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::t_dag_gate(q));
         return pg;
      }
   }
   if (type == "rx")
   {
      double angle = operation.getRotationAngle();
      if (!pg)
         return new qx::rx(sqid(operation), angle);
      else
      {
         for (auto q : qv)
            pg->add(new qx::rx(q,angle));
         return pg;
      }
   }
   if (type == "ry")
   {
      double angle = operation.getRotationAngle();
      if (!pg)
         return new qx::ry(sqid(operation), angle);
      else
      {
         for (auto q : qv)
            pg->add(new qx::ry(q,angle));
         return pg;
      }
   }
   if (type == "rz")
   {
      double angle = operation.getRotationAngle();
      if (!pg)
         return new qx::rz(sqid(operation), angle);
      else
      {
         for (auto q : qv)
            pg->add(new qx::rz(q,angle));
         return pg;
      }
   }
   if (type == "swap")
      return new qx::swap(
            qid(operation, 1),
            qid(operation, 2));
   if (type == "cz")
      return new qx::cphase(
            qid(operation, 1),
            qid(operation, 2));
   if (type == "prep_z")
   {
      if (!pg)
         return new qx::prepz(sqid(operation));
      else
      {
         for (auto q : qv)
            pg->add(new qx::prepz(q));
         return pg;
      }
   }
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
   if (type == "display")
      return new qx::display();
   if (type == "x90")
   {
      double angle = M_PI/2;
      if (!pg)
         return new qx::rx(sqid(operation), angle);
      else
      {
         for (auto q : qv)
            pg->add(new qx::rx(q,angle));
         return pg;
      }
   }
   if (type == "mx90")
   {
      double angle = -M_PI/2;
      if (!pg)
         return new qx::rx(sqid(operation), angle);
      else
      {
         for (auto q : qv)
            pg->add(new qx::rx(q,angle));
         return pg;
      }
   }
   if (type == "y90")
   {
      double angle = M_PI/2;
      if (!pg)
         return new qx::ry(sqid(operation), angle);
      else
      {
         for (auto q : qv)
            pg->add(new qx::ry(q,angle));
         return pg;
      }
   }
   if (type == "my90")
   {
      double angle = -M_PI/2;
      if (!pg)
         return new qx::ry(sqid(operation), angle);
      else
      {
         for (auto q : qv)
            pg->add(new qx::ry(q,angle));
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
   if (type == "prep_y")
      return NULL;
   if (type == "prep_x")
      return NULL;
   if (type == "measure_x")
      return NULL;
   if (type == "measure_y")
      return NULL;
   if (type == "cr")
      return NULL;
   if (type == "crk")
      return NULL;
   return NULL;
}

qx::circuit *qxCircuitFromCQasm(uint32_t qubits_count, compiler::SubCircuit &subcircuit)
{
  uint32_t iterations = subcircuit.numberIterations();
  std::string name = subcircuit.nameSubCircuit();

  qx::circuit *circuit = new qx::circuit(qubits_count, name, iterations);

  
  const std::vector<compiler::OperationsCluster*>& clusters
    = subcircuit.getOperationsCluster();

  ITER_FOR_IN(p_cluster, clusters)
  {
    const std::vector<compiler::Operation*> operations
      = (*p_cluster)->getOperations();
    ITER_FOR_IN(p_operation, operations)
    {
      qx::gate *g = gateLookup(**p_operation);
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

  return circuit;
}

#endif
