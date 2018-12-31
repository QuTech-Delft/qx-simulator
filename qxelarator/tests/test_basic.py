import qxelarator

qx = qxelarator.QX()

qx.set('basic.qasm')
qx.execute()

c1 = qx.get_measurement_outcome(1)
c2 = qx.get_measurement_outcome(2)

print('{} {}'.format(c1,c2) )

print('quantum state: \n'+qx.get_state())
