version 1.0

# quantum error correction test
# 3 qubit bit flip correction code

# qubit definition
qubits 5 

# init logical gate
.init
   x q[0]
   display_binary

# encoding
.encoding
   cnot q[0],q[1]
   cnot q[0],q[2]
   display_binary

# error injection
.error_injection
   x q[1]
   display_binary


# parity check
.parity_check
   cnot q[0],q[3]
   cnot q[1],q[3]
   cnot q[0],q[4]
   cnot q[2],q[4]
   measure q[3]
   measure q[4]
   # display
   display_binary

# error correction
#.error_correction

# decoding
.decoding
   cnot q[0],q[2]
   cnot q[0],q[1]
   # display
   display_binary

