#include "transf_arq.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

long int findSize(char file_name[])
{
    // opening the file in read mode
    FILE *fp = fopen(file_name, "r");

    // checking if the file exist or not
    if (fp == NULL)
    {
        printf("File Not Found!\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_END);

    // calculating the size of the file
    long int res = ftell(fp);

    // closing the file
    fclose(fp);

    return res;
}

arq_fragmentos *fragmenta_arq(char *nome_arquivo, int bits)
{
    arq_fragmentos *blocos;
    FILE *arquivo;
    // pegar o nome do arquivo e seu tamanho

    int size;
    size = findSize(nome_arquivo); //tamanho completo do arquivo em bytes

    // Caso ocorra algum erro na abertura do arquivo
    if ((arquivo = fopen(nome_arquivo, "r")) == NULL)
    {
        // o programa aborta automaticamente
        printf("Erro! Impossivel abrir o arquivo!\n");
        exit(1);
    }
    // 8 bits = 1 byte
    int tam_bloco = bits / 8; // o resto seria o numero de bits do ultimo segmento
    qttd_bloco = size / tam_bloco;

    // blocos->fragmentos = malloc(sizeof(tam_bloco)); //fragmentos tem o tamanho
    //alocar dinamicamente tamanho de cada posicao do vetor com o tam do bloco

    if (size % tam_bloco == 0)
    {
        blocos = malloc(sizeof(arq_fragmentos) * qttd_bloco); //eh alocado o tamanho do blocos
        //numero de blocos exato com tamanho do arquivo

        for (int i = 0; i < qttd_bloco; i++)
        {
            blocos[i].fragmentos = malloc(sizeof(tam_bloco));
            //ler o arquivo com o tamanho em byte com fread
            fread(blocos[i].fragmentos, 1, (size_t)tam_bloco, arquivo);
            blocos[i].size = tam_bloco;

            //printf("%s\n",blocos[i].fragmentos);
        }
    }
    else
    {
        //Fazer verificação do ultimo bloco para nao pegar lixo
        int tam_ultimo = size % tam_bloco;
        blocos = malloc(sizeof(arq_fragmentos) * (qttd_bloco + 1)); //eh alocado o tamanho do blocos
        //O ultimo bloco nao sera preenchido completamente
        qttd_bloco++;
        for (int i = 0; i < qttd_bloco - 1; i++)
        {
            blocos[i].fragmentos = malloc(sizeof(tam_bloco));
            //ler o arquivo com o tamanho em byte com fread
            fread(blocos[i].fragmentos, (size_t)tam_bloco, 1, arquivo);
            //printf("%s\n",blocos[i].fragmentos);
            blocos[i].size = tam_bloco;
        }
        blocos[qttd_bloco - 1].fragmentos = malloc(sizeof(tam_ultimo));
        fread(blocos[qttd_bloco - 1].fragmentos, (size_t)tam_ultimo, 1, arquivo);
        blocos[qttd_bloco - 1].size = tam_ultimo;
    }

    return blocos;
}

int reconstroi_segmento(MensagemTexto *segmento)
{
    FILE *arquivo;
    int tam = 0;
    // pegar o nome do arquivo e seu tamanho

    // Caso ocorra algum erro na abertura do arquivo
    if ((arquivo = fopen("arquivo.txt", "w")) == NULL)
    {
        // o programa aborta automaticamente
        printf("Erro! Impossivel abrir o arquivo!\n");
        exit(1);
    }
    for (int i = 0; i < qttd_bloco; i++)
    {
        fwrite(segmento[i].mensagem, segmento[i].tam, 1, arquivo);
        tam += segmento[i].tam;
    }

    fclose(arquivo);
    return tam;
}

void reconstroi_pacote(ip *pacote)
{
    FILE *arquivo;
    int tam = 0;
    char end_of_file = 5;
    // pegar o nome do arquivo e seu tamanho

    // Caso ocorra algum erro na abertura do arquivo
    if ((arquivo = fopen("arquivo.txt", "w")) == NULL)
    {
        // o programa aborta automaticamente
        printf("Erro! Impossivel abrir o arquivo!\n");
        exit(1);
    }
    for (int i = 0; i < qttd_bloco; i++)
    {
        fwrite(pacote[i].dados.mensagem, pacote[i].dados.tam, 1, arquivo);
        tam += pacote[i].dados.tam;
    }
    fwrite(&end_of_file, 1, 1, arquivo);
    end_of_file = -1;
    fwrite(&end_of_file, 1, 1, arquivo);
    fclose(arquivo);
    //return tam;
}
