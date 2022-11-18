version 1.0
qubits 4

map q[0], q0
map q[1], q1
map b[1], b1
map q[3], q3

x q0
x q1

measure q0
measure q1

c-x b[0], q[2]
c-x b1, q3

display
