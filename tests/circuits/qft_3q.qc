version 1.0

# file   : qft_3q.qc
# author : Nader Khammassi
# brief  : 3 qubits quantum fourrier transform (QFT)
#          input state is |q2.q1.q0> = |100>

qubits 3

# input = |100>
.input
  x q[2]
  display

# qft circuit
.qft

  h q[0]
  cr q[1],q[0],1.5707963  # cr2
  cr q[2],q[0],0.7853982  # cr4
  
  h q[1]
  cr q[2],q[1],1.5707963  # cr2
  
  h q[2]

# reverse qubit order
.reverse
  swap q[0],q[2]
  display




