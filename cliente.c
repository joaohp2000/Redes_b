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
    protocol = TCP; // define tipo de comunicação

    sock = inicializar_comunicacao(CLIENTE, &server); //Abre Conexão
    pacotes = recebe_pacotes(*sock, &server);         // Recebe PAcotes
    reconstroi_pacote(pacotes);                       //Reconstroi arquivo no hd

    if ((arq = fopen("arquivo.txt", "r")) == NULL)
    {
        // o programa aborta automaticamente
        printf("Erro! Impossivel abrir o arquivo!\n");
        exit(1);
    }
    while (ch != EOF)
    {
        putchar(ch);
        ch = getc(arq);
    }
    fclose(arq);
    destroi_pacote(pacotes); //libera os pacotes da memoria
    _close_sock(sock, CLIENTE);

    return 0;
}
