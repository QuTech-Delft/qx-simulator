version 1.0

# file   : qec_shor9_code.qc
# author : Nader Khammassi
# brief  : quantum error correction (shor's 9 qubits code)
# date   : 11/02/2016
# 

qubits 9

map q[0],p0
map q[1],x00
map q[2],x01
map q[3],p1
map q[4],x10
map q[5],x11
map q[6],p2
map q[7],x20
map q[8],x21


.init
  rx p0,1.3
  ry p0,0.3
  # x q[0]
  # display

.phase_flip_encoding
  cnot p0,p1
  cnot p0,p2

  h p0
  h p1
  h p2
  display


.bit_flip_encoding
  cnot p0,x00
  cnot p0,x01

  cnot p1,x10
  cnot p1,x11

  cnot p2,x20
  cnot p2,x21

  display


.error_injection
  x q[3]
  # z q[0]
  # x q[1]


.bit_flip_decoding_and_correction
  cnot p0,x01
  cnot p0,x00
  toffoli x00,x01,p0

  cnot p1,x11
  cnot p1,x10
  toffoli x10,x11,p1

  cnot p2,x21
  cnot p2,x20
  toffoli x20,x21,p2

  display

.phase_flip_decoding_and_correction

  h p0
  h p1
  h p2
  
  cnot p0,p2
  cnot p0,p1
  toffoli p1,p2,p0

  display 

  # measure

  # display_binary
  

# error_model depolarizing_channel, 0.1
  
