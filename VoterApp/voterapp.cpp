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
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d:%m:%Y:%X", &tstruct);

    return buf;
}

int main(int argc, char *argv[] )
{
    ////// verifica se estao todos os parametros
    if (argc < 4)
    {
        printf("Some paremeters are missing\n");
        exit(1);
    }
    int n_c = atoi(argv[1]);
    if (n_c < 1){
        printf("Parametros n_c incorreto\n");
        exit(0);
    }
    int i = 0;
    char *cmd = NULL;  
    string line = "";
    ///// vai buscar o id ao certificado do votante
    string cmd1 = "openssl x509 -noout -subject -in ";
    cmd1.append(argv[2]);
    cmd1.append(" | sed -e 's/^subject.*O = \\([a-zA-Z0-9\\.\\-]*\\).*$/\\1/' > temp.txt");
    system(cmd1.c_str());
    ifstream myfile ("temp.txt");
    getline (myfile,line);
    myfile.close();
    system("rm temp.txt");
    cmd1 = "Voter";
    cmd = (char *)malloc( sizeof(char) * (line.length() - cmd1.length()) );
    if( cmd == NULL)
    {
        perror("Unable to allocate memory");
        exit(1);
    }
    for(i = cmd1.length(); i < line.length() ; i++)
    {
        cmd[i-cmd1.length()] = line[i];
    }
    int id = atoi(cmd);
    free(cmd);
    cmd = NULL;
    int voto = 0, x = 0;
    int *votos_dados = NULL;
    char *buffer = NULL;
    size_t bufsize = 100;
    
    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
    
    votos_dados = (int *)malloc(sizeof(int) * n_c);
    if( votos_dados == NULL)
    {
        perror("Unable to allocate memory");
        exit(1);
    }

    printf("Welcome to the voting system.\n");
    printf("There are %d candidates, so you can vote %d times. You can also vote in the same candidate more than 1 time.\n",n_c,n_c);
    printf("To vote for a candidate you have to choose a number between 0 - %d, that means the number of times you vote for him.\n", n_c);
    printf("Let's start the voting process.\n");
    printf("The candidates list is :\n");

    for(i = 0; i < n_c; i++){
        printf("Candidato %d\n",i+1);
        votos_dados[i] = 0;
    }

    for (i = 0; i < n_c; i++)
    {
        voto = 0;
        printf("How many votes for Candidato %d - ",i+1);
        getline(&buffer,&bufsize,stdin);
        sscanf(buffer,"%d",&voto);
        if (voto < 0){
            printf("Voto Inválido\n");
            i--;
        }
        else{
            votos_dados[i] = voto;  
        }
    }
    EncryptionParameters parms(scheme_type::BFV);
	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(1024);
    auto context = SEALContext::Create(parms);
    
	ifstream Chave_publica1;
    Chave_publica1.open(argv[3], ifstream::binary);
    PublicKey public_key;
    public_key.load(context, Chave_publica1);
    Chave_publica1.close();
    
    Encryptor encryptor(context, public_key);
    Ciphertext msg_encrypted1;
    string filename;
    string path;
    string time = currentDateTime();
    string ballot_str = "";
    string sign = "cat ballot_text.txt ";
    string id_date = "";
    ballot_str.append(to_string(id));
    ballot_str.append(" ");
    ballot_str.append(time);
    ballot_str.append(" ");
    id_date.append(to_string(id));
    id_date.append("_");
    id_date.append(time);

    for(i = 0; i < n_c; i++){
        printf("Candidato %d : %d\n",i+1,votos_dados[i]);
        x = votos_dados[i];
        Plaintext msg_plain1(to_string(x));
        encryptor.encrypt(msg_plain1, msg_encrypted1);
        ofstream msg1;
        filename = "";
        filename.append(time);
        filename.append("_candidate_");
        filename.append(to_string(i+1));
        filename.append("voter_");
        filename.append(to_string(id));
        filename.append(".bin");
        ballot_str.append(filename);
        ballot_str.append(" ");
        sign.append(filename);
        sign.append(" ");

        msg1.open(filename, ofstream::binary);
        msg_encrypted1.save(msg1);
        msg1.close();
        path = "python3 vote_cast.py ";
        path.append(filename);
        path.append(" ");
        path.append(to_string(id));
        system(path.c_str());

    }
    ofstream file;
    file.open("ballot_text.txt");
    file << ballot_str;
    file.close();
    sign.append("> sign_");
    sign.append(id_date);
    sign.append(".txt");
    system(sign.c_str());
    cmd1 = "openssl dgst -sha256 -sign Voter";
    cmd1.append(to_string(id));
    cmd1.append(".key -out sign_");
    cmd1.append(id_date);
    cmd1.append(".txt.sha256 sign_");
    cmd1.append(id_date);
    cmd1.append(".txt");
    system(cmd1.c_str());
    cmd1 = "python3 vote_cast2.py ";
    cmd1.append(ballot_str);
    cmd1.append(" sign_");
    cmd1.append(id_date);
    cmd1.append(".txt.sha256 ");
    cmd1.append(argv[2]);
    system(cmd1.c_str());
    free(buffer);
    free(votos_dados);
    return 0;

}

