import unittest
import os

def test_basic():
    import qxelarator

    qx = qxelarator.QX()

    qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'basic.qasm'))
    qx.execute()

if __name__ == '__main__':
    test_basic()
