version 1.0

# quantum error correction test
# 3 qubit bit flip correction code

# qubit definition
qubits 3 

map q[0],d
map q[1],a0
map q[2],a1

# init logical gate
.init
#   x q[0]
   x d
   display_binary

# encoding
.encoding
   cnot d,a0
   cnot d,a1
   display_binary

# error injection
.error_injection
   x d
   display_binary


# error correction
#.error_correction

# decoding
.decoding
   cnot d,a0
   cnot d,a1
   toffoli a0,a1,d
   display
   # display_binary
 

error_model depolarizing_channel, 0.01

