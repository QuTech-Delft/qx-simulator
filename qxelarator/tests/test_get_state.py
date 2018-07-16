import qxelarator

qx = qxelarator.QX()

qx.set('basic.qasm')
qx.execute()

print( qx.get_state() )
