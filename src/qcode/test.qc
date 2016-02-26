# qcode test file

# qubit definition
qubits 8 

.first_circuit
   h q0
   cnot q0,q1
   x  q1
   y  q2
   z  q3

.second_circuit
   rx q4,0.453
   ry q5,0.244
   rz q6,0.555
   ph q7

.result
# measurement
   measure q1

# noise
noise 0.001334,0.0000344

# decoherence
decoherence 0.0001

# error correction code
qec steans_three_qcode

