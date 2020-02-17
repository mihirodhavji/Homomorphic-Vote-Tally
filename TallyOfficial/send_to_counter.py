import os
import sys
os.chdir("..")
n_c = int(sys.argv[1])
for i in range(n_c):
    cmd = "cp TallyOfficial/candidate_" + str(i+1)  + ".bin Counter"
    os.system(cmd)
os.system("cp TallyOfficial/checksum.bin Counter")
n_v = int(sys.argv[2])
for i in range(n_v):
    cmd = "cp TallyOfficial/voter_checksum_" + str(i+1)  + ".bin Counter"
    os.system(cmd)
os.chdir("TallyOfficial")