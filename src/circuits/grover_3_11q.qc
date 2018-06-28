# 
# grover algorithm
# search number : 3 
# data          : q2,q1,q0

qubits 7

.init
   x q3
   h q0
   h q1
   h q2
   h q3

# iterating 2 times
.grover(2)

   # oracle_3

   x q2
   toffoli q0,q1,q4
   toffoli q1,q4,q5
   toffoli q2,q5,q6
   cnot q6,q3
   toffoli q2,q5,q6
   toffoli q1,q4,q5
   toffoli q0,q1,q4
   x q2
   h q0
   h q1
   h q2

   # inversion

   x q0
   x q1
   x q2
   h q2
   toffoli q0,q1,q2
   h q2
   x q0
   x q1
   x q2
   h q0
   h q1
   h q2
   display

.measure
   h q3
   measure q3
   display
   measure 
   display

# error_model depolarizing_channel, 0.01