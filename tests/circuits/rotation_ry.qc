version 1.0

# file   : rotation_ry.qc
# author : Nader Khammassi
# brief  : y rotations test 

qubits 1

 .y_pi_rotation
   prep_z q[0]
   ry q[0],3.141592653589793
   display

 .pauli_y
   prep_z q[0]
   y q[0]
   display
 
 .measurement
  measure q[0]
  display

