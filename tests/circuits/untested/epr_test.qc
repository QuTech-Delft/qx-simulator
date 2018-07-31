version 1.0

qubits 2


.bell
 h q[0]
 cnot q[0],q[1]
 display

.measurement
 measure q[0]
 measure q[1]
 display
