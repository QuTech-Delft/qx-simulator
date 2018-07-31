version 1.0

# 17 qubits ninja-star surface code

# qubit definition
qubits 26 


map q[0],da1
map q[1],da3
map q[2],da5
map q[3],da11
map q[4],da13
map q[5],da15
map q[6],da21
map q[7],da23
map q[8],da25
map q[9],db1   
map q[10],db3
map q[11],db5
map q[12],db11
map q[13],db13
map q[14],db15
map q[15],db21
map q[16],db23
map q[17],db25

map q[18],x44
map q[19],z10
map q[20],x2
map q[21],z4
map q[22],z12
map q[23],x14
map q[24],z16
map q[25],x22 



# initialization circuit
.initialization

   c-x b[18],x44   # init x44 to zero
   h x44
   cnot x44,da5 
   cnot x44,da3
   h x44
   measure x44
   c-z b[18],da5


   c-x b[25],x22 # init x22 to zero
   h x22
   cnot x22,da23
   cnot x22,da21
   h x22
   measure x22
   c-z b[25],da21


   c-x b[20],x2   # init X2  to zero
   h x2
   cnot x2,da3
   cnot x2,da1
   cnot x2,da13
   cnot x2,da11
   h x2
   measure x2
   c-z b[20],da1


   c-x b[23],x14 # init X14 to zero
   h x14
   cnot x14,da15
   cnot x14,da13
   cnot x14,da25
   cnot x14,da23
   h x14
   measure x14
   c-z b[23],da25

# init ancillas
.init_x_z
   c-x b[18],x44   # init X44 to zero
   c-x b[19],z10   # init Z10 to zero
   c-x b[20],x2   # init X2  to zero
   c-x b[21],z4   # init Z4  to zero
   c-x b[22],z12 # init Z12 to zero
   c-x b[23],x14 # init X14 to zero
   c-x b[24],z16 # init Z16 to zero
   c-x b[25],x22 # init X22 to zero


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




