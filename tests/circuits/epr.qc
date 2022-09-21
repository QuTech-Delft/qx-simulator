version 1.0

# EPR pair

qubits 2

h q[0]
display
cnot q[0],q[1]
display
measure q[0]
display
measure q[1]
display

# error_model depolarizing_channel,0.1
