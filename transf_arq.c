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
#define UDP 0
#define TCP 1



/*3. Criar uma função de envio de pacotes que recebe um pacote e um parâmetro indicando qual protocolo de camada de transporte 
será usado.*/
void envia_pacote(MensagemTexto pacote, int protocolo){
    //conectar com o cliente e enviar o pacote usando o protocolo selecionado
    /* Cria o socket */
    int sock, msg_sock;
    struct sockaddr_in server;
    struct sockaddr_in client;

    if(protocolo == TCP){
	    sock=socket(AF_INET, SOCK_STREAM, 0);
	    if (sock<0) {
	    	perror("opening stream socket");
	    	exit(1);
	    }
    
	    /* Associa */
	    server.sin_family = AF_INET;
	    server.sin_addr.s_addr = INADDR_ANY;
	    server.sin_port=htons(1234);
	    if (bind(sock,(struct sockaddr *) &server, sizeof server) < 0) {
	    	perror("binding stream socket");
	    	exit(1);
	    }

	    /* Conexoes */
    
	    if (listen(sock, 5) == -1) 
	    		printf("errou");
	    int length= sizeof(struct sockaddr_in);
        printf("DDDD  Familia: %d\n", server.sin_family);
        printf("DDDD  Endereco: %s\n", inet_ntoa(server.sin_addr));
        printf("DDDD  Porta: %d\n\n", server.sin_port);
        msg_sock= accept(sock,(struct sockaddr *) &client, &length);
    }
    else{
        if(protocolo == UDP){
             //Creating socker file descriptor
            if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }
            //Criação do pacote UDP
            int serverLength = sizeof(server);
            int clientLength = sizeof(client);
            memset(&server, 0, serverLength);
            memset(&server, 0, serverLength);

            //Filling server information
            server.sin_family = AF_INET;                   //IPV4
            server.sin_addr.s_addr = INADDR_ANY;
            server.sin_port = htons(1234);

            //bind
            if ((bind(sock, (const struct sockaddr *)&server, sizeof(server))) < 0)
            {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
        }
        else{
            printf("Protocolo não identificado");
        }
    }
	
    
}

int qttd_bloco;

int main()
{
    FILE *arquivo;
    char str[80];
    int size_arquivo;
    extern int qttd_bloco;
    printf("digite o nome do arquivo:");
    gets(str);
    arq_fragmentos *blocos;
    int tam_bloco = BITS/8; // o resto seria o numero de bits do ultimo pacote
    blocos = fragmenta_arq(str, BITS);
    //chamada de criação dos pacotes
    MensagemTexto *pacotes;
    pacotes = cria_pacote(qttd_bloco);
    for(int i=0; i<qttd_bloco;i++){
        preenche_pacote(&pacotes[i], tam_bloco, blocos[i].fragmentos, 1234, 1235);
        
        consulta_pacote(pacotes[i]);
    }
    //("%s", (blocos[0].fragmentos));

}

/*
    Generic checksum calculation function
*/

/*
1. Função que recebe, por parâmetro: o caminho para um arquivo (que neste caso será um txt) e também a uma quantidade 
máxima de bits a qual esse arquivo será fracionado. Essa função retorna um array ou uma estrutura de dados com o arquivo 
totalmente fragmentado conforme solicitado.

2. Criação de pacotes a serem transferidos, já com conteúdo completo. Usando os fragmentos gerados pela função anterior.

3. Criar uma função de envio de pacotes que recebe um pacote e um parâmetro indicando qual protocolo de camada de transporte 
será usado.

3.1. Essa função deve validar se o pacote está construido corretamente, inclusive validar se o campo de validação está correto. 
Lembrando que os protocolos de camada de transporte tem seus próprios algoritmos de verificação e vcs devem usá-los.

3.2. Ainda nesta função deve-se criar uma estrutura de comunicação com um receptor que esteja em qualquer parte do mundo, 
mas para esta fase do trabalho, pode ser no mesmo computador mesmo.

4. Criar um outro programa que servirá para receber os pacotes transmitidos.

4.1. Compartilhar com o outro programa as estruturas básicas e idênticas (arquivo .h único?!?)

4.2. Desenvolver uma função que recebe os pacotes e os registra um a um em algum lugar para que possam ser analisados.

4.3. Criar uma função que faz a análise dos pacotes recebidos, incluindo a verificação de integridade.

4.4. Criar uma função que, após validação (da outra função) remonta o arquivo texto em HD.
4.1. Compartilhar com o outro programa as estruturas básicas e idênticas (arquivo .h único?!?)

4.2. Desenvolver uma função que recebe os pacotes e os registra um a um em algum lugar para que possam ser analisados.

4.3. Criar uma função que faz a análise dos pacotes recebidos, incluindo a verificação de integridade.

4.4. Criar uma função que, após validação (da outra função) remonta o arquivo texto em HD.

4.5. Apenas a título de teste, faça uma função para imprimir na tela o conteúdo do arquivo.
*/