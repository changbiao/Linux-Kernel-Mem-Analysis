import sys
import re
import signal
import time
import pickle


full = {}

def display(SIG, FRM):
	global dict
	write_tmp(full)

def write_tmp(obj):
	f = open("tmp", "w")
	pickle.dump(obj, f)
	f.close()


def read(filename):
	ret = []
	f=open(filename)
	lines = f.readlines()
	for line in lines:
		ret.append(line.strip())
	f.close()
	return ret

def parse(lines):
	for line in lines:
		parts = line.split()
		key = parts[1]
		if key in full:
			continue
		full[key] = {}
		for i in range(2, len(parts)):
			if is_even(i):
				key_i = clean(parts[i])
				val_i = clean(parts[i+1])
#				print key_i,
#				print "=",
#				print val_i
				full[key][key_i] = val_i

def is_even(num):
	return num % 2 == 0


def clean(s):
	s=re.sub(":", "", s)
	s=re.sub(",", "", s)
	return s


if __name__ == "__main__":
	signal.signal(signal.SIGHUP, display)
	while True:
		lines = read("/proc/mem_hurst")	
		parse(lines)

