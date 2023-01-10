import unittest
import os

def test_control():
    import qxelarator

    qx = qxelarator.QX()

    qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'control.qasm'))

    qx.execute()

if __name__ == '__main__':
    test_control()
