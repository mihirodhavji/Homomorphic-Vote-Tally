import os
import sys

ntrustees=int(sys.argv[1])
os.system("rm combinekey")
os.system("gcc -Wall -pedantic combinekey.c -o combinekey -Lsss libsss.a -Lrandombytes randombytes/librandombytes.a")
cmd="./combinekey "+str(ntrustees)
os.system(cmd)
f = open("pw.txt", "r")
pw=f.read()
cmd="openssl aes-256-cbc -d -a -in Chave_privada.bin.enc -out Chave_privada.bin.new -k " +pw
os.system(cmd)
