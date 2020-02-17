import os
import sys

f1 = open("Ballot/ballot3.txt", "r")
f2 = open("Ballot/ballot4.txt", "w")

for line in f1:
    line_list = line.split() 
    for i in range(2,len(line_list) - 2, 1):
        filename = line_list[i]
        filename_list = filename.split("_")
        c_id = ""
        for j in range(len(filename_list[2])):
            if ( filename_list[2][j] != 'v'):
                c_id += filename_list[2][j]
            else:
                break
        cid = int(c_id)
        f2.write(str(cid))
        f2.write(" ")
        f2.write(line_list[0])
        f2.write(" ")
        f2.write(filename)
        f2.write("\n")
f1.close()
f2.close()
    
