version 1.0

# file   : prep_x.qc
# author : Nader Khammassi
# brief  : 'prep_x' test

# define 1 qubit
qubits 1

.prepare
  prep_x q[0]
  display

.measurement
  measure q[0]
  display

