version 1.0

# 
# date   : 07/04/2016
# author : nader khammassi
#
# example: loading an arbitrary quantum state from a file
#

qubits 3

# display the default initial state : |000>
.default_state
  display

# load a new quantum state
.load_a_new_arbitrary_state
  load_state "initial_quantum_state.qs"
  display # display the loaded state

# apply some gates to the new state
.algorithm
  x q[0]
  cnot q[0],q[1]
  display # display the fina result
