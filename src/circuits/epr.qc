# file   : epr.qc
# author : Nader Khammassi
# brief  : EPR pair circuit

# define 2 qubits
qubits 2

.init
  prepz q0
  prepz q1

.entangle
  h q0 
  cnot q0,q1
  display    # display the quantum state

.measurement
  measure q0
  measure q1
  display

# to activate error injection, uncomment the following line
# error_model depolarizing_channel,0.1
