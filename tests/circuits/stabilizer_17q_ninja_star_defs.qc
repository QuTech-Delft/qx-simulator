version 1.0

# 17 qubits ninja-star stabilizer

# qubit definition
qubits 17 

map q[0],x44
map q[1],d1
map q[2],d3
map q[3],d5
map q[4],z10
map q[5],x2
map q[6],z4
map q[7],d11
map q[8],d13
map q[9],d15
map q[10],z12
map q[11],x14
map q[12],z16
map q[13],d21
map q[14],d23
map q[15],d25
map q[16],x22 


# display the previous state (from a previous round if exists)
# .previous_round_outcome
#  display_binary

# initialization circuit
.initialization

   c-x b[0],x44   # init x44 to zero
   h x44
   cnot x44,d5 
   cnot x44,d3
   h x44
   measure x44
   c-z b[0],d5


   c-x b[16],x22 # init x22 to zero
   h x22
   cnot x22,d23
   cnot x22,d21
   h x22
   measure x22
   c-z b[16],d21


   c-x b[5],x2   # init X2  to zero
   h x2
   cnot x2,d3
   cnot x2,d1
   cnot x2,d13
   cnot x2,d11
   h x2
   measure x2
   c-z b[5],d1


   c-x b[11],x14 # init X14 to zero
   h x14
   cnot x14,d15
   cnot x14,d13
   cnot x14,d25
   cnot x14,d23
   h x14
   measure x14
   c-z b[11],d25

   # display_binary

#.error_injection
#  z d1

# init ancillas
# .init_x_z
.surface_code_cycle
   c-x b[0],x44   # init X44 to zero
   c-x b[4],z10   # init Z10 to zero
   c-x b[5],x2   # init X2  to zero
   c-x b[6],z4   # init Z4  to zero
   c-x b[10],z12 # init Z12 to zero
   c-x b[11],x14 # init X14 to zero
   c-x b[12],z16 # init Z16 to zero
   c-x b[16],x22 # init X22 to zero

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

   display_binary


error_model depolarizing_channel, 0.001
