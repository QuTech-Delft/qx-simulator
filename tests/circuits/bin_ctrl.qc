version 1.0

# file   : bin_ctrl.qc
# author : Nader Khammassi
# brief  : binary-controlled gates test 


qubits 4

.init
  x q[0:1]
  measure q[0:1]
  display

.bin_ctrl_x_b0_q0_x_b1_q1
  c-x b[0],q[0]
  c-x b[1],q[1]
  display

.bin_ctrl_cnot_b0b1_q0_q2
  c-cnot b[0:1],q[0],q[2]
  display

.bin_ctrl_toffoli_b0b1_q0q1q2
  measure q[2]
  c-toffoli b[0:1],q[0],q[1],q[2]
  display

.bin_ctrl_rx_b0_q0_pi
  c-rx b[0],q[0],3.141592653589793
  display
