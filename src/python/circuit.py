#!/usr/bin/python


# abstract interface
class Gate:
	def __init__(self):
		return
	
	# internal communication protocol
	def quantum_code(self):
		return

# Hadamard
class Hadamard(Gate):

	def __init__(self, qubit):
		self.qubit = qubit

	def quantum_code(self): 
		qc = "h "+str(self.qubit)
		# print qc
		return qc

# CNOT    
class CNOT(Gate):

	def __init__(self, control_qubit, target_qubit):
		self.control_qubit = control_qubit
		self.target_qubit  = target_qubit

	def quantum_code(self): 
		qc = "cnot "+str(self.control_qubit)+","+str(self.target_qubit)
		# print qc
		return qc


# Toffoli 
class Toffoli(Gate):

	def __init__(self, control_qubit_1, control_qubit_2, target_qubit):
		self.control_qubit_1 = control_qubit_1
		self.control_qubit_2 = control_qubit_2
		self.target_qubit    = target_qubit

	def quantum_code(self): 
		qc = "toffoli "+str(self.control_qubit_1)+","+str(self.control_qubit_2)+","+str(self.target_qubit)
		# print qc
		return qc

# Measure
class Measure(Gate):

	def __init__(self, qubit):
		self.qubit = qubit

	def quantum_code(self): 
		qc = "measure "+str(self.qubit)
		# print qc
		return qc



# Circuit
class Circuit():
	def __init__(self, qubits, name="default"):
		self.name   = name
		self.qubits = qubits
		self.gates  = []
		# self.description = "This shape has not been described yeti"
		# self.author = "Nobody has claimed to make this shape yet"

	def dump(self):
		print "[-] circuit '", self.name,"' (", self.qubits," qubits) :"
		for i in range(len(self.gates)):
			print "  |-- ",self.gates[i].quantum_code()
	
	def add(self,gate):
		self.gates.append(gate)

	def execute(self,register):
		print "[+] executing circuit..."
	

#
# example:
#

c = Circuit(5, "test_circuit")
c.add(Hadamard(3))
c.add(CNOT(2,5))
c.add(Toffoli(1,2,3))
c.add(Hadamard(5))
c.dump()
