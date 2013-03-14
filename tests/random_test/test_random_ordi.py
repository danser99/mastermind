#!/usr/bin/env python

from random import randrange as rr

a=[]
i=0

while i<1000:
	i+=1
	a.append((rr(1,7), rr(1,7), rr(1,7)))
	

multiple = []

for t in a:
	cnt = a.count(t)
	i=0
	if cnt>1:
		multiple.append((t,cnt))
	while i<cnt:
		a.remove(t)
		i+=1

for m in multiple:
	print("{}: {} fois /1000".format(m[0], m[1]))
	
