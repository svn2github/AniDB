#!/usr/bin/python

import sys
from datetime import date

username = ''
stats = []
state = 0
viewdate = ''

for line in sys.stdin.readlines():
	if state == 0:
		if username == '' and line.strip() != '':
			username = line.strip()
			state = 1
	elif state == 1:
		#looking for [
		if line.strip() == '[':
			state = 2
	elif state == 2:
		#looking for date
		if line.strip() != '':
			if line.strip() == '-':
				viewdate = 1;
			else:
				l = line.strip()
				# dates are returned as 23.09.2007 23:22
				viewdate = date(
					int(l[6:10]),
					int(l[3:5]),
					int(l[0:2])
				).toordinal()
			state = 3
	elif state == 3:
		#looking for length
		if line.strip() != '':
			# make some damned sparce arrays already -_-
			while len(stats) <= viewdate:
				stats.append(0)
			
			stats[viewdate - 1] += int(line.strip())
			state = 1
	else:
		sys.stderr.write("how did we end in state " + str(state) + "?\n")

filename = "time-wasted-" + username + ".data"
datafile = open(filename, "w")
accum = 0
startoutput = 0
for index, secs in enumerate(stats):
	if secs > 0:
		accum += secs
		if index != 0:
			startoutput = 1
	if startoutput:
		datafile.write(str(date.fromordinal(index + 1)))
		datafile.write("\t" + str(round(accum/3600)) + "\n")
datafile.close()

print username
