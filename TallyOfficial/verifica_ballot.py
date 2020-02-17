import os
import sys

# 1 é numero de candidatos
os.system("cp my-ca.crt Ballot")
os.chdir("Ballot")
n_c = int(sys.argv[1])
arg = n_c + 4
f1 = open("ballot.txt", "r")
f2 = open("ballot1.txt", "w")
for line in f1:
    line_list = line.split()
    cmd1 = "openssl verify -verbose -CAfile my-ca.crt  " + line_list[-1] + " > temp.txt"
    os.system(cmd1)
    myfile = open("temp.txt","r")
    first_line = myfile.readline().rstrip()
    myfile.close()
    os.system("rm temp.txt")
    cmd1 = "" + line_list[-1] + ": OK"
    if (first_line != cmd1):
        print("Certificado errado.\n")
        continue    
    if (len(line_list) == arg):
        f2.write(line)

os.chdir("..") 