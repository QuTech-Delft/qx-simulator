version 1.0

# file   : qft_5q.qc
# author : Nader Khammassi
# brief  : 5 qubits quantum fourrier transform (QFT)

# define 5 qubits
qubits 5

# init qubits to |0...01> (set your own input state here)
.init
#   x q[0]
#   x q[2]
#   x q[3]
#   display_binary  # display the input binary state 

# main circuit
.qft
   h q[0]
   cr q[1],q[0]
   cr q[2],q[0]
   cr q[3],q[0]
   cr q[4],q[0]
   h q[1]
   cr q[2],q[1]
   cr q[3],q[1]
   cr q[4],q[1]
   h q[2]
   cr q[3],q[2]
   cr q[4],q[2]
   h q[3]
   cr q[4],q[3]
   h q[4]

# reverse qubit order
.reverse
#   swap q[0],q[4]
#   swap q[1],q[3]
   display  # display the output quantum state


# to activate error injection, uncomment the next line
# error_model depolarizing_channel,0.001
