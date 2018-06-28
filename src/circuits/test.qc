# file   : epr.qc
# author : Nader Khammassi
# brief  : random test circuit
#          note: configuration directives not implementaed yet.

# qubit definition
qubits 4

# circuit 1
.first_circuit
   h q0
   cnot q0,q1
   x  q1
   y  q2
   z  q3
   display

# circuit 2
.second_circuit
   rx q0,0.453
   ry q1,0.244
   rz q2,0.555
   ph q3
   display

# measurement
.measurement
   measure q0
   measure q1
   measure q2
   measure q3
   display_binary

# operational noise (not implemented yet)
noise 0.001334,0.0000344

# decoherence (not implemented yet)
decoherence 0.0001

# error correction code (not implemented yet)
qec steans_three_qcode

