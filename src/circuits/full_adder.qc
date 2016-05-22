# file   : full_adder.qc
# author : Nader Khammassi
# brief  : quantum full adder (carry-save addition)

# define 4 qubits
qubits 4

# rename qubits
map q0,A 
map q1,B
map q2,C
map q3,D


# init inputs to some values
.init
  x A
  x C

  display_binary  #  initial input value


# perform addition
.add

  toffoli B,C,D
  cnot    B,C
  toffoli A,C,D
  cnot    A,C

  display_binary #  result

.reverse_add

  cnot    A,C
  toffoli A,C,D
  cnot    B,C
  toffoli B,C,D

  display_binary #  result

# error_model depolarizing_channel, 0.1
