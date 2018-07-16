qubits 19

.init
   x q9
   h q0
   h q1
   h q2
   h q3
   h q4
   h q5
   h q6
   h q7
   h q8
   h q9

# iterating 17 times
.grover(17)

   # oracle_64

   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q7
   x q8
   toffoli q0,q1,q10
   toffoli q1,q10,q11
   toffoli q2,q11,q12
   toffoli q3,q12,q13
   toffoli q4,q13,q14
   toffoli q5,q14,q15
   toffoli q6,q15,q16
   toffoli q7,q16,q17
   toffoli q8,q17,q18
   cnot q18,q9
   toffoli q8,q17,q18
   toffoli q7,q16,q17
   toffoli q6,q15,q16
   toffoli q5,q14,q15
   toffoli q4,q13,q14
   toffoli q3,q12,q13
   toffoli q2,q11,q12
   toffoli q1,q10,q11
   toffoli q0,q1,q10
   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q7
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

   # inversion

   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q6
   x q7
   x q8
   h q8
   toffoli q0,q1,q10
   toffoli q1,q10,q11
   toffoli q2,q11,q12
   toffoli q3,q12,q13
   toffoli q4,q13,q14
   toffoli q5,q14,q15
   toffoli q6,q15,q16
   toffoli q7,q16,q17
   cnot q17,q8
   toffoli q7,q16,q17
   toffoli q6,q15,q16
   toffoli q5,q14,q15
   toffoli q4,q13,q14
   toffoli q3,q12,q13
   toffoli q2,q11,q12
   toffoli q1,q10,q11
   toffoli q0,q1,q10
   h q8
   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q6
   x q7
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

.measure
   h q9
   measure q9
   display_binary
