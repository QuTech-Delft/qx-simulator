version 1.0

# file   : rotation_rx.qc
# author : Nader Khammassi
# brief  : x rotations test 

qubits 1


 .x_pi_rotation
   prep_z q[0]
   rx q[0],3.141592653589793
   display
 
 .pauli_x
   prep_z q[0]
   x q[0]
   display

 .measurement
  measure q[0]
  display

