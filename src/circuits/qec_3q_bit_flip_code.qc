
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
   x q0
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
.error_correction
   toffoli q3,q4,q0   # correct for |11> syndrome
   cx b3,q4           
   toffoli q3,q4,q1   # correct for |10> syndrome
   cx b3,q4           # restore ancillas
   cx b4,q3
   toffoli q3,q4,q2   # correct for |01> syndrome
   display_binary

# decoding
.decoding
   cnot q0,q2
   cnot q0,q1
   # display
   display_binary

