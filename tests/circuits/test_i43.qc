version 1.0

# file   : test_i43.qc
# author : Nader Khammassi
# brief  : test for issue 43 (measure/prep_z for more than 18 qubits)

# define 20 qubits
qubits 24


.init
  prep_z q[0:23]
  display


.measurement
  measure q[0:23]
  display

