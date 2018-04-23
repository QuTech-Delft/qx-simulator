qubits 3

.init
	prepz q1
	prepz q2
	x q1

.kernel1
	s q1

# .kernel2
# 	cz q2, q1

.measurement
	measure q1
	measure q2
