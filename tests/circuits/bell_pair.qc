version 1.0

# file   : bell_pair.qc
# author : Nader Khammassi
# brief  : EPR pair circuit

# define 2 qubits
qubits 2


.init
  prep_z q[0]
  prep_z q[1]

.entangle
  h q[0] 
  cnot q[0],q[1]

.measurement
  measure q[0]
  measure q[1]

# to activate error injection, uncomment the following line (error rate = 1%)
# error_model depolarizing_channel,0.01
