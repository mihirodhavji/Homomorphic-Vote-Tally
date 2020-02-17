import os
import sys

if(len(sys.argv) != 4):
    print("Numero de argumentos errado")
    exit(-1)
n_c = int(sys.argv[1])
n_v = int(sys.argv[2])
n_t = int(sys.argv[3])
if (n_c < 1 or n_v < 1 or n_t < 1):
    print("Parametros n_c ou n_v ou n_t incorretos")
    exit(-1)
#criar a chave homomorfica e a lista de pesos dos votantes
print("Gerar a chave homomorfica e a lista de pesos dos votantes")
os.system("cmake .")
os.system("make")
cmd1 = "./set " + sys.argv[2]
os.system(cmd1)
#criar a chave e o certificado CA
for j in range(25):
    print("*")
print("****************************************************************************************************************************")
print("****************************************************************************************************************************")
print("***********************************************Criacao da chave e certificado CA root **************************************")
print("****************************************************************************************************************************")
print("****************************************************************************************************************************")
os.system("sudo openssl genrsa -des3 -out my-ca.key 2048")
os.system("sudo chmod 0777 my-ca.key")
os.system("openssl req -new -x509 -days 3650 -key my-ca.key -out my-ca.crt")
for j in range(25):
    print("*")
print("Assinar a chave privada e publica")
os.system("openssl dgst -sha256 -sign my-ca.key -out Chave_publica.bin.sha256 Chave_publica.bin")
os.system("openssl dgst -sha256 -sign my-ca.key -out Chave_privada.bin.sha256 Chave_privada.bin")
#openssl dgst -sha256 -verify  <(openssl x509 -in my-ca.crt  -pubkey -noout) -signature Chave_privada.bin.sha256 Chave_privada.bin
#openssl dgst -sha256 -verify  <(openssl x509 -in Voter1.crt  -pubkey -noout) -signature voto.txt.sha256 voto.txt
os.system("cp Chave_privada.bin sss")
os.chdir("sss")
cmd1 = "python3 splitkey.py " + str(n_t)
os.system(cmd1)
os.system("rm Chave_privada.bin")
os.chdir("..")
os.system("rm Chave_privada.bin")
os.chdir("..")
cmd1 = "cp Admin/sss/Chave_privada.bin.enc Counter/sss"
os.system(cmd1)
os.system("rm Admin/sss/Chave_privada.bin.enc")
for i in range(n_t):
    cmd1 = "cp Admin/sss/trustees" + str(i) + ".bin Counter/sss"
    os.system(cmd1)
    cmd1 = "rm Admin/sss/trustees" + str(i) + ".bin"
    os.system(cmd1)
for j in range(25):
    print("*")
os.chdir("Admin")
print("assinar a lista com o pesos dos votantes")
cmd1 = "cat "
for i in range(n_v):
    cmd1 += "voter_weight"+ str(i+1) + ".bin "
cmd1 += "> voter_weight_list.bin" 
os.system(cmd1) 
cmd1 = "openssl dgst -sha256 -sign my-ca.key -out voter_weight_list.bin.sha256 voter_weight_list.bin"
os.system(cmd1) 
os.chdir("..")
#openssl dgst -sha256 -verify  <(openssl x509 -in my-ca.crt  -pubkey -noout) -signature Chave_privada.bin.sha256 Chave_privada.bin
os.system("cp Admin/my-ca.crt TallyOfficial")
os.system("cp Admin/voter_weight_list.bin.sha256 TallyOfficial")
############################################################
os.chdir("TallyOfficial")
os.system("mkdir Ballot")
os.chdir("Ballot")
os.chdir("..")
os.chdir("..")

for i in range(n_v):
    for j in range(25):
        print("*")
    cmd2 = "cp Admin/voter_weight" + str(i+1) + ".bin TallyOfficial"
    os.system(cmd2)
    print("**************************************************************************************************************************************")
    print("**************************************************************************************************************************************")
    print("************************************************ Fazer req certificado do voter"+ str(i+1) + " *****************************************************")
    print("**************************************************************************************************************************************")
    print("**************************************************************************************************************************************")
    folder = "Voter" + str(i+1)
    cmd1 = "mkdir " + folder
    os.system(cmd1)
    os.chdir(folder)
    cmd1 = "openssl genrsa -out Voter" + str(i+1) + ".pem 1024"
    cmd2 = "openssl req -new -key Voter" + str(i+1) + ".pem -out Voter" + str(i+1) + ".csr"
    os.system(cmd1)
    os.system(cmd2)
    os.chdir("..")

for i in range(n_v):
    cmd1 = "mv Voter"+ str(i+1) +"/Voter" + str(i+1) + ".csr Admin"
    os.system(cmd1)
os.chdir("Admin")
for i in range(n_v):
    for j in range(25):
        print("*")
    print("**************************************************************************************************************************************")
    print("**************************************************************************************************************************************")
    print("*******************************Root CA assina o certificado do voter"+ str(i+1) + " ****************************************************************")
    print("**************************************************************************************************************************************")
    print("**************************************************************************************************************************************")
    cmd1 = "openssl x509 -req -in Voter"+ str(i+1) + ".csr -out Voter"+ str(i+1) + ".crt -sha1 -CA my-ca.crt -CAkey my-ca.key -CAcreateserial -days 3650"
    cmd2 = "rm Voter" + str(i+1) + ".csr"
    os.system(cmd1) 
    os.system(cmd2)
os.chdir("..")
os.chdir("VoterApp")
os.system("cmake .")
os.system("make")
os.chdir("..")
for i in range(n_v):
    cmd1 = "cp Admin/Voter" + str(i+1) + ".crt Voter" + str(i+1)
    cmd2 = "cp Admin/my-ca.crt Voter" + str(i+1)
    cmd3 = "cp Admin/Chave_publica.bin Voter" + str(i+1)
    cmd4 = "cp VoterApp/voterapp Voter" + str(i+1)
    cmd5 = "cp VoterApp/vote_cast.py Voter" + str(i+1)
    cmd6 = "cp VoterApp/vote_cast2.py Voter" + str(i+1)
    cmd7 = "openssl rsa -in Voter"+ str(i+1) + "/Voter" + str(i+1) + ".pem -out Voter"+ str(i+1) + "/Voter" + str(i+1) + ".key"
    os.system(cmd1)
    os.system(cmd2)
    os.system(cmd3)
    os.system(cmd4)
    os.system(cmd5)
    os.system(cmd6)
    os.system(cmd7)
for j in range(25):
    print("*")
os.system("cp Admin/Chave_privada.bin.sha256 Counter")
os.system("cp Admin/my-ca.crt Counter")
print("The voters folders are ready for voting. Go there and vote.")
print("After voting come back here. Do not close this terminal.N_C = " + str(n_c))
input("Press Enter to count votes ...")
os.chdir("TallyOfficial")
os.system("cmake .")
os.system("make")
cmd1 = "cat "
for i in range(n_v):
    cmd1 += "voter_weight"+ str(i+1) + ".bin "
cmd1 += "> voter_weight_list.bin" 
os.system(cmd1)
os.system("sudo chmod 0755 verifica_voter_list")
os.system("./verifica_voter_list")
myfile = open("temp.txt","r")
first_line = myfile.readline().rstrip()
myfile.close()
os.system("rm temp.txt")
cmd2 = "Verified OK"
if (first_line != cmd2):
    print("voter weigth list invalid")
    exit(-1)
cmd1 = "python3 verifica_ballot.py " + sys.argv[1]
os.system(cmd1)
print("saiu do ballot")
os.system("python3 verifica_ballot1.py")
print("saiu do ballot1")
os.system("python3 verifica_ballot2.py")
print("saiu do ballot2")
myfile = open("Ballot/ballot3.txt","r")
content = myfile.readlines()
myfile.close()
n_v = len(content)
os.system("python3 verifica_ballot3.py")
print("saiu do ballot3")
os.system("python3 verifica_ballot4.py")
print("saiu do ballot4")
os.system("python3 verifica_ballot5.py")
print("saiu do ballot5")
os.system("python3 verifica_ballot6.py")
print("saiu do ballot6")
cmd1 = "./tallyofficial " + sys.argv[1] + " " + str(n_v)
os.system(cmd1)
os.chdir("..")
os.chdir("Counter")
os.system("cmake .")
os.system("make")
os.chdir("sss")
cmd1 = "python3 combinekey.py " + str(n_t)
os.system(cmd1)
os.chdir("..")
os.chdir("..")
os.system("cp Counter/sss/Chave_privada.bin.new Counter")
os.chdir("Counter")
cmd1 = "./counter " + sys.argv[1] + " " + str(n_v)
os.system(cmd1)
os.chdir("..")
