version 1.0
qubits 3

.init
	x q[0]
	prep_z q[1]
	#x q[2]

.controlled_kernel1
   cnot q[1],q[0]
   cnot q[2],q[0]
   h q[1]
   t q[2]
   tdag q[0]
   t q[1]
   cnot q[1],q[0]
   cnot q[2],q[1]
   t q[0]
   cnot q[2],q[0]
   tdag q[1]
   tdag q[0]
   cnot q[2],q[1]
   cnot q[1],q[0]
   t q[0]
   h q[1]
   cnot q[1],q[0]

.measurement
	measure q[0]
	measure q[1]
