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
    MensagemTexto *pacotes;
    arq_fragmentos *blocos;
   
    int size_arquivo,aux,tam_bloco = BITS/8; // o resto seria o numero de bits do ultimo pacote
    extern int qttd_bloco;
     
    char str[80];
    printf("digite o nome do arquivo:");
    gets(str);


    blocos = fragmenta_arq(str, BITS);
    pacotes = cria_pacote(qttd_bloco);  //chamada de criação dos pacotes
    
    for(int i=0; i<qttd_bloco;i++){
        preenche_pacote(&pacotes[i], blocos[i].size, blocos[i].fragmentos, 1234, 0);
       // consulta_pacote(pacotes[i]); 
       //pacotes[i].checksum = csum()
    }
    envia_pacote(pacotes, TCP); //Envia os pacotes 

    destroi_pacote(pacotes);     //libera os pacotes da memoria
   
}

/*
    Generic checksum calculation function
*/

