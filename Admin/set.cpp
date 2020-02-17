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

int main(int argc, char * argv[])
{
    if (argc < 2) {
        printf("falta o numero de votantes\n.");
        exit(1);
    }
    int n_v = atoi(argv[1]);
    int i = 0, x = 0;
	EncryptionParameters parms(scheme_type::BFV);
	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(1024);
    auto context = SEALContext::Create(parms);

	KeyGenerator keygen(context);
    PublicKey public_key = keygen.public_key();
    SecretKey secret_key = keygen.secret_key();

    ofstream Chave_publica;
    Chave_publica.open ("Chave_publica.bin", ofstream::binary);
    public_key.save(Chave_publica);
    Chave_publica.close();

    ofstream Chave_privada;
    Chave_privada.open ("Chave_privada.bin", ofstream::binary);
    secret_key.save(Chave_privada);
    Chave_privada.close();

    Encryptor encryptor(context, public_key);
    Plaintext msg_plain1;
    Ciphertext msg_encrypted1;
    string filename;

    for(i = 0; i < n_v; i++){
        x = i+1;
        msg_plain1 = to_string(x);
        encryptor.encrypt(msg_plain1, msg_encrypted1);
        ofstream msg1;
        filename = "";
        filename.append("voter_weight");
        filename.append(to_string(i+1));
        filename.append(".bin");
        msg1.open(filename, ofstream::binary);
        msg_encrypted1.save(msg1);
        msg1.close();
    }
    

}