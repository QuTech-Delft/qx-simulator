version 1.0

qubits 1


# prep_z q[0]

.rb(100)
 prep_z q[0]
 x q[0]
 h q[0]
 y q[0]
 x q[0]
 x q[0]
 y q[0]
 h q[0]
 x q[0]
 x q[0]
 h q[0]
 y q[0]
 x q[0]
 x q[0]
 y q[0]
 h q[0]
 x q[0]
 x q[0]
 h q[0]
 y q[0]
 x q[0]
 x q[0]
 y q[0]
 h q[0]
 x q[0]
 measure q[0]

.result
 
 display

error_model depolarizing_channel,0.001
