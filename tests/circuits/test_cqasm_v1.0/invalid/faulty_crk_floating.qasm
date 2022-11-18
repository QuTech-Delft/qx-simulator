version 1.0
qubits 4

map q[0], q0
map q[1], q1

crk q0, q1, 10
crk q0, q1, -10


# --- Faulty code ---
# CRK gate applied with floating argument
crk q0, q1, 0.1
