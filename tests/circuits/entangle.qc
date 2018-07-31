version 1.0

# file   : entangle.qc
# author : Nader Khammassi
# brief  : basic 8 qubits entanglement circuit 

qubits 8

.init

  prep_z q[0:7]

.entangle

  h q[0]
  cnot q[0],q[1]
  cnot q[0],q[2]
  cnot q[0],q[3]
  cnot q[0],q[4]
  cnot q[0],q[5]
  cnot q[0],q[6]
  cnot q[0],q[7]
  display

.measurement

  measure q[0:7]
  display
