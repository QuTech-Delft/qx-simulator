import unittest
import os

def test_ch():
    import qxelarator

    qx = qxelarator.QX()

    qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'ch.qasm'))
    qx.execute()

    c0 = qx.get_measurement_outcome(0)
    c1 = qx.get_measurement_outcome(1)

    print('{} {}'.format(c0,c1))
