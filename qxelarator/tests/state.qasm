version 1.0

qubits 3

.init
	prep_z q[1]
	prep_z q[2]
	x q[1]

.kernel1
	s q[1]
