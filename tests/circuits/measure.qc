version 1.0

# file   : measure.qc
# author : Nader Khammassi
# brief  : test measurement in different cases


qubits 2


.ground
  prep_z q[0,1]
  measure q[0]
  display
 

.exited_state
  prep_z q[0,1]
  x q[0]
  measure q[0]
  display_binary


.superposition_loop(1000)
  prep_z q[0,1]
  h q[1]
  measure q[1]


.entangle_loop(100)
  prep_z q[0,1]
  h q[0]
  cnot q[0],q[1]
  measure q[0]
  measure q[1]

.entangle_result
  display_binary

