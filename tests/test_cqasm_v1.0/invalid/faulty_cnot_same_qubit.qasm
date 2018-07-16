version 1.0
qubits 4

map q[0], q0
map q[1], q1

cnot q0, q1
cnot q1, q[2]
cnot q[2], q1
cnot q[2], q[3]

# --- Faulty code ---
# CNOT gate applied on the same qubit
cnot q0, q[0]
