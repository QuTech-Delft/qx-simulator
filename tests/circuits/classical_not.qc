version 1.0

# file   : classical_not.qc
# author : Fer Grooteman
# brief  : test classical not

qubits 4

.mapping
  map b[0:3], bitcontroller

.init
  x q[0:3]
  measure q[0:3]
  display

.not_single_bit
  not b[0]
  display
  not b[1]
  display
  not b[2]
  display
  not b[3]
  display
  not b[3]
  display
  not b[2]
  display
  not b[1]
  display
  not b[0]
  display

.not_two_bits
  not b[0:1]
  display
  not b[2:3]
  display
  not b[2,3]
  display
  not b[0,1]
  display

.not_four_bits
  not b[0:3]
  display
  not b[0,1,2,3]
  display

.not_four_mapped_bits
  not bitcontroller
  display
  not bitcontroller
  display


