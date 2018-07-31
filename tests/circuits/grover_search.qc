version 1.0


# file   : grover_search.qc
# author : Nader Khammassi
# brief  : search number |3> in 3 qubits search space.

qubits 7

.init

   x q[3]
   h q[0:3]

# iterating 2 times
.grover(2)

   # oracle |x> ?= |3>
   x q[2]
   toffoli q[0],q[1],q[4]
   toffoli q[1],q[4],q[5]
   toffoli q[2],q[5],q[6]
   cnot q[6],q[3]
   toffoli q[2],q[5],q[6]
   toffoli q[1],q[4],q[5]
   toffoli q[0],q[1],q[4]

   x q[2]
   h q[0:2]

   # inversion
   x q[0:2]
   h q[2]
   toffoli q[0],q[1],q[2]
   h q[2]
   x q[0:2]
   h q[0:2]
   display

.final_state

   h q[3]
   x q[3]
   display

.mesurement

   measure q[0:2] # measure the solution
   display

# error_model depolarizing_channel, 0.01
