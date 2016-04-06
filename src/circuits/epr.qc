# file   : epr.qc
# author : Nader Khammassi
# brief  : EPR pair circuit

# define 2 qubits
qubits 2


h q0 
display    # display the quantum state
cnot q0,q1
display
measure q0
display
measure q1
display

# to activate error injection, uncomment the following line
# error_model depolarizing_channel,0.1
