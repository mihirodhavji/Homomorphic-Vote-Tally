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

int main(int argc, char *argv[] )
{
    if (argc < 3)
    {
        printf("Some paremeters are missing\n");
        exit(1);
    }
    int n_c = atoi(argv[1]);
    int n_v = atoi(argv[2]);
    int i = 0, j = 0;

    EncryptionParameters parms(scheme_type::BFV);
	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(1024);
    auto context = SEALContext::Create(parms);

    Evaluator evaluator(context);
    Ciphertext candidate_sum;
    Ciphertext weight;
    Ciphertext vote;
    Ciphertext weighted_vote;
    Ciphertext checksum;
    string vote_filename = "";
    string vote_filename1 = "";
    string weight_filename = "";
    string saida_filename = "";
    string str1 = "";
    ////// i = candidato
    ////// j = votante
    int a , b;
    ifstream file("Ballot/ballot6.txt");
    for(a = 0; a < n_c ; a++)
    {
        for(b = 0; b < n_v ; b++)
        {
            getline(file, str1);
            getline(file, vote_filename);
            sscanf(str1.c_str(),"%d %d",&i, &j);
            vote_filename1 = "Ballot/";
            vote_filename1.append(vote_filename);

            ifstream vote_msg;
            vote_msg.open(vote_filename1, ifstream::binary);
            vote.load(context, vote_msg);
            vote_msg.close();

            weight_filename = "voter_weight";
            weight_filename.append(to_string(j));
            weight_filename.append(".bin");
            ifstream weight_msg;
            weight_msg.open(weight_filename, ifstream::binary);
            weight.load(context, weight_msg);
            weight_msg.close();
            evaluator.multiply(weight, vote, weighted_vote);
            if ( b == 0)
                candidate_sum = weighted_vote;
            else
                evaluator.add(candidate_sum, weighted_vote, candidate_sum);
            if (a == 0 & b == 0)
                checksum = vote;
            else
                evaluator.add(checksum, vote, checksum);
        }
        ofstream saida_msg;
        saida_filename = "candidate_";
        saida_filename.append(to_string(i));
        saida_filename.append(".bin");
        saida_msg.open(saida_filename, ifstream::binary);
        candidate_sum.save(saida_msg);
        saida_msg.close();
        
    }
    ofstream saida_msg;
    saida_filename = "checksum.bin";
    saida_msg.open(saida_filename, ifstream::binary);
    checksum.save(saida_msg);
    saida_msg.close();
    ////// i = candidato
    ////// j = votante
    //////////////////////////////////////////////////////////
    Ciphertext checksum_voter;
    ifstream file1("Ballot/ballot7.txt");
    for(a = 0; a < n_v ; a++)
    {
        for(b = 0; b < n_c ; b++)
        {
            getline(file1, str1);
            getline(file1, vote_filename);
            sscanf(str1.c_str(),"%d %d",&i, &j);
            vote_filename1 = "Ballot/";
            vote_filename1.append(vote_filename);

            ifstream vote_msg;
            vote_msg.open(vote_filename1, ifstream::binary);
            vote.load(context, vote_msg);
            vote_msg.close();

            if (b == 0)
                checksum_voter = vote;
            else
                evaluator.add(checksum_voter, vote, checksum_voter);
        }
        ofstream saida_msg;
        saida_filename = "voter_checksum_";
        saida_filename.append(to_string(a+1));
        saida_filename.append(".bin");
        saida_msg.open(saida_filename, ifstream::binary);
        checksum_voter.save(saida_msg);
        saida_msg.close();
        string path = "python3 send_to_counter.py ";
        path.append(to_string(n_c));
        path.append(" ");
        path.append(to_string(n_v));
        system(path.c_str());
        
    }
    return 0;

}



