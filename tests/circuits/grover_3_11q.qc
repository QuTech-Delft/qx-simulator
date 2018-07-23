version 1.0

# 
# grover algorithm
# search number : 3 
# data          : q[2],q[1],q[0]

qubits 7

.init
   x q[3]
   h q[0]
   h q[1]
   h q[2]
   h q[3]

# iterating 2 times
.grover(2)

   # oracle_3

   x q[2]
   toffoli q[0],q[1],q[4]
   toffoli q[1],q[4],q[5]
   toffoli q[2],q[5],q[6]
   cnot q[6],q[3]
   toffoli q[2],q[5],q[6]
   toffoli q[1],q[4],q[5]
   toffoli q[0],q[1],q[4]
   x q[2]
   h q[0]
   h q[1]
   h q[2]

   # inversion

   x q[0]
   x q[1]
   x q[2]
   h q[2]
   toffoli q[0],q[1],q[2]
   h q[2]
   x q[0]
   x q[1]
   x q[2]
   h q[0]
   h q[1]
   h q[2]
   display

.measurement
   h q[3]
   measure q[3]
   display
   measure_all
   display

# error_model depolarizing_channel, 0.01
