qubits 6

.init
	prepz q0
	prepz q1
	prepz q2
	x q0
	x q1
	x q2

	prepz q3
	prepz q4

	prepz q5
	#x q5

.cc_x
	toffoli q0, q1, q3
	toffoli q2, q3, q4

	cnot q4, q5

	toffoli q2, q3, q4
	toffoli q0, q1, q3

	measure q0
	measure q1
	measure q2
	measure q3
 	measure q4
 	measure q5
