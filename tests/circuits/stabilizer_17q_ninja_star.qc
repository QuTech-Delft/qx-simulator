version 1.0

# 17 qubits ninja-star stabilizer

# qubit definition
qubits 17 

# qubits mapping :
#
# q[0]  -> X44
# q[1]  -> D1
# q[2]  -> D3
# q[3]  -> D5
# q[4]  -> Z10
# q[5]  -> X2
# q[6]  -> Z4
# q[7]  -> D11
# q[8]  -> D13
# q[9]  -> D15
# q[10] -> Z12
# q[11] -> X14
# q[12] -> Z16
# q[13] -> D21
# q[14] -> D23
# q[15] -> D25
# q[16] -> X22 

# display the previous state (from a previous round if exists)
.initial_state
  display_binary

# init ancillas
.init
   c-x b[0],q[0]   # init X44 to zero
   c-x b[4],q[4]   # init Z10 to zero
   c-x b[5],q[5]   # init X2  to zero
   c-x b[6],q[6]   # init Z4  to zero
   c-x b[10],q[10] # init Z12 to zero
   c-x b[11],q[11] # init X14 to zero
   c-x b[12],q[12] # init Z16 to zero
   c-x b[16],q[16] # init X22 to zero

   # debug : display intermediate state
   display_binary 

   h q[0]   # X44
   h q[5]   # X2
   h q[11]  # X14
   h q[16]

   display_binary

# initialization
.init
   measure q[0]  # X44
   
.upper_right_x

.lower_left_x

.upper_left

.lower_right


# measure ancillas
.measurements
   measure q[0]  # X44
   measure q[4]
   measure q[5]  # X2 
   measure q[6]
   measure q[10]
   measure q[11]
   measure q[12]
   measure q[16]

# init ancillas
.init
   c-x b[0],q[0]   # init X44 to zero
   c-x b[4],q[4]   # init Z10 to zero
   c-x b[5],q[5]   # init X2  to zero
   c-x b[6],q[6]   # init Z4  to zero
   c-x b[10],q[10] # init Z12 to zero
   c-x b[11],q[11] # init X14 to zero
   c-x b[12],q[12] # init Z16 to zero
   c-x b[16],q[16] # init X22 to zero

   # debug : display intermediate state
   display_binary 

   h q[0]  # X44
   h q[5]  # X2
   h q[11]
   h q[16]

# 1st round
.round_1
   cnot q[1],q[4]
   cnot q[5],q[2]  # X2
   cnot q[3],q[6]

   cnot q[8],q[10]
   cnot q[11],q[9]

   cnot q[16],q[14]

   # display_binary

# 2nd round
.round_2
   cnot q[5],q[1] # X2
   cnot q[2],q[6]

   cnot q[7],q[10]
   cnot q[11],q[8]
   cnot q[9],q[12]

   cnot q[16],q[13]

   display_binary


# 3rd round
.round_3
   cnot q[0],q[3]  # X44

   cnot q[7],q[4]
   cnot q[5],q[8]  # X2
   cnot q[9],q[6]

   cnot q[14],q[10]
   cnot q[11],q[15]

   display_binary

# 4th round
.round_4
   cnot q[0],q[2]  # X44
   cnot q[5],q[7]  # X2
   cnot q[8],q[6]

   cnot q[13],q[10]
   cnot q[11],q[14]
   cnot q[15],q[12]

   display_binary

   h q[0]
   h q[5]  # X2
   h q[11]
   h q[16]

   display_binary

# measure ancillas
.measurements
   measure q[0]
   measure q[4]
   measure q[5]  # X2 
   measure q[6]
   measure q[10]
   measure q[11]
   measure q[12]
   measure q[16]

  # display final state
  display_binary

