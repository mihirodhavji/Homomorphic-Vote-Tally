import os
import sys

f1 = open("Ballot/ballot1.txt", "r")
f2 = open("Ballot/ballot2.txt", "w")

for line in f1:
    f3 = open("temp.txt","w")
    line_list = line.split()
    sign = ""
    for i in range(len(line_list) - 2):
        sign += line_list[i]
        sign += " "
    f3.write(sign)
    cmd = "cat temp.txt "
    for i in range(2,len(line_list) - 2, 1):
        cmd += "Ballot/"
        cmd += line_list[i]
        cmd += " "
    cmd += "> sign.txt"
    f3.close()
    os.system(cmd)
    cmd = "cp Ballot/" + line_list[len(line_list) - 2] + " ."
    os.system(cmd)
    cmd = "cp Ballot/Voter" + line_list[0] + ".crt ."
    os.system(cmd)
    cmd = "openssl dgst -sha256 -verify  <(openssl x509 -in Voter" + line_list[0] + ".crt -pubkey -noout) -signature "+ line_list[len(line_list) - 2] + " sign.txt > assinatura.txt"
    f4 = open("verifica","w")
    f4.write("#!/bin/bash\n")
    f4.write(cmd)
    f4.close()
    os.system("sudo chmod 0755 verifica")
    os.system("./verifica")
    f4 = open("assinatura.txt","r")
    for linha in f4:
        if linha == "Verified OK\n":
            f2.write(line)
    f4.close()
#os.system("rm assinatura.txt")
#os.system("rm sign.txt")
#os.system("rm verifica")
f1.close()
f2.close()
    
