version 1.0

# file   : rotation_rz.qc
# author : Nader Khammassi
# brief  : z rotations test 

qubits 1

 
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

