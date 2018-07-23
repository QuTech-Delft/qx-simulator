version 1.0

qubits 16

.entangle(10)
  h q[0]
  cnot q[0],q[1]
  cnot q[0],q[2]
  cnot q[0],q[3]
  cnot q[0],q[4]
  cnot q[0],q[5]
  cnot q[0],q[6]
  cnot q[0],q[7]
  cnot q[0],q[8]
  cnot q[0],q[9]
  cnot q[0],q[10]
  cnot q[0],q[11]
  cnot q[0],q[12]
  cnot q[0],q[13]
  cnot q[0],q[14]
  cnot q[0],q[15]

# .measure
  measure q[0]
  measure q[1]
  measure q[2]
  measure q[3]
  measure q[4]
  measure q[5]
  measure q[6]
  measure q[7]
  measure q[8]
  measure q[9]
  measure q[10]
  measure q[11]
  measure q[12]
  measure q[13]
  measure q[14]
  measure q[15]
  display_binary
