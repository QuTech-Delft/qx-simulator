version 1.0

qubits 2

.init
	prep_z q[0]
	prep_z q[1]

    x q[0]
    x q[1]

	cz q[0], q[1]
	measure q[0]
	measure q[1]
