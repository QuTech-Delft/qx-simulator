# quantum full adder
# quantum carry-save addition

# define 4 qubits
qubits 4

# name qubits
map q0,A 
map q1,B
map q2,C
map q3,D


# init inputs to some values
.init
  x A
  x C

  display_binary  # display initial input value


# perform addition
.add

  toffoli B,C,D
  cnot    B,C
  toffoli A,C,D
  cnot    A,C

  display_binary # display result

.reverse_add

  cnot    A,C
  toffoli A,C,D
  cnot    B,C
  toffoli B,C,D

  display_binary # display result


error_model depolarizing_channel, 0.1

  


