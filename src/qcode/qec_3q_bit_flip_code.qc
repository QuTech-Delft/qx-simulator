
# quantum error correction test
# 3 qubit bit flip correction code

# qubit definition
qubits 5 

# init logical gate
.init
   x q0
   display_binary

# encoding
.encoding
   cnot q0,q1
   cnot q0,q2
   display_binary

# error injection
.error_injection
   x q1
   display_binary


# parity check
.parity_check
   cnot q0,q3
   cnot q1,q3
   cnot q0,q4
   cnot q2,q4
   measure q3
   measure q4
   # display
   display_binary

# error correction
#.error_correction

# decoding
.decoding
   cnot q0,q2
   cnot q0,q1
   # display
   display_binary

