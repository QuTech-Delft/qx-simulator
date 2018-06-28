import qxelarator

qx = qxelarator.QX()

qx.set('control.qasm')

qx.execute()

c0 = qx.get_measurement_outcome(0)
c1 = qx.get_measurement_outcome(1)
c2 = qx.get_measurement_outcome(2)
c3 = qx.get_measurement_outcome(3)
c4 = qx.get_measurement_outcome(4)
c5 = qx.get_measurement_outcome(5)


print('{} {} {} {} {} {}'.format(c0,c1,c2,c3,c4,c5))
