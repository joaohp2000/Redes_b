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
    MensagemTexto *segmentos;
    arq_fragmentos *blocos;
    ip *pacotes;
    int size_arquivo,aux,tam_bloco = BITS/8; // o resto seria o numero de bits do ultimo segmento
    extern int qttd_bloco;
     
    char str[80];
    printf("digite o nome do arquivo:");
    gets(str);


    blocos = fragmenta_arq(str, BITS);
   // segmentos = cria_segmento(qttd_bloco);  //chamada de criação dos segmentos
    pacotes = cria_data_ip(qttd_bloco);

    for(int i=0; i<qttd_bloco;i++){
        preenche_segmento(&pacotes[i].dados, blocos[i].size, blocos[i].fragmentos, 1234, 0);
        preenche_ip(&pacotes[i], 8888, 9999);
        
       //consulta_segmento(segmentos[i]); 
       //segmentos[i].checksum = csum()
    }
    envia_pacote(pacotes, UDP); //Envia os segmentos 

    destroi_pacote(pacotes);     //libera os segmentos da memoria
   
}



