version 1.0

# file   : toffoli.qc
# author : Nader Khammassi
# brief  : simple toffoli gate test

qubits 3

.input
  x q[0]
  x q[1]
 display   # print input state

.result
  toffoli q[0],q[1],q[2]
  measure q[0:2]
  display  # print the result
