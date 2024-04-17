import os
import qxelarator
import unittest


class QxelaratorTest(unittest.TestCase):
    def test_execute_file(self):
        cqasm_file_name = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'bell_pair.qasm')
        simulation_result = qxelarator.execute_file(cqasm_file_name, iterations=23)

        self.assertIsInstance(simulation_result, qxelarator.SimulationResult)
        self.assertEqual(simulation_result.shots_requested, 23)
        self.assertEqual(simulation_result.shots_done, 23)
        self.assertEqual(simulation_result.results, {"00": 23})
        self.assertEqual(simulation_result.state, {"11": complex(1., 0.)})

    def test_execute_string_fails_returns_simulation_error(self):
        cqasm_string = """\
version 3.0

qubit q

X q,
measure q
"""
        simulation_error = qxelarator.execute_string(cqasm_string, iterations=2)

        self.assertIsInstance(simulation_error, qxelarator.SimulationError)
        self.assertTrue(simulation_error.message.startswith("""\
Cannot parse and analyze cQASM v3:
Error at <unknown file name>:5:5..6: mismatched input '\\n'\
"""))

    def test_execute_string(self):
        cqasm_string = """\
version 3.0

qubit q

X q
measure q
"""
        simulation_result = qxelarator.execute_string(cqasm_string, iterations=2)

        self.assertIsInstance(simulation_result, qxelarator.SimulationResult)
        self.assertEqual(simulation_result.shots_requested, 2)
        self.assertEqual(simulation_result.shots_done, 2)
        self.assertEqual(simulation_result.results, {"1": 2})
        self.assertEqual(simulation_result.state, {"1": complex(1., 0.)})

    def test_seed_deterministic(self):
        cqasm_string = """\
version 3.0

qubit q

H q
measure q
"""
        simulation_result = qxelarator.execute_string(cqasm_string, iterations=20, seed=123)
        self.assertEqual(simulation_result.results, {"0": 12, "1": 8})


if __name__ == '__main__':
    unittest.main()
