import unittest
import os

def test_basic():
    import qxelarator

    qx = qxelarator.QX()

    qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'basic.qasm'))
    qx.execute()

    c0 = qx.get_measurement_outcome(0)
    c1 = qx.get_measurement_outcome(1)

    print('{} {}'.format(c0,c1) )

    print('quantum state: \n'+qx.get_state())
