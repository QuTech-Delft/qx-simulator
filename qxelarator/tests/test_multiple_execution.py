import qxelarator

qx = qxelarator.QX()

qx.set('basic.qasm')

shots = 100

for i in range(shots):
	qx.execute()
	c0 = qx.get_measurement_outcome(0)
	c1 = qx.get_measurement_outcome(1)
	print('{} {}'.format(c0,c1) )

print('quantum state: \n'+qx.get_state())
