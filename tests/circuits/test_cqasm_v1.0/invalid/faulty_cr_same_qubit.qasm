version 1.0
qubits 4

map q[0], q0
map q[1], q1

cr q0, q1, 10
cr q0, q1, -10


# --- Faulty code ---
# Two qubit gate applied on the same qubit
cr q1, q1, 10
