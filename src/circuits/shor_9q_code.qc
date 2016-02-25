# 11/02/2016
# Shor's 9 qubits code

qubits 9

map q0,p0
map q1,p1
map q2,p2
map q3,x00
map q4,x01
map q5,x10
map q6,x11
map q7,x20
map q8,x21


.init
  x p0
  display_binary

.phase_flip_encoding
  cnot p0,p1
  cnot p0,p2

  h p0
  h p1
  h p2
  display_binary


.bit_flip_encoding
  cnot p0,x00
  cnot p0,x01

  cnot p1,x10
  cnot p1,x11

  cnot p2,x20
  cnot p2,x21

  display_binary


.error_injection
  x p2


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

  display_binary

.phase_flip_decoding_and_correction

  h p0
  h p1
  h p2
  
  cnot p0,p2
  cnot p0,p1
  toffoli p1,p2,p0

  measure

  display_binary
  

  
