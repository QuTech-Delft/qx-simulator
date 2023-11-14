import unittest
import os
import qxelarator

class QxelaratorTest(unittest.TestCase):
    def test_execute_file(self):
        cQasmFileName = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'bell_pair.qasm')
        simulationResult = qxelarator.execute_file(cQasmFileName, iterations = 23)

        self.assertIsInstance(simulationResult, qxelarator.SimulationResult)
        self.assertEqual(simulationResult.shots_requested, 23)
        self.assertEqual(simulationResult.shots_done, 23)
        self.assertEqual(simulationResult.results, {"00": 23})
        self.assertEqual(simulationResult.state, {"11": complex(1., 0.)})

    def test_execute_string_fails_returns_simulation_error(self):
        cQasmString = \
"""
version 1.0

qubits 1

.myCircuit
    x q[0],
    measure q[0]
"""
        simulationError = qxelarator.execute_string(cQasmString, iterations = 2)

        self.assertIsInstance(simulationError, qxelarator.SimulationError)
        self.assertEqual(simulationError.message, """\
Cannot parse and analyze cQASM v1: 
<unknown>:7:12: syntax error, unexpected NEWLINE
""")

    def test_execute_string(self):
        cQasmString = \
"""
version 1.0

qubits 1

.myCircuit
    x q[0]
    measure q[0]
"""
        simulationResult = qxelarator.execute_string(cQasmString, iterations = 2)

        self.assertIsInstance(simulationResult, qxelarator.SimulationResult)
        self.assertEqual(simulationResult.shots_requested, 2)
        self.assertEqual(simulationResult.shots_done, 2)
        self.assertEqual(simulationResult.results, {"1": 2})
        self.assertEqual(simulationResult.state, {"1": complex(1., 0.)})
    
    def test_execute_string_v3(self):
        cQasmString = \
"""
version 3.0

qubit[1] q
bit[1] b

x q[0]
b[0] = measure q[0]
"""
        simulationResult = qxelarator.execute_string(cQasmString, iterations = 2, seed = 1, version = "3.0")

        self.assertIsInstance(simulationResult, qxelarator.SimulationResult)
        self.assertEqual(simulationResult.shots_requested, 2)
        self.assertEqual(simulationResult.shots_done, 2)
        self.assertEqual(simulationResult.results, {"1": 2})
        self.assertEqual(simulationResult.state, {"1": complex(1., 0.)})
    
    def test_seed_deterministic(self):
        cQasmString = \
"""
version 1.0

qubits 1

.myCircuit
    h q[0]
    measure q[0]
"""
        simulationResult = qxelarator.execute_string(cQasmString, iterations = 20, seed = 123)
        self.assertEqual(simulationResult.results, {"0": 12, "1": 8})

if __name__ == '__main__':
    unittest.main()
