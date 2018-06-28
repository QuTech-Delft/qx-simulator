
# quantum error correction test
# 3 qubits phase/sign flip correction code

# qubit definition
qubits 3 

# init logical gate
.init
   x q0
   display_binary

# encoding
.encoding
   cnot q0,q1
   cnot q0,q2
   h q0
   h q1
   h q2
   display

# error injection
.error_injection
   z q0
   display


# decoding 
.decoding
   h q0
   h q1
   h q2
   cnot q0,q1
   cnot q0,q2
   toffoli q1,q2,q0
   display

#error_model depolarizing_channel,0.1


