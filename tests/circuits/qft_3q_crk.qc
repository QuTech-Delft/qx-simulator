version 1.0

# file   : qft_3q_crk.qc
# author : Nader Khammassi
# brief  : 3 qubits quantum fourrier transform (QFT) using 
#          the CRK notation
#          input state is |q2.q1.q0> = |100>

qubits 4

# input = |100>
.input
  x q[2]
  display

# qft circuit
.qft

  h q[0]
  crk q[1],q[0],2 
  crk q[2],q[0],3
  
  h q[1]
  crk q[2],q[1],2
  
  h q[2]

# reverse qubit order
.reverse
  swap q[0],q[2]
  display

