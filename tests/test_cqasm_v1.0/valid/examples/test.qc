version 1.0
qubits 8

map q[0,0:7,5,6], bleh2
map b[5:7], bleh


prep_z q[5]
reset-AveraGing
reset-Averaging Q[1,4,6]


measure_all
measure_parity     q[2:5], x,     q[0], z

x q[4:7]

rX q[1,2:5], 3.1416E-01


.mapping
   map b[0:7], bitcontroller

.waits
   wait 2

.first_circuit(50)
   H q[0]
   cnot q[0],q[1]
   x  q[1]
   Y  q[2]
   z  q[6]
   {x q[3] | h q[7]| c-x bitcontroller, bleh2| c-toffoli b[4], q[3], q[4], q[5]}

.second_circuit
   rx q[4],     -0.453e1
   ry q[5],0.244
   rz q[6],0.555
   c-x bitcontroller, q[2:5]
   c-swap b[7], q[1] , q[5,7]
   toffoli q[3], q[4], q[0:5]

.result
# measurement
   measure q[1]
   {measure q[0] | measure q[2] | measure q[3]}

.display_results
display
display b[1]
display bleh