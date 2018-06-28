qubits 5

.init
   x q2
   h q0
   h q1
   h q2

# iterating 1 times
.grover(1)

   # oracle_1

   x q1
   toffoli q0,q1,q3
   toffoli q1,q3,q4
   cnot q4,q2
   toffoli q1,q3,q4
   toffoli q0,q1,q3
   x q1
   h q0
   h q1

   # inversion

   x q0
   x q1
   h q1
   toffoli q0,q1,q3
   cnot q3,q1
   toffoli q0,q1,q3
   h q1
   x q0
   x q1
   h q0
   h q1

.measure
   h q2
   measure q2
   display
