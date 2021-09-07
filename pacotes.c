#include "transf_arq.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <time.h>

int qttd_bloco;

MensagemTexto *cria_pacote(int num_pacotes)
{
    //alocar
    MensagemTexto *envio;
    envio = malloc(sizeof(MensagemTexto) * num_pacotes);
    return envio;
}


MensagemTexto * recebe(int protocolo){
    //pacotes armazenados vetor estrutura arq_fragmentos
    int sock,tam;
    
    struct sockaddr_in name, server;
    struct hostent *hp, *gethostbyname();
    struct MensagemTexto msg, *pacotes;
    int len = sizeof(msg), length = sizeof(server);
    if(!(pacotes = (MensagemTexto *) malloc(sizeof(MensagemTexto)))){
         printf("Erro");
    } 
   
   if(protocolo == TCP){
	    sock=socket(AF_INET, SOCK_STREAM, 0);
	    if (sock<0) {
	    	perror("opening stream socket");
	    	exit(1);
	    }
    
	    /* Associa */
	    server.sin_family=AF_INET;
        hp = gethostbyname("localhost");
        if (hp==0) {
            fprintf(stderr, "Unknown host\n");
            exit(2);
        }
        bcopy ((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	    server.sin_port = htons(1234);


        
    if (connect (sock,(struct sockaddr *)&server,sizeof server)<0) {
                perror("connecting stream socket");
		exit(1);
	}
    
	    /* Conexoes */
        

        do{
            if ((recv(sock, (char *)&msg, len, 0)) ==0){
				perror("Fim da Conexão");
                break;

            }
            else{
                qttd_bloco++;
                if(!(pacotes = (MensagemTexto *) realloc(pacotes, (qttd_bloco)*sizeof(MensagemTexto)))){
                    printf("Errouu");
                }
                preenche_pacote(&pacotes[qttd_bloco-1], msg.tam, msg.mensagem, msg.IdEnvio, msg.IdRecebe);
                pacotes[qttd_bloco-1].checksum =msg.checksum;
                consulta_pacote(pacotes[qttd_bloco-1]);
            } 
        }while(1);
        close(sock);
        return pacotes;
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
            server.sin_family = AF_INET;
            server.sin_port= htons (1234);
            
            /* Acessa o DNS */
            hp = gethostbyname("localhost");
            if (hp==0){
              perror("Host nao encontrado");
              exit(2);
            }
            
            bcopy ((char*)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
            MensagemTexto sinc;
            if (sendto (sock, (char *)&sinc, len, 0, (struct sockaddr *)&server, length) < 0) 
                perror("Envio da mensagem");
            
            if ((recvfrom(sock, (char *)&qttd_bloco, sizeof(int), 0, (struct sockaddr *)&server, &length)) <=0)
			    	perror("Fim da Conexão");
            printf("%d\n", qttd_bloco);
            for(int i=0;i<qttd_bloco;i++){
                if ((recvfrom(sock, (char *)&msg, len, 0, (struct sockaddr *)&server, &length)) <=0){
			    	perror("Fim da Conexão");
                    break;

                }
                else{
                    if(!(pacotes = (MensagemTexto *) realloc(pacotes, (i+1)*sizeof(MensagemTexto)))){
                        printf("Errouu");
                    }
                    preenche_pacote(&pacotes[i], msg.tam, msg.mensagem, msg.IdEnvio, msg.IdRecebe);
                    pacotes[i].checksum =msg.checksum;
                    consulta_pacote(pacotes[i]);
                } 
             }
            close(sock);
            return pacotes;
        }
        else{
            printf("Protocolo não identificado");
        }
    }

}


//testar a funcao csum e ver seu retorno, se for algo relativamente satisfatorio adicionar no preenchimento do pacote o valor de csum desse pacote
//printar para ver se nao houve mudança no valor, tentar enviar para o "cliente" e printar para verificar se nao teve mudança
//entao aplicar no cliente a funcao csum apos isso comparar os csum's que foi enviado com o novo calculado.
//aplicar csum apenas na mensagem que esta sendo enviada


void destroi_pacote(MensagemTexto *pacote)
{
    //desalocar
    free(pacote);

    //char data;
    //data = recvfrom(sockfd, "?", "?", 0, "?", &length);
    //free(data);
}

void preenche_pacote(MensagemTexto *pacote, int tam, char *mensagem, unsigned int IdEnvio, unsigned int IdRecebe)
{
    //preencher os campos do pacote
    pacote->tam = tam;
    pacote->IdEnvio = IdEnvio;
    pacote->IdRecebe = IdRecebe;
   
    memcpy(&(pacote->mensagem), mensagem, tam);
}

void consulta_pacote(MensagemTexto pacote)
{
    //mostra os campos do pacote
    printf("\n************************************\n");
    printf("Porta Origem: %d\n", pacote.IdEnvio);
    printf("Porta Destino: %d\n", pacote.IdRecebe);
    printf("Tamanho: %d\n", pacote.tam);
    printf("Mensagem:");
    fwrite(pacote.mensagem, pacote.tam,1, stdout);
    printf("\nChecksum : %04X \n", pacote.checksum);
    printf("\n------------------------------------\n");
}

void envia_pacote(MensagemTexto * pacote, int protocolo){
    //conectar com o cliente e enviar o pacote usando o protocolo selecionado
    /* Cria o socket */
    int sock, msg_sock;
    struct sockaddr_in server;
    struct sockaddr_in client;
	int length= sizeof(struct sockaddr_in);
    int len = sizeof(MensagemTexto);
    struct hostent *hp, *gethostbyname();
    
    if(protocolo == TCP){
	    sock=socket(AF_INET, SOCK_STREAM, 0);
	    if (sock<0) {
	    	perror("opening stream socket");
	    	exit(1);
	    }
    
	    /* Associa */
	    server.sin_family = AF_INET;
	    server.sin_port=htons(1234);

                /* Acessa o DNS */
        hp = gethostbyname("localhost");
        if (hp==0){
          perror("Host nao encontrado");
          exit(2);
        }
        bcopy ((char*)hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	    if (bind(sock,(struct sockaddr *) &server, sizeof server) < 0) {
	    	perror("binding stream socket");
	    	exit(1);
	    }
        
	    /* Conexoes */

	    if (listen(sock, 5) == -1) 
	    		printf("errou");

        printf("DDDD  Familia: %d\n", server.sin_family);
        printf("DDDD  Endereco: %s\n", inet_ntoa(server.sin_addr));
        printf("DDDD  Porta: %d\n\n", server.sin_port);
        msg_sock= accept(sock,(struct sockaddr *) &client, &length);      
        for(int i=0;i<qttd_bloco;i++){
            pacote[i].IdRecebe=ntohs(client.sin_port);
            bzero(&(pacote->checksum), sizeof(unsigned short int));
            pacote[i].checksum=check(&pacote[i], 12);
           // consulta_pacote(pacote[i]);
            send(msg_sock, (char*)&pacote[i], len,0);
            
        }
        sleep(2);
        close(sock);
        close(msg_sock);


    }
    else{
        if(protocolo == UDP){
             //Creating socker file descriptor
            /*Cria o socket*/
            /*socket(familia do socket, --- ,num do protocolo)*/
            sock = socket(AF_INET, SOCK_DGRAM, 0);
            if(sock<0){ /*se nao conseguir abrir o socket*/
              perror ("abertura de socket");
              exit(1);
            }

            server.sin_family = AF_INET; //sin socket internet
            server.sin_port = htons(1234); //deixa o SO atribuir a porta

            hp = gethostbyname("localhost");
            if (hp==0){
              perror("Host nao encontrado");
              exit(2);
            }
            bcopy ((char*)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
            /*parametros do bind o descritor, struct de endereÃ§o de socket*/
            /*se der erro no bind, se a porta ja ta em uso etc*/
            if(bind (sock, (struct sockaddr *)&server, sizeof server) < 0){
              perror ("binding datagrama");
              exit(1);
            }
            printf("SSSS  Familia: %d\n", server.sin_family);
            printf("SSSS  Endereco: %s\n", inet_ntoa(server.sin_addr));
            printf("SSSS  Porta: %d\n\n", ntohs(server.sin_port));
            MensagemTexto sinc;
            if (recvfrom (sock, (char *)&sinc, sizeof len, 0, (struct sockaddr *)&client, &length) < 0) 
                perror("Envio da mensagem");
            printf("CCCC  Familia: %d\n", client.sin_family);
            printf("CCCC  Endereco: %s\n", inet_ntoa(client.sin_addr));
            printf("CCCC  Porta: %d\n\n", ntohs(client.sin_port));
            if (sendto (sock, (char *)&qttd_bloco, sizeof(int), 0, (struct sockaddr *)&client, length) < 0) 
                    perror("Envio da mensagem");
        
            for(int i=0;i<qttd_bloco;i++){
                pacote[i].IdRecebe=ntohs(client.sin_port);
                bzero(&(pacote->checksum), sizeof(unsigned short int));
                pacote[i].checksum=check(&pacote[i], 12);
               
                if (sendto (sock, (char *)&pacote[i], len, 0, (struct sockaddr *)&client, length) < 0) 
                    perror("Envio da mensagem");
                    
                   
                    //consulta_pacote(pacote[i]);
            }
            close(sock);

        }
        else{
            printf("Protocolo não identificado");
        }
    }
	
    
}

void close_socket(int sock){
    static int flag =0;
    if(flag != 0) {
        //close(flag);
        printf("Fechando Socket...\n");
    }
    else{
        
        flag = sock;
        //printf("Atribui %d\n", flag);
    }
    
}

uint16_t check(uint16_t * pacote, int size){
    uint16_t *pak = pacote;
    uint16_t cksum=0;

    for(int i=0;i<size/sizeof(uint16_t);i++){
        cksum+=(uint16_t)*pak;
        pak++;
    }
    cksum=~cksum;
    return cksum;
}

int valida_pacotes(MensagemTexto *pacotes){
    unsigned short csum;
    int valida=0;
    for(int i=0;i<qttd_bloco;i++){
        if((check((uint16_t *)&pacotes[i],12))==0 ) {
            valida++;
        }
        else {
            printf("Pacote Incorreto");
            return 0;
        }
    }
  //  printf("okkk: %d", valida);
    if(qttd_bloco == valida) printf("\nTodos os pacotes estão válidos\n\n"); return valida;
    
}