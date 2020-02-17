Instruções para correr:
python3 admin.py n_c n_v n_t dentro da pasta Admin
n_c é o numero de candidatos
n_v é o numero de votantes
n_t é o numero de trustees para guardar a chave privada
exemplo : python3 admin.py 3 2 2

Neste programa quando preencher o certificado dos Voters, preencher no campo Organization Name : VoterX, onde X será o id do voter. Tem de ser mesmo VoterX.

Este programa avisa o utilizador para ir votar, e de seguida voltar.
Assim que acabar de votar, volta ao terminal que esta a correr o admin.py e carregue enter para continuar.

Para votar, tem de abrir cada uma das pastas VoterX e dentro da pasta correr: ./voterapp n_c filename.crt pubkey.bin
n_c é o numero de candidatos
filename.crt é ficheiro certificado do votante.
pubkey.bin é a chave publica que usa para encriptar o voto.
exemplo : ./voterapp 3 Voter1.crt Chave_publica.bin

Para voltar a fazer o programa de novo, devia eliminar todas as pastas VoterX, o Ballot que se encontra dentro da pasta TallyOfficial e os ficheiros trustees dentro da pasta Counter/sss.
Pode também eliminar todas as pastas e descomprimir o ficheiro .zip de novo.


