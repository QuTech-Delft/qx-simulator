version 1.0

# file   : integer_arguments.qc
# author : Koen Martens
# brief  : test that integer arguments are parsed where float is expected

# define 1 qubit
qubits 1

.prepare
  prep_z q[0]
  display

.argumenttest
  rx q[0], 3
  ry q[0], 23
  rz q[0], 42

.measurement
  measure q[0]
  display

