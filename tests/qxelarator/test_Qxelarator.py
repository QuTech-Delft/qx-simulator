import unittest
import os
import qxelarator

class QxelaratorTest(unittest.TestCase):
    def test_oldAPI(self):
        qx = qxelarator.QX()

        cQasmFileName = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'bell_pair.qasm')
        qx.set(cQasmFileName)
        simulationResult = qx.execute(23)

        expected = \
"""{
    "info": {
        "shots_requested": 23,
        "shots_done": 23
    },
    "results": {
        "00": 23
    },
    "state": {
        "11": {
            "real": 1.00000000,
            "imag": 0.00000000,
            "norm": 1.00000000
        }
    }
}"""
        self.assertEqual(simulationResult, expected)

    def test_execute_file(self):
        cQasmFileName = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'bell_pair.qasm')
        simulationResult = qxelarator.execute_file(cQasmFileName, iterations = 23)

        self.assertIsInstance(simulationResult, qxelarator.SimulationResult)
        self.assertEqual(simulationResult.shots_requested, 23)
        self.assertEqual(simulationResult.shots_done, 23)
        self.assertEqual(simulationResult.results, {"00": 23})
        self.assertEqual(simulationResult.state, {"11": complex(1., 0.)})

    def test_execute_string_fails_returns_none(self):
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
        self.assertEqual(simulationError.message, "Simulation failed!")

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
