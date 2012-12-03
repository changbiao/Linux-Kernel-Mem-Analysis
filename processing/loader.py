import pickle
import re
import sys

def read_tmp(filename):
	f = open(filename, "r")
	d = pickle.load(f)
	f.close()
	return d


	


if __name__ == "__main__":
	full = read_tmp("tmp")
	search_key = sys.argv[1]
	search_val = sys.argv[2]
	for key, val in full.iteritems():
		if re.search(search_val, val[search_key]):
			print key,
			print val
		
	
