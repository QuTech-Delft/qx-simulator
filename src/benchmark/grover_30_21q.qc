qubits 21

.init
   x q10
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
   h q10

# iterating 25 times
.grover(25)

   # oracle_30

   x q0
   x q5
   x q6
   x q7
   x q8
   x q9
   toffoli q0,q1,q11
   toffoli q1,q11,q12
   toffoli q2,q12,q13
   toffoli q3,q13,q14
   toffoli q4,q14,q15
   toffoli q5,q15,q16
   toffoli q6,q16,q17
   toffoli q7,q17,q18
   toffoli q8,q18,q19
   toffoli q9,q19,q20
   cnot q20,q10
   toffoli q9,q19,q20
   toffoli q8,q18,q19
   toffoli q7,q17,q18
   toffoli q6,q16,q17
   toffoli q5,q15,q16
   toffoli q4,q14,q15
   toffoli q3,q13,q14
   toffoli q2,q12,q13
   toffoli q1,q11,q12
   toffoli q0,q1,q11
   x q0
   x q5
   x q6
   x q7
   x q8
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
   x q9
   h q9
   toffoli q0,q1,q11
   toffoli q1,q11,q12
   toffoli q2,q12,q13
   toffoli q3,q13,q14
   toffoli q4,q14,q15
   toffoli q5,q15,q16
   toffoli q6,q16,q17
   toffoli q7,q17,q18
   toffoli q8,q18,q19
   cnot q19,q9
   toffoli q8,q18,q19
   toffoli q7,q17,q18
   toffoli q6,q16,q17
   toffoli q5,q15,q16
   toffoli q4,q14,q15
   toffoli q3,q13,q14
   toffoli q2,q12,q13
   toffoli q1,q11,q12
   toffoli q0,q1,q11
   h q9
   x q0
   x q1
   x q2
   x q3
   x q4
   x q5
   x q6
   x q7
   x q8
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

.measure
   h q10
   measure
