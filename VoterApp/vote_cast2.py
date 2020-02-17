import os
import sys

# 1 = id
# 2 é a frase do ballot
# 3 é id_date

os.chdir("..")
cmd = "cp Voter" + str(sys.argv[1]) +  "/"  + sys.argv[len(sys.argv) - 2]  + " TallyOfficial/Ballot"
os.system(cmd)
cmd = "cp Voter" + str(sys.argv[1]) +  "/"  + sys.argv[len(sys.argv) - 1]  + " TallyOfficial/Ballot"
os.system(cmd)
cmd = ""
f = open("TallyOfficial/Ballot/ballot.txt", "a")
for i in range(1,len(sys.argv),1):
    cmd += sys.argv[i]
    cmd += " " 
cmd += "\n"
f.write(cmd)
f.close()