version 1.0
qubits 4

map q[0], q0

h q0
h q[1]

rz q[0], 3.14
rz q0, 0.0
rz q[1], -0.0
rz q[1], -1.14
rz q[2], 10.3E-3
rz q[3], -10.3E-3

display
