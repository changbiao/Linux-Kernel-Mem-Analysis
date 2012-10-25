import sys
import signal

dict = {};


class Mem_Info:
	def __init__(self, pid, address, bit0, bit1, bit2, bit3, bit4):
		self.pid = pid
		self.address = address
		self.bit0 = bit0
		self.bit1 = bit1
		self.bit2 = bit2
		self.bit3 = bit3
		self.bit4 = bit4

	def __repr__(self):
		s = ""
		s+= str(self.pid) + " "
		s+= str(self.address) + " "
		s+= str(self.bit0) + " "
		s+= str(self.bit1) + " " 
		s+= str(self.bit2) + " " 
		s+= str(self.bit3) + " " 
		s+= str(self.bit4) + " " 
		return s

def display(SIG, FRM):
	global dict
	write_tmp(dict)

def write_tmp(obj):
	f = open("tmp", "w")
	f.write(str(obj) + "\n")
	f.close()

def submit(line):
	try:
		global dict
		parts = line.split()
		record = parts[1]
		pid = parts[3]
		address = parts[5]
		bit0 = parts[7]
		bit1 = parts[9]
		bit2 = parts[11]
		bit3 = parts[13]
		bit4 = parts[15]
		mem_info =  Mem_Info(pid, address, bit0, bit1, bit2, bit3, bit4)
		dict[record] = mem_info
	except:
		pass
	

if __name__ == "__main__":
	signal.signal(signal.SIGHUP, display)
	while True:
		try:
			line = raw_input()
			submit(line)
		except:
			pass
		
