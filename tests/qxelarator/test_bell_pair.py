import unittest
import os
import qxelarator

class QxelaratorTest(unittest.TestCase):
    def test_basic(self):
        qx = qxelarator.QX()

        qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'bell_pair.qasm'))
        simulationResult = qx.execute(23)

        expected = """{
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

if __name__ == '__main__':
    unittest.main()
