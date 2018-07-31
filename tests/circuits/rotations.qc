version 1.0

# file   : rotations.qc
# author : Nader Khammassi
# brief  : x/y/z rotations test 

qubits 1


 .x_pi_rotation
   prep_z q[0]
   rx q[0],3.141592653589793
   display
 
 .pauli_x
   prep_z q[0]
   x q[0]
   display

 
 .y_pi_rotation
   prep_z q[0]
   ry q[0],3.141592653589793
   display

 .pauli_y
   prep_z q[0]
   y q[0]
   display
 
 .z_pi_rotation
   prep_z q[0]
   x q[0]
   rz q[0],3.141592653589793
   display

 .pauli_z
   prep_z q[0]
   z q[0]
   x q[0]
   z q[0]
   display
 

 .measurement
  measure q[0]
  display

