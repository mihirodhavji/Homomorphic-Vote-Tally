import os
import sys

f1 = open("Ballot/ballot4.txt", "r")
f2 = open("Ballot/ballot7.txt", "w")

for line in f1:
    line_list = line.split()
    f2.write(line_list[0])
    f2.write(" ")
    f2.write(line_list[1])
    f2.write("\n")
    f2.write(line_list[2])
    f2.write("\n")
f1.close()
f2.close()