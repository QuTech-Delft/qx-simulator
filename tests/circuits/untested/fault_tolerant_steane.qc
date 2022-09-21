version 1.0

qubits 24


# define logical qubits
map q[0],l0_0
map q[1],l0_1
map q[2],l0_2
map q[3],l0_3
map q[4],l0_4
map q[5],l0_5
map q[6],l0_6
map q[6],phy0
map q[7],l1_0
map q[8],l1_1
map q[9],l1_2
map q[10],l1_3
map q[11],l1_4
map q[12],l1_5
map q[13],l1_6
map q[13],phy1
map q[14],l2_0
map q[15],l2_1
map q[16],l2_2
map q[17],l2_3
map q[18],l2_4
map q[19],l2_5
map q[20],l2_6
map q[20],phy2

# define ancilla qubits
map q[21],syn0
map b[21],bsyn0
map q[22],syn1
map b[22],bsyn1
map q[23],syn2
map b[23],bsyn2

.init
   x phy0
   display_binary

# encode physical qubit 0
.encode_l0
   h l0_0
   h l0_1
   h l0_2
   cnot l0_6,l0_4
   cnot l0_6,l0_5
   cnot l0_0,l0_3
   cnot l0_0,l0_5
   cnot l0_0,l0_6
   cnot l0_1,l0_3
   cnot l0_1,l0_4
   cnot l0_1,l0_6
   cnot l0_2,l0_3
   cnot l0_2,l0_4
   cnot l0_2,l0_5

# encode physical qubit 1
.encode_l1
   h l1_0
   h l1_1
   h l1_2
   cnot l1_6,l1_4
   cnot l1_6,l1_5
   cnot l1_0,l1_3
   cnot l1_0,l1_5
   cnot l1_0,l1_6
   cnot l1_1,l1_3
   cnot l1_1,l1_4
   cnot l1_1,l1_6
   cnot l1_2,l1_3
   cnot l1_2,l1_4
   cnot l1_2,l1_5

# encode physical qubit 2
.encode_l2
   h l2_0
   h l2_1
   h l2_2
   cnot l2_6,l2_4
   cnot l2_6,l2_5
   cnot l2_0,l2_3
   cnot l2_0,l2_5
   cnot l2_0,l2_6
   cnot l2_1,l2_3
   cnot l2_1,l2_4
   cnot l2_1,l2_6
   cnot l2_2,l2_3
   cnot l2_2,l2_4
   cnot l2_2,l2_5

# diagnose bitflip on qubit 0
.diagnose_bitflip_l0
   prep_z syn0
   cnot l0_3,syn0
   cnot l0_2,syn0
   cnot l0_1,syn0
   cnot l0_0,syn0
   measure syn0
   prep_z syn1
   cnot l0_5,syn1
   cnot l0_4,syn1
   cnot l0_1,syn1
   cnot l0_0,syn1
   measure syn1
   prep_z syn2
   cnot l0_6,syn2
   cnot l0_4,syn2
   cnot l0_2,syn2
   cnot l0_0,syn2
   measure syn2

# correct phaseflip on qubit 0
.correct_bitflip_l0
   c-x bsyn0,bsyn1,bsyn2,l0_0
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l0_1
   not bsyn2
   not bsyn1
   c-x bsyn0,bsyn1,bsyn2,l0_2
   not bsyn1
   not bsyn1
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l0_3
   not bsyn1
   not bsyn2
   not bsyn0
   c-x bsyn0,bsyn1,bsyn2,l0_4
   not bsyn0
   not bsyn0
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l0_5
   not bsyn0
   not bsyn2
   not bsyn0
   not bsyn1
   c-x bsyn0,bsyn1,bsyn2,l0_6
   not bsyn0
   not bsyn1

# diagnose phaseflip on qubit 0
.diagnose_bitflip_l0
   h l0_0
   h l0_1
   h l0_2
   h l0_3
   h l0_4
   h l0_5
   h l0_6
   prep_z syn0
   cnot l0_3,syn0
   cnot l0_2,syn0
   cnot l0_1,syn0
   cnot l0_0,syn0
   measure syn0
   prep_z syn1
   cnot l0_5,syn1
   cnot l0_4,syn1
   cnot l0_1,syn1
   cnot l0_0,syn1
   measure syn1
   prep_z syn2
   cnot l0_6,syn2
   cnot l0_4,syn2
   cnot l0_2,syn2
   cnot l0_0,syn2
   measure syn2
   h l0_0
   h l0_1
   h l0_2
   h l0_3
   h l0_4
   h l0_5
   h l0_6

# diagnose bitflip on qubit 1
.diagnose_bitflip_l1
   prep_z syn0
   cnot l1_3,syn0
   cnot l1_2,syn0
   cnot l1_1,syn0
   cnot l1_0,syn0
   measure syn0
   prep_z syn1
   cnot l1_5,syn1
   cnot l1_4,syn1
   cnot l1_1,syn1
   cnot l1_0,syn1
   measure syn1
   prep_z syn2
   cnot l1_6,syn2
   cnot l1_4,syn2
   cnot l1_2,syn2
   cnot l1_0,syn2
   measure syn2

# correct phaseflip on qubit 1
.correct_bitflip_l1
   c-x bsyn0,bsyn1,bsyn2,l1_0
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l1_1
   not bsyn2
   not bsyn1
   c-x bsyn0,bsyn1,bsyn2,l1_2
   not bsyn1
   not bsyn1
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l1_3
   not bsyn1
   not bsyn2
   not bsyn0
   c-x bsyn0,bsyn1,bsyn2,l1_4
   not bsyn0
   not bsyn0
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l1_5
   not bsyn0
   not bsyn2
   not bsyn0
   not bsyn1
   c-x bsyn0,bsyn1,bsyn2,l1_6
   not bsyn0
   not bsyn1

# diagnose phaseflip on qubit 1
.diagnose_bitflip_l1
   h l1_0
   h l1_1
   h l1_2
   h l1_3
   h l1_4
   h l1_5
   h l1_6
   prep_z syn0
   cnot l1_3,syn0
   cnot l1_2,syn0
   cnot l1_1,syn0
   cnot l1_0,syn0
   measure syn0
   prep_z syn1
   cnot l1_5,syn1
   cnot l1_4,syn1
   cnot l1_1,syn1
   cnot l1_0,syn1
   measure syn1
   prep_z syn2
   cnot l1_6,syn2
   cnot l1_4,syn2
   cnot l1_2,syn2
   cnot l1_0,syn2
   measure syn2
   h l1_0
   h l1_1
   h l1_2
   h l1_3
   h l1_4
   h l1_5
   h l1_6

# diagnose bitflip on qubit 2
.diagnose_bitflip_l2
   prep_z syn0
   cnot l2_3,syn0
   cnot l2_2,syn0
   cnot l2_1,syn0
   cnot l2_0,syn0
   measure syn0
   prep_z syn1
   cnot l2_5,syn1
   cnot l2_4,syn1
   cnot l2_1,syn1
   cnot l2_0,syn1
   measure syn1
   prep_z syn2
   cnot l2_6,syn2
   cnot l2_4,syn2
   cnot l2_2,syn2
   cnot l2_0,syn2
   measure syn2

# correct phaseflip on qubit 2
.correct_bitflip_l2
   c-x bsyn0,bsyn1,bsyn2,l2_0
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l2_1
   not bsyn2
   not bsyn1
   c-x bsyn0,bsyn1,bsyn2,l2_2
   not bsyn1
   not bsyn1
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l2_3
   not bsyn1
   not bsyn2
   not bsyn0
   c-x bsyn0,bsyn1,bsyn2,l2_4
   not bsyn0
   not bsyn0
   not bsyn2
   c-x bsyn0,bsyn1,bsyn2,l2_5
   not bsyn0
   not bsyn2
   not bsyn0
   not bsyn1
   c-x bsyn0,bsyn1,bsyn2,l2_6
   not bsyn0
   not bsyn1

# diagnose phaseflip on qubit 2
.diagnose_bitflip_l2
   h l2_0
   h l2_1
   h l2_2
   h l2_3
   h l2_4
   h l2_5
   h l2_6
   prep_z syn0
   cnot l2_3,syn0
   cnot l2_2,syn0
   cnot l2_1,syn0
   cnot l2_0,syn0
   measure syn0
   prep_z syn1
   cnot l2_5,syn1
   cnot l2_4,syn1
   cnot l2_1,syn1
   cnot l2_0,syn1
   measure syn1
   prep_z syn2
   cnot l2_6,syn2
   cnot l2_4,syn2
   cnot l2_2,syn2
   cnot l2_0,syn2
   measure syn2
   h l2_0
   h l2_1
   h l2_2
   h l2_3
   h l2_4
   h l2_5
   h l2_6

# ft cnot 0,1
.ft_cnot_0_1
   cnot l0_0,l1_0
   cnot l0_1,l1_1
   cnot l0_2,l1_2
   cnot l0_3,l1_3
   cnot l0_4,l1_4
   cnot l0_5,l1_5
   cnot l0_6,l1_6

# ft cnot 0,2
.ft_cnot_0_2
   cnot l0_0,l2_0
   cnot l0_1,l2_1
   cnot l0_2,l2_2
   cnot l0_3,l2_3
   cnot l0_4,l2_4
   cnot l0_5,l2_5
   cnot l0_6,l2_6

# decode logical qubit 0
.decode_l0
   cnot l0_2,l0_5
   cnot l0_2,l0_4
   cnot l0_2,l0_3
   cnot l0_1,l0_6
   cnot l0_1,l0_4
   cnot l0_1,l0_3
   cnot l0_0,l0_6
   cnot l0_0,l0_5
   cnot l0_0,l0_3
   cnot l0_6,l0_5
   cnot l0_6,l0_4
   h l0_2
   h l0_1
   h l0_0

# decode logical qubit 1
.decode_l1
   cnot l1_2,l1_5
   cnot l1_2,l1_4
   cnot l1_2,l1_3
   cnot l1_1,l1_6
   cnot l1_1,l1_4
   cnot l1_1,l1_3
   cnot l1_0,l1_6
   cnot l1_0,l1_5
   cnot l1_0,l1_3
   cnot l1_6,l1_5
   cnot l1_6,l1_4
   h l1_2
   h l1_1
   h l1_0

# decode logical qubit 2
.decode_l2
   cnot l2_2,l2_5
   cnot l2_2,l2_4
   cnot l2_2,l2_3
   cnot l2_1,l2_6
   cnot l2_1,l2_4
   cnot l2_1,l2_3
   cnot l2_0,l2_6
   cnot l2_0,l2_5
   cnot l2_0,l2_3
   cnot l2_6,l2_5
   cnot l2_6,l2_4
   h l2_2
   h l2_1
   h l2_0

.result
   measure_all
   display_binary
