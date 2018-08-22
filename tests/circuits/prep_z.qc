version 1.0

# file   : prep_z.qc
# author : Nader Khammassi
# brief  : 'prep_z' test

# define 1 qubit
qubits 1


.prepare
  prep_z q[0]
  display

.measurement
  measure q[0]
  display

