# circuit example

qubits 3


.init(5)
  x q0
  h q1
  z q2
  s q0
  h q0
  display

.exec(3)
  h q2
  s q1