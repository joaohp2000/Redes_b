#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "transf_arq.h"
#include <unistd.h>

int main()
{
    FILE *arq;
    ip *pacotes;
    char ch;

    int *sock;
    struct sockaddr_in server; // define atributos do servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(1235);
    server.sin_addr.s_addr = INADDR_ANY;
    protocol = UDP; // define tipo de comunicação

    sock = inicializar_comunicacao(CLIENTE, &server); //Abre Conexão
    pacotes = recebe_pacotes(*sock, &server);         // Recebe PAcotes
    reconstroi_pacote(pacotes);                       //Reconstroi arquivo no hd

    if (!valida_pacotes(pacotes, qttd_bloco))
    { //Valida pacotes e le conteudo do arquivo
        printf("Todo os Pacotes estao validos\n");
        ler_arquivo("arquivo.txt");
    }
    destroi_pacote(pacotes); //libera os pacotes da memoria
    _close_sock(sock, CLIENTE);

    return 0;
}