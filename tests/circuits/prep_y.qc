version 1.0

# file   : prep_y.qc
# author : Nader Khammassi
# brief  : 'prep_y' test

# define 1 qubit
qubits 1


.prepare
  prep_y q[0]
  display

.measurement
  measure q[0]
  display

