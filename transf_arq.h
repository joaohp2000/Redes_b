#ifndef _TRANSF_ARQ
#define _TRANSF_ARQ
//porta origem, porta de destino, tamanho, checksum - 16b ; dados - 32b

#include <stdint.h>
#define UDP 0
#define TCP 1

struct arq_fragmentos
{
    int size;
    char *fragmentos;   //32 -4 ; 16 -2 
    
};
typedef struct arq_fragmentos arq_fragmentos;

extern int qttd_bloco;



struct MensagemTexto
{
    unsigned short int IdEnvio;  //Porta de origem
    unsigned short int IdRecebe; //porta de destino
    unsigned short int tam;      //tamanho
    unsigned short int checksum; //checksum
    char mensagem[4];            //dados, provavelmente teremos de adicionar o \0 quando for 4
};
typedef struct MensagemTexto MensagemTexto;

MensagemTexto *cria_pacote(int num_pacotes);

uint16_t check(uint16_t * pacote, int size);

long int findSize(char file_name[]);

void destroi_pacote(MensagemTexto *pacote);

void preenche_pacote(MensagemTexto *pacote, int tam, char *mensagem, unsigned int IdEnvio, unsigned int IdRecebe);

void consulta_pacote(MensagemTexto pacote);

void envia_pacote(MensagemTexto *pacote, int protocolo);

MensagemTexto * recebe(int protocolo);

arq_fragmentos * fragmenta_arq(char *nome_arquivo, int bits);

void close_socket(int sock);

int reconstroi_pacote(MensagemTexto *pacote);
void valida_pacotes(MensagemTexto *pacotes);
#endif