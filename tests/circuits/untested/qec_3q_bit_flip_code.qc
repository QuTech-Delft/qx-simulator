version 1.0

# file   : qec_3q_bit_flip_code.qc
# author : Nader Khammassi
# brief  : quantum error correction test
#          circuit for a basic 3 qubits bitflip code

# define 5 qubits : 3 data qubits and 2 ancillas (syndrome qubits)
qubits 5 

# initialize logical qubit
.init
   x q[0]
   display

# encoding
.encoding
   cnot q[0],q[1]
   cnot q[0],q[2]
   display_binary

# error injection
.error_injection
   x q[2] 
   display


# parity check
.parity_check
   cnot q[0],q[3]
   cnot q[1],q[3]
   cnot q[0],q[4]
   cnot q[2],q[4]
   measure q[3]
   measure q[4]
   display_binary


# error correction circuit can be described here 
.error_correction
  # x q[4]
  # toffoli q[3],q[4],q[1]
  # x q[4]
  # not q[4]
  # c-x b3,b4,q[1]
  
  display


# decoding
.decoding
   cnot q[0],q[2]
   cnot q[0],q[1]
   # display
   display_binary

# error_model depolarizing_channel,0.1
