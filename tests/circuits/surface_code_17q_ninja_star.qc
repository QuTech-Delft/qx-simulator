
# 17 qubits ninja-star stabilizer

# qubit definition
qubits 17 

map q0,x44
map q1,d1
map q2,d3
map q3,d5
map q4,z10
map q5,x2
map q6,z4
map q7,d11
map q8,d13
map q9,d15
map q10,z12
map q11,x14
map q12,z16
map q13,d21
map q14,d23
map q15,d25
map q16,x22 


# display the previous state (from a previous round if exists)
# .previous_round_outcome
#  display_binary

# initialization circuit
.initialization

   # cx b0,x44   # init x44 to zero
   prepz x44
   h x44
   cnot x44,d5 
   cnot x44,d3
   h x44
   measure x44
   cz b0,d5


   # cx b16,x22 # init x22 to zero
   prepz x22
   h x22
   cnot x22,d23
   cnot x22,d21
   h x22
   measure x22
   cz b16,d21


   # cx b5,x2   # init X2  to zero
   prepz x2
   h x2
   cnot x2,d3
   cnot x2,d1
   cnot x2,d13
   cnot x2,d11
   h x2
   measure x2
   cz b5,d1


   # cx b11,x14 # init X14 to zero
   prepz x14
   h x14
   cnot x14,d15
   cnot x14,d13
   cnot x14,d25
   cnot x14,d23
   h x14
   measure x14
   cz b11,d25

   display

.error_injection
    # x d13

# init ancillas
# .init_x_z
.surface_code_cycle
   # cx b0,x44   # init X44 to zero
   # cx b4,z10   # init Z10 to zero
   # cx b5,x2   # init X2  to zero
   # cx b6,z4   # init Z4  to zero
   # cx b10,z12 # init Z12 to zero
   # cx b11,x14 # init X14 to zero
   # cx b12,z16 # init Z16 to zero
   # cx b16,x22 # init X22 to zero

   prepz  x44 
   prepz  z10 
   prepz  x2  
   prepz  z4  
   prepz  z12
   prepz  x14
   prepz  z16
   prepz  x22

   # debug : display intermediate state
   # display_binary

   h x44
   h x2
   h x14
   h x22
   # display_binary

# 1st round
# .round_1
   cnot d1,z10
   cnot x2,d3
   cnot d5,z4

   cnot d13,z12
   cnot x14,d15

   cnot x22,d23
   # display_binary

# 2nd round
# .round_2
   cnot x2,d1
   cnot d3,z4

   cnot d11,z12
   cnot x14,d13
   cnot d15,z16

   cnot x22,d21
   # display_binary


# 3rd round
# .round_3
   cnot x44,d5 

   cnot d11,z10
   cnot x2,d13
   cnot d15,z4

   cnot d23,z12
   cnot x14,d25
   # display_binary

# 4th round
# .round_4
   cnot x44,d3

   cnot x2,d11
   cnot d13,z4

   cnot d21,z12
   cnot x14,d23
   cnot d25,z16
   # display_binary

   h x44
   h x2
   h x14
   h x22

.syndromes_measurement

   measure x44
   measure z10
   measure x2 
   measure z4
   measure z12
   measure x14
   measure z16
   measure x22

   display


# error_model depolarizing_channel, 0.001
