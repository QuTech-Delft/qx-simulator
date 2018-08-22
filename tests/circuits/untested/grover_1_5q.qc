version 1.0

qubits 5

.init
   x q[2]
   h q[0]
   h q[1]
   h q[2]

# iterating 1 times
.grover(1)

   # oracle_1

   x q[1]
   toffoli q[0],q[1],q[3]
   toffoli q[1],q[3],q[4]
   cnot q[4],q[2]
   toffoli q[1],q[3],q[4]
   toffoli q[0],q[1],q[3]
   x q[1]
   h q[0]
   h q[1]

   # inversion

   x q[0]
   x q[1]
   h q[1]
   toffoli q[0],q[1],q[3]
   cnot q[3],q[1]
   toffoli q[0],q[1],q[3]
   h q[1]
   x q[0]
   x q[1]
   h q[0]
   h q[1]

.measurement
   h q[2]
   measure q[2]
   display
