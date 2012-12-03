import pickle
import re
import sys

def readDataObj(filename):
	f = open(filename, "r")
	d = pickle.load(f)
	f.close()
	return d

if __name__ == "__main__":
	#load in the dumped dictionary object
	full = readDataObj("tmp")
	
	#pass in search key and value as command line args
	search_key = sys.argv[1]
	search_val = sys.argv[2]

	#perform search
	for key, val in full.iteritems():
		if re.search(search_val, val[search_key]):
			print key,
			print val
		
	
