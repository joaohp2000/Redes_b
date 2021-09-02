#include "transf_arq.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

/*
//----- Type defines ----------------------------------------------------------
typedef unsigned char      byte;    // Byte is a char
typedef unsigned short int word16;  // 16-bit word is a short int
typedef unsigned int       word32;  // 32-bit word is an int

//----- Defines ---------------------------------------------------------------
#define BUFFER_LEN        4096      // Length of buffer


static unsigned short int csum(uint16_t *usBuff, int isize);

word16 checksum(byte *addr, word32 count)
{
  register word32 sum = 0;

  // Main summing loop
  while(count > 1)
  {
    sum = sum + *(addr)++; //minha luz ta caindo - passando mensagem por pacote
    count = count - 2;
  }

  // Add left-over byte, if any
  if (count > 0)
    sum = sum + *((byte *) addr);

  // Fold 32-bit sum to 16 bits
  while (sum>>16)
    sum = (sum & 0xFFFF) + (sum >> 16);

  return(~sum);
}
*/
MensagemTexto *cria_pacote(int num_pacotes)
{
    //alocar
    MensagemTexto *envio;
    envio = malloc(sizeof(MensagemTexto) * num_pacotes);
    return envio;
}

static unsigned short int csum(uint16_t *usBuff, int isize)
{
    unsigned int cksum = 0;
    for (; isize > 1; isize--)
    {
        cksum += *usBuff++;
    }
    if (isize == 1)
    {
        cksum += *(uint16_t *)usBuff;
    }
    return (cksum);
}

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
   //
   
    memcpy(&(pacote->mensagem), mensagem, tam);
    //pacote->checksum = csum(pacote->mensagem, 4);
    //adicionar o \0 ao fim da mensagem
    /*for(int i = 0; i<4;i++){
        printf("%c",pacote->mensagem[i]);
    }*/
    //printf("checksum = %04X \n", pacote->checksum);
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
    printf("\n------------------------------------\n");
}

