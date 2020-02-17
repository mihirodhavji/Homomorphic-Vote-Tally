import os
import sys

f1 = open("Ballot/ballot2.txt", "r")
f2 = open("Ballot/ballot3.txt", "w")
dici = {}
for line in f1:
    line_list = line.split()
    dici[line_list[0]]={}
    dici[line_list[0]]['line']=line

for key in dici:
    f2.write(dici[key]['line'])

f1.close()
f2.close()