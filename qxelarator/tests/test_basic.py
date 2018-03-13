import qxelarator

qx = qxelarator.QX()

qx.set('basic.qasm')
qx.execute()
res = qx.get_measurement_outcome(0)

if(res):
    print("True")
else:
    print("False")

