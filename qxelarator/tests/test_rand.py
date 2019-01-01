import qxelarator

qx = qxelarator.QX()
qx.set('rand.qasm')

NTests = 100
qx.execute(NTests)
res = qx.get_measurement_outcome(0)
