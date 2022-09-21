version 1.0

# quantum error correction test
# 3 qubits phase/sign flip correction code

# qubit definition
qubits 3 

# init logical gate
.init
   x q[0]
   display_binary

# encoding
.encoding
   cnot q[0],q[1]
   cnot q[0],q[2]
   h q[0]
   h q[1]
   h q[2]
   display

# error injection
.error_injection
   z q[0]
   display


# decoding 
.decoding
   h q[0]
   h q[1]
   h q[2]
   cnot q[0],q[1]
   cnot q[0],q[2]
   toffoli q[1],q[2],q[0]
   # display
   display

#.error_correction


