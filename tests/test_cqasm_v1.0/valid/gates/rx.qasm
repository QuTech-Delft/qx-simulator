version 1.0
qubits 4

map q[0], q0

h q0
h q[1]

rx q[0], 3.14
rx q0, 0.0
rx q[1], -0.0
rx q[1], -1.14
rx q[2], 10.3E-3
rx q[3], -10.3E-3

display
