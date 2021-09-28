#include "transf_arq.h"
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


ip * cria_data_ip(int num){
    ip * data_ip = malloc(sizeof(ip)*num);
    return data_ip;
}

void preenche_ip(ip *data_ip, uint32_t ip_destino, int protocolo_transp){
    struct hostent *hp, *gethostbyname();

    hp = gethostbyname("localhost");
        if (hp==0){
          perror("Host nao encontrado");
          exit(2);
        }
    bcopy ((char*)hp->h_addr, (char *)&data_ip->Cabecalho_ip.end_ip_org, hp->h_length);
    data_ip->Cabecalho_ip.end_ip_dest = ip_destino;
    

    //Preenchimento do cabeçalho IPV4
    
    data_ip->Cabecalho_ip.versao = 4;  //Versao do IP 4
    data_ip->Cabecalho_ip.comp_cabec = sizeof(Cabecalho);
    data_ip->Cabecalho_ip.comp_datagrama = sizeof(ip);
    data_ip->Cabecalho_ip.tipo_serv = 0;
    data_ip->Cabecalho_ip.identificador = 0;//fazer
    data_ip->Cabecalho_ip.flags = 0;//nao saber ainda
    data_ip->Cabecalho_ip.desloc_frag = 0;
   
    data_ip->Cabecalho_ip.tempo_vida = 0;
    data_ip->Cabecalho_ip.protoc_super = protocolo_transp;
    data_ip->Cabecalho_ip.opcoes = 0;

    data_ip->Cabecalho_ip.checksum = check((uint16_t *) &data_ip->Cabecalho_ip, data_ip->Cabecalho_ip.comp_cabec );
    
}


void envia_pacote(ip * pacote, int protocolo){
    //conectar com o cliente e enviar o pacote usando o protocolo selecionado
    /* Cria o socket */
    int sock, msg_sock;
    struct sockaddr_in server;
    struct sockaddr_in client;
	int length= sizeof(struct sockaddr_in);
    int len = sizeof(ip);
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
        //printf("DDDD  Endereco: %s\n", inet_ntoa(server.sin_addr));
//          printf("DDDD  Porta: %d\n\n", server.sin_port);

        msg_sock= accept(sock,(struct sockaddr *) &client, &length);      
        for(int i=0;i<qttd_bloco;i++){

            pacote[i].dados.IdRecebe=ntohs(client.sin_port);
            bzero(&(pacote[i].dados.checksum), sizeof(unsigned short int));
            pacote[i].dados.checksum=check((uint16_t *)&pacote[i].dados, 12);
           // consulta_pacote(pacote[i]);
            send(msg_sock, (char*)&pacote[i], len,0);
            
        }

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
            if (recvfrom (sock, (char *)&sinc, sizeof(MensagemTexto), 0, (struct sockaddr *)&client, &length) < 0) 
                perror("Envio da mensagem");
            printf("CCCC  Familia: %d\n", client.sin_family);
            printf("CCCC  Endereco: %s\n", inet_ntoa(client.sin_addr));
            printf("CCCC  Porta: %d\n\n", ntohs(client.sin_port));
            if (sendto (sock, (char *)&qttd_bloco, sizeof(int), 0, (struct sockaddr *)&client, length) < 0) 
                    perror("Envio da mensagem");
        
            for(int i=0;i<qttd_bloco;i++){
                pacote[i].dados.IdRecebe=ntohs(client.sin_port);
                bzero(&(pacote[i].dados.checksum), sizeof(unsigned short int));
                pacote[i].dados.checksum=check((uint16_t *)&pacote[i].dados, 12);
               
                if (sendto (sock, (char *)&pacote[i], len, 0, (struct sockaddr *)&client, length) < 0) 
                    perror("Envio da mensagem");
                    
                   
                    consulta_segmento(pacote[i].dados);
            }
            close(sock);

        }
        else{
            printf("Protocolo não identificado");
        }
    }
	
    
}

MensagemTexto * recebe_pacotes(int protocolo){
    //segmentos armazenados vetor estrutura arq_fragmentos
    int sock,tam;
    
    struct sockaddr_in name, server;
    struct hostent *hp, *gethostbyname();
    struct MensagemTexto *segmentos;
    ip msg;
    int len = sizeof(msg), length = sizeof(server);
    int len_trans = sizeof(segmentos);
    
    if(!(segmentos = (MensagemTexto *) malloc(sizeof(MensagemTexto)))){
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
                if(!(segmentos = (MensagemTexto *) realloc(segmentos, (qttd_bloco)*sizeof(MensagemTexto)))){
                    printf("Errouu");
                }
                preenche_segmento(&segmentos[qttd_bloco-1], msg.dados.tam, msg.dados.mensagem, msg.dados.IdEnvio, msg.dados.IdRecebe);
                segmentos[qttd_bloco-1].checksum =msg.dados.checksum;
                consulta_segmento(segmentos[qttd_bloco-1]);
            } 
        }while(1);
        close(sock);
        return segmentos;
    }
    else{
        if(protocolo == UDP){
             //Creating socker file descriptor
            if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }
            //Criação do segmento UDP
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
            if (sendto (sock, (char *)&sinc, len_trans, 0, (struct sockaddr *)&server, length) < 0) 
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
                   // consulta_segmento(msg.dados);
                    if(!(segmentos = (MensagemTexto *) realloc(segmentos, (i+1)*sizeof(MensagemTexto)))){
                        printf("Errouu");
                    }
                    preenche_segmento(&segmentos[i], msg.dados.tam, msg.dados.mensagem, msg.dados.IdEnvio, msg.dados.IdRecebe);
                    segmentos[i].checksum =msg.dados.checksum;
                    consulta_segmento(segmentos[i]);
                } 
             }
            close(sock);
            return segmentos;
        }
        else{
            printf("Protocolo não identificado");
        }
    }

}

void destroi_pacote(ip *pacote){
    free(pacote);
}