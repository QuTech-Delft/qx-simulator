# interface.py  
import socket


"""
class QXClient():
	def __init__(self, address="localhost", port="5555"):
		self.address   = address
		self.port      = port
	

	def connect(self):
		print "[-] circuit '", self.name,"' (", self.qubits," qubits) :"
		for i in range(len(self.gates)):
			print "  |-- ",self.gates[i].quantum_code()
	
	def add(self,gate):
		self.gates.append(gate)

	def execute(self,register):
		print "[+] executing circuit..."
"""	

# create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

# get local machine name
host = socket.gethostname()                           

port = 5555

# connection to hostname on the port.
s.connect((host, port))                               

# Receive no more than 1024 bytes
tm = s.recv(1024)                                     

s.close()

print("The time got from the server is %s" % tm.decode('ascii'))
