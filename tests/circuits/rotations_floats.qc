version 1.0
qubits 2

# test parsing of different variations of floats

Rz q[0], 0.123
Rz q[0], 0
Rz q[0], 0.
Rz q[0], 123.123
Rz q[1],12

.measurement
   measure q[0]
   measure q[1]
