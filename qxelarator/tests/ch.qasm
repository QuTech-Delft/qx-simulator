
qubits 3

.init
	x q0
	prepz q1
	#x q2

.controlled_kernel1
   cnot q1,q0
   cnot q2,q0
   h q1
   t q2
   tdag q0
   t q1
   cnot q1,q0
   cnot q2,q1
   t q0
   cnot q2,q0
   tdag q1
   tdag q0
   cnot q2,q1
   cnot q1,q0
   t q0
   h q1
   cnot q1,q0

.measurement
	measure q0
	measure q1
