version 1.0

qubits 6

.init
	prep_z q[0]
	prep_z q[1]
	prep_z q[2]
	x q[0]
	x q[1]
	x q[2]

	prep_z q[3]
	prep_z q[4]

	prep_z q[5]

.cc_x
	toffoli q[0], q[1], q[3]
	toffoli q[2], q[3], q[4]

	cnot q[4], q[5]

	toffoli q[2], q[3], q[4]
	toffoli q[0], q[1], q[3]

	measure q[0]
	measure q[1]
	measure q[2]
	measure q[3]
 	measure q[4]
 	measure q[5]
