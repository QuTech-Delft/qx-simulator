version 1.0

# file   : full_adder.qc
# author : Nader Khammassi
# brief  : quantum full adder (carry-save addition)

# define 4 qubits
qubits 4

# rename qubits
map q[0],b0 
map q[1],b1
map q[2],b2
map q[3],b3


# init inputs to some values
.init
  x b0
  x b2

  display_binary  #  initial input value


# perform addition
.add

  toffoli b1,b2,b3
  cnot    b1,b2
  toffoli b0,b2,b3
  cnot    b0,b2

  display_binary #  result

.reverse_add

  cnot    b0,b2
  toffoli b0,b2,b3
  cnot    b1,b2
  toffoli b1,b2,b3

  display_binary #  result

# error_model depolarizing_channel, 0.1
