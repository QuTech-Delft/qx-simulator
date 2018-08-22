
# 17 qubits ninja-star stabilizer

# qubit definition
qubits 17 

# qubits mapping :
#
# q0  -> X44
# q1  -> D1
# q2  -> D3
# q3  -> D5
# q4  -> Z10
# q5  -> X2
# q6  -> Z4
# q7  -> D11
# q8  -> D13
# q9  -> D15
# q10 -> Z12
# q11 -> X14
# q12 -> Z16
# q13 -> D21
# q14 -> D23
# q15 -> D25
# q16 -> X22 

# display the previous state (from a previous round if exists)
.initial_state
  display_binary

# init ancillas
.init
   cx b0,q0   # init X44 to zero
   cx b4,q4   # init Z10 to zero
   cx b5,q5   # init X2  to zero
   cx b6,q6   # init Z4  to zero
   cx b10,q10 # init Z12 to zero
   cx b11,q11 # init X14 to zero
   cx b12,q12 # init Z16 to zero
   cx b16,q16 # init X22 to zero

   # debug : display intermediate state
   display_binary 

   h q0   # X44
   h q5   # X2
   h q11  # X14
   h q16

   display_binary

# initialization
.init
   measure q0  # X44
   
.upper_right_x

.lower_left_x

.upper_left

.lower_right


# measure ancillas
.measurements
   measure q0  # X44
   measure q4
   measure q5  # X2 
   measure q6
   measure q10
   measure q11
   measure q12
   measure q16

# init ancillas
.init
   cx b0,q0   # init X44 to zero
   cx b4,q4   # init Z10 to zero
   cx b5,q5   # init X2  to zero
   cx b6,q6   # init Z4  to zero
   cx b10,q10 # init Z12 to zero
   cx b11,q11 # init X14 to zero
   cx b12,q12 # init Z16 to zero
   cx b16,q16 # init X22 to zero

   # debug : display intermediate state
   display_binary 

   h q0  # X44
   h q5  # X2
   h q11
   h q16

# 1st round
.round_1
   cnot q1,q4
   cnot q5,q2  # X2
   cnot q3,q6

   cnot q8,q10
   cnot q11,q9

   cnot q16,q14

   # display_binary

# 2nd round
.round_2
   cnot q5,q1 # X2
   cnot q2,q6

   cnot q7,q10
   cnot q11,q8
   cnot q9,q12

   cnot q16,q13

   display_binary


# 3rd round
.round_3
   cnot q0,q3  # X44

   cnot q7,q4
   cnot q5,q8  # X2
   cnot q9,q6

   cnot q14,q10
   cnot q11,q15

   display_binary

# 4th round
.round_4
   cnot q0,q2  # X44
   cnot q5,q7  # X2
   cnot q8,q6

   cnot q13,q10
   cnot q11,q14
   cnot q15,q12

   display_binary

   h q0
   h q5  # X2
   h q11
   h q16

   display_binary

# measure ancillas
.measurements
   measure q0
   measure q4
   measure q5  # X2 
   measure q6
   measure q10
   measure q11
   measure q12
   measure q16

  # display final state
  display_binary

