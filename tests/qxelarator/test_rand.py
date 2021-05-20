import unittest
import os

def test_rand():
    import qxelarator

    qx = qxelarator.QX()
    qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'rand.qasm'))

    NTests = 100
    qx.execute(NTests)
    res = qx.get_measurement_outcome(0)

if __name__ == '__main__':
    test_rand()
