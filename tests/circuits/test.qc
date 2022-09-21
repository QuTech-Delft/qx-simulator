version 1.0

# qcode test file

# qubit definition
qubits 8 

.first_circuit
   h q[0]
   cnot q[0],q[1]
   x  q[1]
   y  q[2]
   z  q[3]

.second_circuit
   rx q[4],0.453
   ry q[5],0.244
   rz q[6],0.555
   # ??? ph q[7]

.result
# measurement
   measure q[1]

# noise
# noise 0.001334,0.0000344

# decoherence
# decoherence 0.0001

# error correction code
# qec steans_three_qcode

