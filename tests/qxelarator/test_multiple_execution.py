import unittest
import os

@unittest.skip('breaks CI all the time; causes sporadic problem that needs to be fixed')
def test_multiple_execution():
    import qxelarator

    qx = qxelarator.QX()

    qx.set(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'basic.qasm'))

    shots = 10

    for i in range(shots):
        print('shot {}'.format(i))
        qx.execute()

if __name__ == '__main__':
    test_multiple_execution()
