#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "transf_arq.h"


int main(){
    FILE *arq;
    int size_arquivo;
    char c;
    MensagemTexto *pacotes;
    pacotes = recebe(UDP); // Abre conexão e recebe pacotes
    if(valida_pacotes(pacotes)){ // Valida pelo campo do checksum se os pacotes estão corretos
        size_arquivo=reconstroi_pacote(pacotes); //Reconstroi arquivo no hd
        if ((arq = fopen("arquivo.txt", "r")) == NULL)
        {
            // o programa aborta automaticamente
            printf("Erro! Impossivel abrir o arquivo!\n");
            exit(1);
        }

        printf("Conteudo do Arquivo:");
        for(int i=0;i<size_arquivo;i++){
            fread(&c,1,1,arq);
            fwrite(&c,1,1,stdout);      //printa conteudo do arquivo
        }
        printf("\n");
        } 
    else{
        printf("Não é Possivél reconstruir o arquivo!");
    }
    
    destroi_pacote(pacotes);     //libera os pacotes da memoria

    
    return 0;
}
