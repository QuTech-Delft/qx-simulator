version 1.0

# file   : qft_8q.qc
# author : Nader Khammassi
# brief  : 8 qubits quantum fourrier transform (QFT)

# define 8 qubits
qubits 8

# init qubits to |0...01> (set your own input state here)
.init
   x q[0]
   x q[2]
   x q[4]
   x q[6]
   display_binary  # display the input binary state 

# main qft circuit
.qft
   h q[0]
   cr q[1],q[0]
   cr q[2],q[0]
   cr q[3],q[0]
   cr q[4],q[0]
   cr q[5],q[0]
   cr q[6],q[0]
   cr q[7],q[0]
   h q[1]
   cr q[2],q[1]
   cr q[3],q[1]
   cr q[4],q[1]
   cr q[5],q[1]
   cr q[6],q[1]
   cr q[7],q[1]
   h q[2]
   cr q[3],q[2]
   cr q[4],q[2]
   cr q[5],q[2]
   cr q[6],q[2]
   cr q[7],q[2]
   h q[3]
   cr q[4],q[3]
   cr q[5],q[3]
   cr q[6],q[3]
   cr q[7],q[3]
   h q[4]
   cr q[5],q[4]
   cr q[6],q[4]
   cr q[7],q[4]
   h q[5]
   cr q[6],q[5]
   cr q[7],q[5]
   h q[6]
   cr q[7],q[6]
   h q[7]

# reverse qubit order
.reverse
   swap q[0],q[7]
   swap q[1],q[6]
   swap q[2],q[5]
   swap q[3],q[4]

#   display


# to activate error injection, uncomment the next line
# error_model depolarizing_channel,0.001
