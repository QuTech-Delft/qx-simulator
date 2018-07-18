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
  std::string type = operation.getType();
  if (type == "h")
    return new qx::hadamard(sqid(operation));
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
    return new qx::pauli_x(sqid(operation));
  if (type == "y")
    return new qx::pauli_y(sqid(operation));
  if (type == "z")
    return new qx::pauli_z(sqid(operation));
  if (type == "s")
    return new qx::phase_shift(sqid(operation));
  if (type == "sdag")
    return new qx::s_dag_gate(sqid(operation));
  if (type == "t")
    return new qx::t_gate(sqid(operation));
  if (type == "tdag")
    return new qx::t_dag_gate(sqid(operation));
  if (type == "rx")
    return new qx::rx(
      sqid(operation),
      operation.getRotationAngle());
  if (type == "ry")
    return new qx::ry(
      sqid(operation),
      operation.getRotationAngle());
  if (type == "rz")
    return new qx::rz(
      sqid(operation),
      operation.getRotationAngle());
  if (type == "swap")
    return new qx::swap(
      qid(operation, 1),
      qid(operation, 2));
  if (type == "cz")
    return new qx::cphase(
      qid(operation, 1),
      qid(operation, 2));
  if (type == "prep_z")
    return new qx::prepz(sqid(operation));
  if (type == "measure" || type == "measure_z")
    return new qx::measure(sqid(operation));
  if (type == "measure_all")
    return new qx::measure();
  if (type == "display")
    return new qx::display();
  if (type == "x90")
    return new qx::rx(sqid(operation), M_PI/2);
  if (type == "mx90")
    return new qx::rx(sqid(operation), -M_PI/2);
  if (type == "y90")
    return new qx::ry(sqid(operation), M_PI/2);
  if (type == "my90")
    return new qx::ry(sqid(operation), -M_PI/2);
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
