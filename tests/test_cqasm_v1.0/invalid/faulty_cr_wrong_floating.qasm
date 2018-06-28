version 1.0
qubits 4

map q[0], q0
map q[1], q1

cr q0, q1, 10
cr q0, q1, -10


# --- Faulty code ---
# CR applied with invalid floating argument
cr q0, q1, .1
