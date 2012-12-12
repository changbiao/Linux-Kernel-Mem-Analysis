import sys
import re
import signal
import time
import pickle
import os

#full is the full dictionary of data stored in mem
full = {}

#search query file
searchFile = "search.dat"

def dumpData(SIG, FRM):
	global dict
	writeOutObj(full)

def writeOutObj(obj):
	f = open("tmp", "w")
	if hasQuerySpecified():
		obj = cleanObj(obj)
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


def hasQuerySpecified():
	#check if pid file exists
	if os.path.exists(searchFile):
		return True
	return False

def cleanObj(obj):
	#open pid file
	query = read(searchFile)[0]
	parts = query.split(":")
	search_key = parts[0]
	search_val = parts[1]

	#remove all objects that aren't related to pid
	objTmp = {}
	for key, val in obj.iteritems():
		if re.search(search_val, val[search_key]):
			objTmp[key] = val
	#return the new obj
	return objTmp

def parse(lines):
	for line in lines:
		parts = line.split()
		key = parts[1]
		if key in full:
			continue
		full[key] = {}
		for i in range(2, len(parts)):
			if isEven(i):
				key_i = clean(parts[i])
				val_i = clean(parts[i+1])
				full[key][key_i] = val_i

def isEven(num):
	return num % 2 == 0

def clean(s):
	s=re.sub(":", "", s)
	s=re.sub(",", "", s)
	return s


if __name__ == "__main__":
	#assign the HUP signal handler to dump
	signal.signal(signal.SIGHUP, dumpData)

	#indefinitely read the proc module and store data
	while True:
		lines = read("/proc/mem_hurst")	
		parse(lines)

