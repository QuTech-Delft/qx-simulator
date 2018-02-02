
# file   : qec_3q_bit_flip_code.qc
# author : Nader Khammassi
# brief  : quantum error correction test
#          circuit for a basic 3 qubits bitflip code

# define 5 qubits : 3 data qubits and 2 ancillas (syndrome qubits)
qubits 5 

# initialize logical qubit
.init
   x q0
   display

# encoding
.encoding
   cnot q0,q1
   cnot q0,q2
   display_binary

# error injection
.error_injection
   x q2
   display


# parity check
.parity_check
   cnot q0,q3
   cnot q1,q3
   cnot q0,q4
   cnot q2,q4
   measure q3
   measure q4
   display_binary


# error correction circuit can be described here 
.error_correction  
  # syn : 11 -> q0   
  c-x b4,b3,q0
  # syn : 01 -> flip q1
  not b4  
  c-x b4,b3,q1
  not b4
  # syn : 10 -> q2
  not b3  
  c-x b4,b3,q2
  not b3 
  
  display


# decoding
.decoding
   cnot q0,q2
   cnot q0,q1
   # display
   display_binary

# error_model depolarizing_channel,0.1