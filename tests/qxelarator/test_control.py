import unittest
import os

def test_control():
    import qxelarator

    qx = qxelarator.QX()

    qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'control.qasm'))

    qx.execute()

    c0 = qx.get_measurement_outcome(0)
    c1 = qx.get_measurement_outcome(1)
    c2 = qx.get_measurement_outcome(2)
    c3 = qx.get_measurement_outcome(3)
    c4 = qx.get_measurement_outcome(4)
    c5 = qx.get_measurement_outcome(5)


    print('{} {} {} {} {} {}'.format(c0,c1,c2,c3,c4,c5))

if __name__ == '__main__':
    test_control()
