qubits 17

.init
   x q8
   h q0
   h q1
   h q2
   h q3
   h q4
   h q5
   h q6
   h q7
   h q8

# iterating 12 times
.grover(12)

   # oracle_64

   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q7
   toffoli q0,q1,q9
   toffoli q1,q9,q10
   toffoli q2,q10,q11
   toffoli q3,q11,q12
   toffoli q4,q12,q13
   toffoli q5,q13,q14
   toffoli q6,q14,q15
   toffoli q7,q15,q16
   cnot q16,q8
   toffoli q7,q15,q16
   toffoli q6,q14,q15
   toffoli q5,q13,q14
   toffoli q4,q12,q13
   toffoli q3,q11,q12
   toffoli q2,q10,q11
   toffoli q1,q9,q10
   toffoli q0,q1,q9
   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q7
   h q0
   h q1
   h q2
   h q3
   h q4
   h q5
   h q6
   h q7

   # inversion

   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q6
   x q7
   h q7
   toffoli q0,q1,q9
   toffoli q1,q9,q10
   toffoli q2,q10,q11
   toffoli q3,q11,q12
   toffoli q4,q12,q13
   toffoli q5,q13,q14
   toffoli q6,q14,q15
   cnot q15,q7
   toffoli q6,q14,q15
   toffoli q5,q13,q14
   toffoli q4,q12,q13
   toffoli q3,q11,q12
   toffoli q2,q10,q11
   toffoli q1,q9,q10
   toffoli q0,q1,q9
   h q7
   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q6
   x q7
   h q0
   h q1
   h q2
   h q3
   h q4
   h q5
   h q6
   h q7

.measure
   h q8
   measure
   display_binary

