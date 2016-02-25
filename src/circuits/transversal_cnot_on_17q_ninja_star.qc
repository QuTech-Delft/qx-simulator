
# 17 qubits ninja-star surface code

# qubit definition
qubits 26 


map q0,da1
map q1,da3
map q2,da5
map q3,da11
map q4,da13
map q5,da15
map q6,da21
map q7,da23
map q8,da25
map q9,db1   
map q10,db3
map q11,db5
map q12,db11
map q13,db13
map q14,db15
map q15,db21
map q16,db23
map q17,db25

map q18,x44
map q19,z10
map q20,x2
map q21,z4
map q22,z12
map q23,x14
map q24,z16
map q25,x22 


# display the previous state (from a previous round if exists)
# .previous_round_outcome
#  display_binary

# initialization circuit to put qubit A on a logical zero state
.initializing_first_qubit_to_logical_0_state

   cx b18,x44   # init x44 to zero
   h x44
   cnot x44,da5 
   cnot x44,da3
   h x44
   measure x44
   cz b18,da5


   cx b25,x22 # init x22 to zero
   h x22
   cnot x22,da23
   cnot x22,da21
   h x22
   measure x22
   cz b25,da21


   cx b20,x2   # init X2  to zero
   h x2
   cnot x2,da3
   cnot x2,da1
   cnot x2,da13
   cnot x2,da11
   h x2
   measure x2
   cz b20,da1


   cx b23,x14 # init X14 to zero
   h x14
   cnot x14,da15
   cnot x14,da13
   cnot x14,da25
   cnot x14,da23
   h x14
   measure x14
   cz b23,da25

# init ancillas
.start_surface_code_cycle

   cx b18,x44   # init X44 to zero
   cx b19,z10   # init Z10 to zero
   cx b20,x2   # init X2  to zero
   cx b21,z4   # init Z4  to zero
   cx b22,z12 # init Z12 to zero
   cx b23,x14 # init X14 to zero
   cx b24,z16 # init Z16 to zero
   cx b25,x22 # init X22 to zero

   h x44
   h x2
   h x14
   h x22
   # display_binary

# 1st round
.round_1
   cnot da1,z10
   cnot x2,da3
   cnot da5,z4

   cnot da13,z12
   cnot x14,da15

   cnot x22,da23
   # display_binary

# 2nd round
.round_2
   cnot x2,da1
   cnot da3,z4

   cnot da11,z12
   cnot x14,da13
   cnot da15,z16

   cnot x22,da21
   # display_binary


# 3rd round
.round_3
   cnot x44,da5 

   cnot da11,z10
   cnot x2,da13
   cnot da15,z4

   cnot da23,z12
   cnot x14,da25
   # display_binary

# 4th round
.round_4
   cnot x44,da3

   cnot x2,da11
   cnot da13,z4

   cnot da21,z12
   cnot x14,da23
   cnot da25,z16
   # display_binary

.syndromes_measurements
   h x44
   h x2
   h x14
   h x22

   measure x44
   measure z10
   measure x2 
   measure z4
   measure z12
   measure x14
   measure z16
   measure x22

   display_binary


.set_first_qubit_to_logical_1_state

   x da5
   x da13
   x da21

   display


.initialize_second_qubit_to_logical_0_state

   cx b18,x44   # init x44 to zero
   h x44
   cnot x44,db5 
   cnot x44,db3
   h x44
   measure x44
   cz b18,db5


   cx b25,x22 # init x22 to zero
   h x22
   cnot x22,db23
   cnot x22,db21
   h x22
   measure x22
   cz b25,db21


   cx b20,x2   # init X2  to zero
   h x2
   cnot x2,db3
   cnot x2,db1
   cnot x2,db13
   cnot x2,db11
   h x2
   measure x2
   cz b20,db1


   cx b23,x14 # init X14 to zero
   h x14
   cnot x14,db15
   cnot x14,db13
   cnot x14,db25
   cnot x14,db23
   h x14
   measure x14
   cz b23,db25


.start_surface_code_cycle
   # init ancillas
   cx b18,x44 # init X44 to zero
   cx b19,z10 # init Z10 to zero
   cx b20,x2  # init X2  to zero
   cx b21,z4  # init Z4  to zero
   cx b22,z12 # init Z12 to zero
   cx b23,x14 # init X14 to zero
   cx b24,z16 # init Z16 to zero
   cx b25,x22 # init X22 to zero

   # debug : display intermediate state
   # display_binary

   h x44
   h x2
   h x14
   h x22
   # display_binary

# 1st round
.round_1
   cnot db1,z10
   cnot x2,db3
   cnot db5,z4

   cnot db13,z12
   cnot x14,db15

   cnot x22,db23
   # display_binary

# 2nd round
.round_2
   cnot x2,db1
   cnot db3,z4

   cnot db11,z12
   cnot x14,db13
   cnot db15,z16

   cnot x22,db21
   # display_binary


# 3rd round
.round_3
   cnot x44,db5 

   cnot db11,z10
   cnot x2,db13
   cnot db15,z4

   cnot db23,z12
   cnot x14,db25
   # display_binary

# 4th round
.round_4
   cnot x44,db3

   cnot x2,db11
   cnot db13,z4

   cnot db21,z12
   cnot x14,db23
   cnot db25,z16
   # display_binary

.measurements
   h x44
   h x2
   h x14
   h x22

   measure x44
   measure z10
   measure x2 
   measure z4
   measure z12
   measure x14
   measure z16
   measure x22
   
   display_binary

.set_second_logical_qubit_to_logical_1_state

   x db5
   x db13
   x db21


.state_before_the_transversal_cnot
   display

# apply a transversal cnot on the two logical qubits
.transversal_cnot

   cnot da1,db1
   cnot da3,db3
   cnot da5,db5
   cnot da11,db11
   cnot da13,db13
   cnot da15,db15
   cnot da21,db21
   cnot da23,db23
   cnot da25,db25

.state_after_the_transversal_cnot
   # display the result state
   display
