import os
import qxelarator
import unittest


class QxelaratorTest(unittest.TestCase):
    def test_execute_file(self):
        cqasm_file_name = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'bell_pair.cq')
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
bit b

X q,
b = measure q
"""
        simulation_error = qxelarator.execute_string(cqasm_string, iterations=2)
        print(simulation_error)

        self.assertIsInstance(simulation_error, qxelarator.SimulationError)
        self.assertTrue(simulation_error.message.startswith("""\
cQASM v3 analyzer returned errors:
Error at <unknown file name>:6:5..6: extraneous input '\\n'\
"""))

    def test_execute_string(self):
        cqasm_string = """\
version 3.0

qubit q
bit b

X q
b = measure q
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
bit b

H q
b = measure q
"""
        simulation_result = qxelarator.execute_string(cqasm_string, iterations=20, seed=123)
        self.assertEqual(simulation_result.results, {"0": 12, "1": 8})

    def test_rn_gate(self):
        cqasm_string = """\
version 3.0

qubit q
bit b

Rn(1,0,0,pi,pi/2) q
b = measure q
"""
        simulation_result = qxelarator.execute_string(cqasm_string, iterations=2)

        self.assertIsInstance(simulation_result, qxelarator.SimulationResult)
        self.assertEqual(simulation_result.shots_requested, 2)
        self.assertEqual(simulation_result.shots_done, 2)
        self.assertEqual(simulation_result.results, {"1": 2})
        self.assertAlmostEqual(simulation_result.state["1"], complex(1., 0.), places=15)

    def test_parameterized_measure_is_not_supported(self):
        cqasm_string = """\
version 3.0

qubit q
bit b

b = measure(0, 1, 0) q
"""
        simulation_error = qxelarator.execute_string(cqasm_string, iterations=1)

        self.assertIsInstance(simulation_error, qxelarator.SimulationError)
        self.assertEqual(simulation_error.message,
            "parameterized measure instructions are not supported")

    def test_measure_z_is_alias_of_measure(self):
        cqasm_string = """\
version 3.0

qubit q
bit b

X q
b = measureZ q
"""
        simulation_result = qxelarator.execute_string(cqasm_string, iterations=1)

        self.assertIsInstance(simulation_result, qxelarator.SimulationResult)
        self.assertEqual(simulation_result.results, {"1": 1})

    def test_measure_x_and_y_are_not_supported(self):
        for instruction_name in ("measureX", "measureY"):
            with self.subTest(instruction_name=instruction_name):
                cqasm_string = f"""\
version 3.0

qubit q
bit b

b = {instruction_name} q
"""
                simulation_error = qxelarator.execute_string(cqasm_string, iterations=1)

                self.assertIsInstance(simulation_error, qxelarator.SimulationError)
                self.assertEqual(simulation_error.message,
                    f"measurement instruction '{instruction_name}' is not supported")

    def test_instructions(self):
        cqasm_string = """\
version 3.0

qubit[3] q
bit b

I q[0]
X q[0]
Y q[0]
Z q[0]
H q[0]
S q[0]
Sdag q[0]
T q[0]
Tdag q[0]
X90 q[0]
mX90 q[0]
Y90 q[0]
mY90 q[0]
Z90 q[0]
mZ90 q[0]
Rx(pi/2) q[0]
Ry(pi/2) q[0]
Rz(pi/2) q[0]
Rn(1,0,0,pi,pi/2) q[0]
U(pi/2,pi/3,pi/4) q[0]

CNOT q[0], q[1]
CR(pi/2) q[0], q[1]
CRk(2) q[0], q[1]
CZ q[0], q[1]
SWAP q[0], q[1]
CCX q[0], q[1], q[2]
CCNOT q[0], q[1], q[2]
CSWAP q[0], q[1], q[2]
CV q[0], q[1]
CY q[0], q[1]
DCNOT q[0], q[1]
ECR q[0], q[1]
InvSqrtSWAP q[0], q[1]
ISWAP q[0], q[1]
M q[0], q[1]
MS q[0], q[1]
SqrtISWAP q[0], q[1]
SqrtSWAP q[0], q[1]

asm(other_backend) '''backend-specific code'''
barrier q
wait(5) q


b = measure q[0]
reset q[0]
    """
        simulation_result = qxelarator.execute_string(cqasm_string, iterations=1)

        self.assertIsInstance(simulation_result, qxelarator.SimulationResult)
        self.assertEqual(simulation_result.shots_requested, 1)
        self.assertEqual(simulation_result.shots_done, 1)
        self.assertTrue(simulation_result.state)

    def test_qx_backend_asm_is_not_supported(self):
        cqasm_string = """\
version 3.0

qubit q

asm(qx_simulator) '''qx-simulator specific code'''
"""
        simulation_error = qxelarator.execute_string(cqasm_string, iterations=1)

        self.assertIsInstance(simulation_error, qxelarator.SimulationError)
        self.assertEqual(simulation_error.message,
            "asm declarations for backend 'qx_simulator' are not supported at the moment")

if __name__ == '__main__':
    unittest.main()
