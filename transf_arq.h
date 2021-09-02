#ifndef _TRANSF_ARQ
#define _TRANSF_ARQ
//porta origem, porta de destino, tamanho, checksum - 16b ; dados - 32b
struct arq_fragmentos
{
    char *fragmentos;   //32 -4 ; 16 -2 
};
typedef struct arq_fragmentos arq_fragmentos;



struct MensagemTexto
{
    unsigned short int IdEnvio;  //Porta de origem
    unsigned short int IdRecebe; //porta de destino
    unsigned short int checksum; //checksum
    unsigned short int tam;      //tamanho
    char mensagem[4];            //dados, provavelmente teremos de adicionar o \0 quando for 4
};
typedef struct MensagemTexto MensagemTexto;

MensagemTexto *cria_pacote(int num_pacotes);

void destroi_pacote(MensagemTexto *pacote);

void preenche_pacote(MensagemTexto *pacote, int tam, char *mensagem, unsigned int IdEnvio, unsigned int IdRecebe);

void consulta_pacote(MensagemTexto pacote);

void envia_pacote(MensagemTexto pacote, int protocolo);

arq_fragmentos * fragmenta_arq(char *nome_arquivo, int bits);
#endif