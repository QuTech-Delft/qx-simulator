
# load state test
qubits 3

.default_state
  display

.default_state
  h q0
  h q2
  display

.load_an_arbitrary_state
  load_state "initial.state"
  display


