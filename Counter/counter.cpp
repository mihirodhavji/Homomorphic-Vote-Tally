#include <cstddef>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <memory>
#include <limits>
#include <algorithm>
#include <numeric>
#include "seal/seal.h"
#include <iostream>

using namespace std;
using namespace seal;

int hexadecimalToDecimal(string hexVal) 
{    
    int len = hexVal.length();       
    int base = 1; 
    int dec_val = 0; 
      
    for (int i=len-1; i>=0; i--) 
    {    
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
            base = base * 16; 
        } 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
            base = base*16; 
        } 
    }
    return dec_val; 
}


int main(int argc, char *argv[] )
{
    if (argc < 2)
    {
        printf("Some paremeters are missing\n");
        exit(1);
    }
    int n_c = atoi(argv[1]);
    int n_v = atoi(argv[2]);
    int checksum = n_v * n_c;
    system("sudo chmod 0755 verifica_prikey");
    system("./verifica_prikey");
    string line = "";
    string cmd1 = "";
    ifstream myfile1 ("temp.txt");
    getline (myfile1,line);
    myfile1.close();
    system("rm temp.txt");
    cmd1 = "Verified OK";
    if (line.compare(cmd1) != 0){
        printf("Chave privada errada.\n");
        exit(0);
    }

    int i = 0;
    int *votes = NULL;
    votes = (int *)malloc(sizeof(int)*n_c);
    if( votes == NULL)
    {
        perror("Unable to allocate memory");
        exit(1);
    }
    EncryptionParameters parms(scheme_type::BFV);
	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(1024);
    auto context = SEALContext::Create(parms);

    ifstream Chave_privada;
    Chave_privada.open("Chave_privada.bin.new",  ifstream::binary);
    SecretKey secret_key;
    secret_key.load(context, Chave_privada);
    Chave_privada.close();
    
    Decryptor decryptor(context, secret_key);
    Ciphertext e;
    Plaintext d;
    string entrada="";

    ifstream entrada_msg1;
    for (i = 0; i < n_v ; i++)
    {
        entrada = "voter_checksum_";
        entrada.append(to_string(i+1));
        entrada.append(".bin");
        entrada_msg1.open(entrada, ifstream::binary);
        e.load(context, entrada_msg1);
        entrada_msg1.close();
        decryptor.decrypt(e, d);
        entrada = "";
        entrada = (d.to_string());
        if (n_c != hexadecimalToDecimal(entrada)){
            printf("Voter Checksum not verified.Election canceled.\n");
            exit(1);
        }
    }
    ifstream checksum_msg;
    checksum_msg.open("checksum.bin", ifstream::binary);
    e.load(context, checksum_msg);
    checksum_msg.close();
    
    decryptor.decrypt(e, d);
    
    entrada = (d.to_string());
    int result = hexadecimalToDecimal(entrada);
    
    
    if (result == checksum)
        printf("Checksum verified.\n");
    else
    {
        printf("Checksum not verified.Election canceled.\n");
        exit(1);
    }
    ifstream entrada_msg;
    for (i = 0; i < n_c ; i++)
    {
        entrada = "candidate_";
        entrada.append(to_string(i+1));
        entrada.append(".bin");
        entrada_msg.open(entrada, ifstream::binary);
        e.load(context, entrada_msg);
        entrada_msg.close();
        decryptor.decrypt(e, d);
        entrada = (d.to_string());
        votes[i] = hexadecimalToDecimal(entrada);
    }
    for(i = 0; i < n_c ; i++)
    {
        printf("Candidato %d - %d\n",i+1,votes[i]);
    }

    

}



