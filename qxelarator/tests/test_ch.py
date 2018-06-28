import qxelarator

qx = qxelarator.QX()
qx.set('ch.qasm')

NTests = 1000
true_counter = 0
for i in range(NTests):
	qx.execute()
	true_counter += qx.get_measurement_outcome(1)

qx.execute()
c0 = qx.get_measurement_outcome(0)
c1 = qx.get_measurement_outcome(1)
print('{} {}'.format(c0,c1))

print( 'True percentage = {}'.format(true_counter/NTests*100) )
