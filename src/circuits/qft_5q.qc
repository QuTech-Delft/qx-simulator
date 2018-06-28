# file   : qft_5q.qc
# author : Nader Khammassi
# brief  : 5 qubits quantum fourrier transform (QFT)

# define 5 qubits
qubits 5

# init qubits to |0...01> (set your own input state here)
.init
   x q0
   x q2
   x q3
   display_binary  # display the input binary state 

# main circuit
.qft
   h q0
   cr q1,q0
   cr q2,q0
   cr q3,q0
   cr q4,q0
   h q1
   cr q2,q1
   cr q3,q1
   cr q4,q1
   h q2
   cr q3,q2
   cr q4,q2
   h q3
   cr q4,q3
   h q4

# reverse qubit order
.reverse
   swap q0,q4
   swap q1,q3
   display  # display the output quantum state


# to activate error injection, uncomment the next line
# error_model depolarizing_channel,0.001
