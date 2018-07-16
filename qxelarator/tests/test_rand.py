import qxelarator

qx = qxelarator.QX()
qx.set('rand.qasm')

NTests = 1000
true_counter = 0
for i in range(NTests):
    qx.execute()
    res = qx.get_measurement_outcome(0)
    true_counter = true_counter + res

print( 'True percentage = {}'.format(true_counter/NTests*100) )
