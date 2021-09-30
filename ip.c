#include "transf_arq.h"
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <unistd.h>

ip *cria_data_ip(int num)
{
    ip *data_ip = malloc(sizeof(ip) * num);
    return data_ip;
}

void preenche_ip(ip *data_ip, uint32_t ip_destino, int protocolo_transp)
{
    struct hostent *hp, *gethostbyname();

    hp = gethostbyname("localhost");
    if (hp == 0)
    {
        perror("Host nao encontrado");
        exit(2);
    }
    bcopy((char *)hp->h_addr, (char *)&data_ip->Cabecalho_ip.end_ip_org, hp->h_length);
    data_ip->Cabecalho_ip.end_ip_dest = ip_destino;

    //Preenchimento do cabeçalho IPV4

    data_ip->Cabecalho_ip.versao = 4;                     //Versao do IP 4 //feito
    data_ip->Cabecalho_ip.comp_cabec = sizeof(Cabecalho); // feito
    data_ip->Cabecalho_ip.comp_datagrama = sizeof(ip);    // feito
    data_ip->Cabecalho_ip.tipo_serv = 0;                  //verificar
    data_ip->Cabecalho_ip.identificador = 0;              //fazer
    data_ip->Cabecalho_ip.flags = 0;                      //fazer
    data_ip->Cabecalho_ip.desloc_frag = 0;                //verificar

    data_ip->Cabecalho_ip.tempo_vida = 0;                  //feito
    data_ip->Cabecalho_ip.protoc_super = protocolo_transp; // feito
    data_ip->Cabecalho_ip.opcoes = 0;                      // feito
    //feito
    data_ip->Cabecalho_ip.checksum = check((uint16_t *)&data_ip->Cabecalho_ip, data_ip->Cabecalho_ip.comp_cabec);
}

void envia_pacote(ip *pacote, int *sock, struct sockaddr_in *host)
{
    //conectar com o cliente e enviar o pacote usando o protocolo selecionado
    struct sockaddr_in server;
    int length = sizeof(struct sockaddr_in);
    getsockname(sock[0], (struct sockaddr *)&server, &length);

    ip *tamanho;
    tamanho = cria_data_ip(1);
    preenche_segmento(&(tamanho->dados), 4, (char *)&qttd_bloco, host->sin_port, server.sin_port);
    preenche_ip(tamanho, host->sin_addr.s_addr, TCP_IP);

    if (protocol == UDP)
    {
        envia(tamanho, *sock, host);
        for (int i = 0; i < qttd_bloco; i++)
        {
            envia(&pacote[i], *sock, host);
        }
    }
    else
    {
        envia(tamanho, sock[1]);
        for (int i = 0; i < qttd_bloco; i++)
        {
            envia(&pacote[i], sock[1]);
        }
    }
    destroi_pacote(tamanho);
}

ip *recebe_pacotes(int sock, struct sockaddr_in *host)
{
    ip *pacotes;
    ip *tamanho = cria_data_ip(1);

    int *len;

    if (protocol == UDP)
    {
        recebe(tamanho, sock, host);
        len = (int *)(tamanho->dados.mensagem);
        qttd_bloco = *len;
        pacotes = cria_data_ip(qttd_bloco);
        for (int i = 0; i < qttd_bloco; i++)
        {
            recebe(&pacotes[i], sock, host);
        }
    }
    else
    {
        recebe(tamanho, sock);
        len = (int *)(tamanho->dados.mensagem);
        qttd_bloco = *len;
        pacotes = cria_data_ip(qttd_bloco);
        for (int i = 0; i < qttd_bloco; i++)
        {
            recebe(&pacotes[i], sock);
        }
    }

    destroi_pacote(tamanho);
    return pacotes;
}

void destroi_pacote(ip *pacote)
{
    free(pacote);
}

int *inicializar_comunicacao(int server_client, struct sockaddr_in *server, ...)
{
    int *sock;
    struct sockaddr_in *_client;
    int length = sizeof(struct sockaddr_in);
    //typedef struct sockaddr_in sockaddr_in ;
    va_list ap;
    va_start(ap, server);

    if (server_client == SERVIDOR)
    {
        int recv;
        _client = va_arg(ap, struct sockaddr_in *);
        if (protocol == UDP)
        {
            sock = malloc(4);
            //  printf("inicializando servidor udp");
            *sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (*sock < 0)
            { /*se nao conseguir abrir() o socket*/
                perror("abertura de socket");
                exit(1);
            }
            if (bind(*sock, (struct sockaddr *)server, length) < 0)
            {
                perror("binding datagrama");
                exit(1);
            }

            if (recvfrom(*sock, (char *)&recv, sizeof(int), 0, (struct sockaddr *)_client, &length) < 0)
                perror("Envio da mensagem");

            return sock;
        }
        else
        {
            sock = malloc(8);
            //  printf("inicializando servidor udp");
            sock[0] = socket(AF_INET, SOCK_STREAM, 0);
            if (sock[0] < 0)
            { /*se nao conseguir abrir() o socket*/
                perror("abertura de socket");
                exit(1);
            }
            if (bind(sock[0], (struct sockaddr *)server, length) < 0)
            {
                perror("binding datagrama");
                exit(1);
            }
            if (listen(sock[0], 5) == -1)
                printf("errou");
            sock[1] = accept(sock[0], (struct sockaddr *)_client, &length);
        }
    }
    else
    {
        sock = malloc(4);
        if (protocol == UDP)
        {
            *sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (*sock < 0)
            { /*se nao conseguir abrir o socket*/
                perror("abertura de socket");
                exit(1);
            }
            if (sendto(*sock, (char *)&recv, sizeof(int), 0, (struct sockaddr *)server, length) < 0)
                perror("Envio da mensagem");
        }
        else
        {
            *sock = socket(AF_INET, SOCK_STREAM, 0);
            if (*sock < 0)
            { /*se nao conseguir abrir o socket*/
                perror("abertura de socket");
                exit(1);
            }

            if (connect(*sock, (struct sockaddr *)server, length) < 0)
            {
                perror("connecting stream socket");
                exit(1);
            }
        }
        // return origem_destino;
    }
    va_end(ap);
    return sock;
}

int envia(ip *pacote, int sock, ...)
{
    va_list ap;
    va_start(ap, sock);
    struct sockaddr_in *destino;
    int length = sizeof(struct sockaddr_in);
    if (protocol == UDP)
    {
        destino = va_arg(ap, struct sockaddr_in *);
        if (sendto(sock, (char *)pacote, sizeof(ip), 0, (struct sockaddr *)destino, length) < 0)
        {
            perror("Envio da mensagem");
            va_end(ap);
            return -1;
        }
    }
    else
    {
        if (send(sock, (char *)pacote, sizeof(ip), 0) < 0)
        {
            perror("Envio da mensagem");
            va_end(ap);
            return -1;
        }
    }
    va_end(ap);
    return 0;
}

int recebe(ip *pacote, int sock, ...)
{
    int rcv;
    struct sockaddr_in *destino;
    va_list ap;
    va_start(ap, sock);
    int length = sizeof(struct sockaddr_in);
    pacote->dados.checksum = check((uint16_t *)&(pacote->dados),  sizeof(MensagemTexto));
    if (protocol == UDP)
    {
        destino = va_arg(ap, struct sockaddr_in *);
        if (recvfrom(sock, (char *)pacote, sizeof(ip), 0, (struct sockaddr *)destino, &length) < 0)
        {
            perror("Envio da mensagem");
            va_end(ap);
            return 2;
        }
    }
    else
    {
        if (recv(sock, (char *)pacote, sizeof(ip), 0) <= 0)
        {
            perror("Fim da Conexão");
            va_end(ap);
            return 2;
        }
    }
    va_end(ap);
    if (!(check((uint16_t *)&pacote->dados,  sizeof(MensagemTexto)) + check((uint16_t *)&pacote->Cabecalho_ip, sizeof(Cabecalho))))
        return 0;
    else
        return 1;
}

void _close_sock(int *sock, int server_client)
{
    if (!(protocol == TCP && server_client == SERVIDOR))
    {
        close(*sock);
    }
    else
    {
        close(sock[1]);
        close(sock[0]);
    }
}

int valida_pacotes(ip *pacote, int num_blocos)
{
    if (num_blocos == 1)
        return (check((uint16_t *)&pacote[0].dados, sizeof(MensagemTexto)) + check((uint16_t *)&pacote[0].Cabecalho_ip, sizeof(Cabecalho)));
    else
    {
        return valida_pacotes(&pacote[1], num_blocos - 1) + (check((uint16_t *)&pacote[num_blocos - 1].dados,  sizeof(MensagemTexto)) + check((uint16_t *)&pacote[num_blocos - 1].Cabecalho_ip, sizeof(Cabecalho)));
    }
}