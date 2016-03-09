# EPR pair

qubits 2

h q0 
display
cnot q0,q1
display
measure q0
display
measure q1
display

# error_model depolarizing_channel,0.1
