
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



# initialization circuit
.initialization

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
.init_x_z
   cx b18,x44   # init X44 to zero
   cx b19,z10   # init Z10 to zero
   cx b20,x2   # init X2  to zero
   cx b21,z4   # init Z4  to zero
   cx b22,z12 # init Z12 to zero
   cx b23,x14 # init X14 to zero
   cx b24,z16 # init Z16 to zero
   cx b25,x22 # init X22 to zero


   # debug : display intermediate state
   display_binary

   h x44
   h x2
   h x14
   h x22
   display_binary

# 1st round
.round_1
   cnot da1,z10
   cnot x2,da3
   cnot da5,z4

   cnot da13,z12
   cnot x14,da15

   cnot x22,da23
   display_binary

# 2nd round
.round_2
   cnot x2,da1
   cnot da3,z4

   cnot da11,z12
   cnot x14,da13
   cnot da15,z16

   cnot x22,da21
   display_binary


# 3rd round
.round_3
   cnot x44,da5 

   cnot da11,z10
   cnot x2,da13
   cnot da15,z4

   cnot da23,z12
   cnot x14,da25
   display_binary

# 4th round
.round_4
   cnot x44,da3

   cnot x2,da11
   cnot da13,z4

   cnot da21,z12
   cnot x14,da23
   cnot da25,z16
   display_binary

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

.logical_0_state
   display

#.error_injection
#  z da1

.set_logical_1

   x da5
   x da13
   x da21

.logical_1_state
   display




