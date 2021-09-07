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

MensagemTexto *cria_pacote(int num_pacotes); // Função tipo Mensagem texto, retorna um endereço com num_pacotes alocados

uint16_t check(uint16_t * pacote, int size); // Função de Checksum

long int findSize(char file_name[]); // Retorna tamanho do arquivo

void destroi_pacote(MensagemTexto *pacote); //Libera da memoria pacotes alocados

void preenche_pacote(MensagemTexto *pacote, int tam, char *mensagem, unsigned int IdEnvio, unsigned int IdRecebe); // Preenche pacotes

void consulta_pacote(MensagemTexto pacote); //imprime os campos do pacote

void envia_pacote(MensagemTexto *pacote, int protocolo); //Envia pacotes, recebe ponterio para pacotes e variavel do tipo de protocolo (TCP =1) (UDp =0)

MensagemTexto * recebe(int protocolo); //Retorna endereço para pacotes recebidos. Parametro: tipo de protocolo

arq_fragmentos * fragmenta_arq(char *nome_arquivo, int bits); // Fragementa arquivos em N bits, de acordo com parametro

void close_socket(int sock); // Fecha socket (Não em uso)

int reconstroi_pacote(MensagemTexto *pacote); //Reconstroi pacotes no hd e retorna tamanho do arquivo
int valida_pacotes(MensagemTexto *pacotes); // Valida pelo campo checksum se todos os pacotes estao válidos
#endif