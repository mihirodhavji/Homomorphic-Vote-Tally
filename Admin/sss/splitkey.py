import os
import sys

ntrustees=int(sys.argv[1])
pw="garagemdavizinha"
os.system("rm Chave_privada.bin.enc")
os.system("rm splitkey")
cmd="openssl aes-256-cbc -a -salt -in Chave_privada.bin -out Chave_privada.bin.enc -k "+pw
os.system(cmd)
os.system("gcc -Wall -pedantic splitkey.c -o splitkey -Lsss libsss.a -Lrandombytes randombytes/librandombytes.a")
cmd="./splitkey "+pw+ " "+str(ntrustees)
os.system(cmd)
