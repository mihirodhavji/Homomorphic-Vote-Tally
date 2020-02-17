import os
import sys

# 2 = id
# 1 = filename .bin
os.chdir("..")
cmd = "cp Voter" + str(sys.argv[2]) +  "/" + sys.argv[1] + " TallyOfficial/Ballot"
os.system(cmd)

