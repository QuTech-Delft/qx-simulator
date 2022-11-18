version 1.0
qubits 4

map q[0], q0
map q[1], q1
map b[1], b1
map q[3], q3

x q0
x q1
x q[2]
x q[3]

measure q0
measure q1

c-z b[0], q3

display
