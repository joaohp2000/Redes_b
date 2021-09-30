#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "transf_arq.h"
#define BITS 32


int main()
{
    arq_fragmentos *blocos;                //                                  
    char str[80];                          //      
    printf("digite o nome do arquivo:");   // leitura do arquivo                            
    gets(str);                             // 
    blocos = fragmenta_arq(str, BITS);     // fragmenta arquivo                          


    ip *pacotes;
    int *sock;
    struct sockaddr_in server, cli; // define atributos do servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(1235);
    server.sin_addr.s_addr = INADDR_ANY;
    protocol = TCP;         // define tipo de comunicação
    
    sock = inicializar_comunicacao(SERVIDOR, &server, &cli); //inicializa servidor tipo TCP

    pacotes = cria_data_ip(qttd_bloco); //cria pacotes ip



    for(int i=0; i<qttd_bloco;i++){
        preenche_segmento(&pacotes[i].dados, blocos[i].size, blocos[i].fragmentos, cli.sin_port , server.sin_port);
        preenche_ip(&pacotes[i], cli.sin_addr.s_addr, TCP_IP);
        
       //consulta_segmento(segmentos[i]); 
       //segmentos[i].checksum = csum()
    }
    envia_pacote(pacotes, sock, &cli); //Envia os segmentos 
    destroi_pacote(pacotes);     //libera os segmentos da memoria
    _close_sock(sock, SERVIDOR);
    

   /*int *sock;
   ip pacote;
   bzero(&pacote, sizeof(pacote));
   pacote.dados.tam = 15;
   struct sockaddr_in server, cli;
   server.sin_family = AF_INET;
   server.sin_port = htons(1234);
   server.sin_addr.s_addr = INADDR_ANY;
   protocol = TCP;
   sock = inicializar_comunicacao(SERVIDOR, &server, &cli);
   envia(&pacote, &cli, sock);
   _close_sock(sock);
*/
}



