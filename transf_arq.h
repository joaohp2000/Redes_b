#ifndef _TRANSF_ARQ
#define _TRANSF_ARQ
//porta origem, porta de destino, tamanho, checksum - 16b ; dados - 32b

#include <stdint.h>
#include <sys/types.h>
#include <netinet/in.h>

#define UDP 0
#define TCP 1
#define TCP_IP 6
#define TCP_IP 6
#define UDP_IP 7
#define SERVIDOR 1
#define CLIENTE 0

struct arq_fragmentos
{
    int size;
    char *fragmentos;   //32 -4 ; 16 -2 
    
};
typedef struct arq_fragmentos arq_fragmentos;

extern int qttd_bloco;



extern uint8_t protocol;
// campo de dado recebe a struct do cabeçalho
struct MensagemTexto
{
    unsigned short int IdEnvio;  //Porta de origem
    unsigned short int IdRecebe; //porta de destino
    unsigned short int tam;      //tamanho
    unsigned short int checksum; //checksum
    char mensagem[4];            //dados, provavelmente teremos de adicionar o \0 quando for 4
};
typedef struct MensagemTexto MensagemTexto;

struct Cabecalho
{
    uint8_t versao;             //Versão - 4
    uint8_t comp_cabec;         //Comprimento do cabeçalho - 4   
    uint8_t tipo_serv;          //Tipo de serviço - 8
    uint16_t comp_datagrama;    //comprimento do datagrama - 16
    uint16_t identificador;     //Identificador de 16 bit - 16
    uint8_t flags;              //Flags - 3 
    uint16_t desloc_frag;       //Deslocamento de fragmentação - 13
    uint8_t tempo_vida;         //Tempo de vida - 8
    uint8_t protoc_super;       //Protocolo da camada superior - 8
    uint16_t checksum;          //Soma de verificação do cabeçalho -16
    uint32_t end_ip_org;        //Endereço IP da origem - 32
    uint32_t end_ip_dest;       //Endereço IP do destino - 32
    uint32_t opcoes;            //Opçoes (se houver) - 32
};
typedef struct Cabecalho Cabecalho;


struct ip
{
    Cabecalho Cabecalho_ip;
    MensagemTexto dados;
};
typedef struct ip ip;

MensagemTexto *cria_segmento(int num_segmentos);
 // Função tipo Mensagem texto, retorna um endereço com num_segmentos alocados

ip * cria_data_ip();

void envia_pacote(ip * pacote, int * sock, struct sockaddr_in *host);
 
ip * recebe_pacotes(int sock, struct sockaddr_in *host);

void destroi_pacote(ip *pacote);

void preenche_ip(ip *data_ip, uint32_t ip_destino, int protocolo_transp);

uint16_t check(uint16_t * _segmento, int size); // Função de Checksum

long int findSize(char file_name[]); // Retorna tamanho do arquivo

void destroi_segmento(MensagemTexto *segmento); //Libera da memoria segmentos alocados

void preenche_segmento(MensagemTexto *segmento, int tam, char *mensagem, unsigned int IdEnvio, unsigned int IdRecebe); // Preenche segmentos

void consulta_segmento(MensagemTexto segmento); //imprime os campos do segmento

void envia_segmento(MensagemTexto *segmento, int protocolo); //Envia segmentos, recebe ponterio para segmentos e variavel do tipo de protocolo (TCP =1) (UDp =0)

MensagemTexto * recebe_segemento(int protocolo); //Retorna endereço para segmentos recebidos. Parametro: tipo de protocolo

arq_fragmentos * fragmenta_arq(char *nome_arquivo, int bits); // Fragementa arquivos em N bits, de acordo com parametro

void close_socket(int sock); // Fecha socket (Não em uso)

int reconstroi_segmento(MensagemTexto *segmento); //Reconstroi segmentos no hd e retorna tamanho do arquivo
int valida_segmentos(MensagemTexto *segmentos); // Valida pelo campo checksum se todos os segmentos estao válidos

int *inicializar_comunicacao(int server_client, struct sockaddr_in *server, ...);

int recebe(ip *pacote, int sock, ...);

int envia(ip *pacote, int sock, ...);

void reconstroi_pacote(ip *pacote);

void _close_sock(int *sock, int server_client);
#endif