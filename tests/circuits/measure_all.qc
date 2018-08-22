version 1.0

# file   : measure_all.qc
# author : Nader Khammassi
# brief  : measure_all test 

qubits 4

 .measurement(1000)
   prep_z q[0,1,2,3]
   x90 q[0]
   h q[1]
   x q[2]
   measure_all

.result
   display

