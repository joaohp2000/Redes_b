![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.001.png)

**PONTIFÍCIA UNIVERSIDADE CATÓLICA DE CAMPINAS** ENGENHARIA DE COMPUTAÇÃO

Redes de Computadores B

Carlos Henrique Vieira Marques – 18720367 Matheus Henrique Moretti - 18082974 Giuliano Marques Sanfins – 17142837

João Henrique Pereira – 18712919

Transferência de arquivo texto

CAMPINAS MARÇO 2020

SUMÁRIO

1. Introdução 3
1. Abrir o Arquivo 4
1. Fragmentação do arquivo 4
1. Criação de Pacotes 7
1. Formação dos pacotes com os fragmentos e envio 7
1. Recebimento dos Pacotes 8
1. Validação e reconstrução do arquivo 11
1. **Introdução**

O trabalho realizado refere-se a utilização de sockets, que se trata de um canal de comunicação, utilizando os protocolos TCP e UDP, com transferência de arquivos fragmentados em pacotes entre processos.

O sistema criado permite que seja escolhido qual dos protocolos, entre TCP ou UDP, deseja-se utilizar para fazer a transferência do arquivo, além disso, deve permitir que se escolha o tamanho, em bits, dos pacotes que serão usados para transferência do arquivo.

2. **Abrir o Arquivo**

Inicialmente é selecionado o arquivo origem e então ele é aberto em modo de leitura com o comando fopen, função que retorna um "apontador" para o arquivo caso consiga abri-lo, e caso contrário é informado erro e o programa é finalizado.

![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.002.png)

3. **Fragmentação do Arquivo**

Após a seleção e abertura do arquivo, calcula-se o tamanho do bloco pegando o número de bits informado anteriormente e o dividindo por oito, e então com essa informação é calculada a quantidade base de blocos que serão usados no pacote de envio, dividindo o tamanho do arquivo pelo dos blocos.

Depois é feita uma verificação caso a divisão anterior tenha resto igual a zero significa que a quantidade de blocos está certa e todos os blocos estarão preenchidos por completo, caso não seja igual a zero significa que será necessário mais um bloco e ele não contemplará todo espaço permitido, com isso é separado um espaço na memória, capaz de receber todas as informações relevantes do pacote (porta de origem, porta de destino, tamanho, checksum e a mensagem), para cada pacote.

Então é lido do arquivo apenas a quantidade necessária de caracteres, e então essa informação é salva junto do tamanho do bloco.

arq\_fragmentos \* fragmenta\_arq(char \*nome\_arquivo, int bits) {

arq\_fragmentos \*blocos;

FILE \*arquivo;

// pegar o nome do arquivo e seu tamanho

int size;![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.003.png)

size = findSize(nome\_arquivo); //tamanho completo do arquivo em

bytes

// Caso ocorra algum erro na abertura do arquivo![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.004.png)

if ((arquivo = fopen(nome\_arquivo, "r")) == NULL)

{

// o programa aborta automaticamente

printf("Erro! Impossivel abrir o arquivo!\n");

exit(1);

}

// 8 bits = 1 byte

int tam\_bloco = bits/8; // o resto seria o numero de bits do ultimo

pacote

qttd\_bloco = size/tam\_bloco;

// blocos->fragmentos = malloc(sizeof(tam\_bloco)); //fragmentos tem![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.005.png)

o tamanho

//alocar dinamicamente tamanho de cada posicao do vetor com o tam do bloco

if(size%tam\_bloco == 0){![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.006.png)

blocos = malloc(sizeof(arq\_fragmentos) \* qttd\_bloco); //eh alocado o tamanho do blocos

//numero de blocos exato com tamanho do arquivo

for(int i=0; i<qttd\_bloco; i++){![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.003.png)

blocos[i].fragmentos = malloc(sizeof(tam\_bloco)); //ler o arquivo com o tamanho em byte com fread fread(blocos[i].fragmentos, 1, (size\_t)tam\_bloco ,

arquivo);

blocos[i].size=tam\_bloco; //printf("%s\n",blocos[i].fragmentos);![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.007.png)

} } else{

//Fazer verificação do ultimo bloco para nao pegar lixo int tam\_ultimo = size%tam\_bloco;

blocos = malloc(sizeof(arq\_fragmentos) \* (qttd\_bloco+1)); //eh alocado o tamanho do blocos

//O ultimo bloco nao sera preenchido completamente qttd\_bloco++;

for(int i=0; i<qttd\_bloco-1; i++){

blocos[i].fragmentos = malloc(sizeof(tam\_bloco)); //ler o arquivo com o tamanho em byte com fread fread(blocos[i].fragmentos, (size\_t)tam\_bloco , 1,

arquivo);

//printf("%s\n",blocos[i].fragmentos); blocos[i].size=tam\_bloco;

}

blocos[qttd\_bloco-1].fragmentos = malloc(sizeof(tam\_ultimo)); fread(blocos[qttd\_bloco-1].fragmentos, (size\_t)tam\_ultimo , 1,

arquivo); blocos[qttd\_bloco-1].size=tam\_ultimo;

}

return blocos;![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.003.png)

}![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.008.png)

4. **Criação de Pacotes**

Logo após a fragmentação do arquivo é feita a criação dos pacotes, em uma função que instância uma variável do tipo struct **MensagemTexto**, que se trata do pacote que será enviado ele contém os seguintes campos os dados da porta de origem, porta de destino, tamanho, checksum e a mensagem, faz a alocação dinâmica da variável e retorna os pacotes criados.

**MensagemTexto \*cria\_pacote(int num\_pacotes)**

**{**

**MensagemTexto \*envio;**

**envio = malloc(sizeof(MensagemTexto) \* num\_pacotes); return envio;**

**}**

5. **Formação dos pacotes com os fragmentos e envio**

O pacote terá os valores das portas de **origem** e **destinos** setados, além de preencher o campo de mensagem com a mensagem fragmentada de cada bloco, também é passada a variável **tam** que representa o tamanho da mensagem fragmentada que cada bloco carrega, ou seja, o número de bytes que cada bloco possui para formar a mensagem, desta forma se o número de **BITS** escolhidos por bloco for 32, teremos blocos com tamanho de mensagem equivalentes a 4 caracteres, visto que cada carácter equivale a 1 byte e consequentemente 8 bits.

void preenche\_pacote(MensagemTexto \*pacote, int tam, char \*mensagem, unsigned int IdEnvio, unsigned int IdRecebe)

{

//preencher os campos do pacote pacote->tam = tam; pacote->IdEnvio = IdEnvio; pacote->IdRecebe = IdRecebe;

memcpy(&(pacote->mensagem), mensagem, tam);![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.005.png)

}

6. **Recebimento dos Pacotes**

A função de recebimento de pacotes tem a opção de receber o pacote via protocolo TCP ou UDP, escolhido previamente. Em seguida, os pacotes são salvos para ser feita a validação do Checksum com intuito de verificar que o conteúdo da mensagem foi entregue corretamente.

MensagemTexto \* recebe(int *protocolo*){

//pacotes armazenados vetor estrutura arq\_fragmentos

int sock,tam;

extern int qttd\_bloco;

struct sockaddr\_in name, server;

struct hostent \*hp, \*gethostbyname();

struct MensagemTexto msg, \*pacotes;

int len = sizeof(msg), length = sizeof(server);

if(!(pacotes = (MensagemTexto \*) malloc(sizeof(MensagemTexto)))){

printf("Erro");

}

if(protocolo == TCP){![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.009.png)

sock=socket(AF\_INET, SOCK\_STREAM, 0); if (sock<0) {

perror("opening stream socket"); exit(1);

}

/\* Associa \*/ ![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.009.png)server.sin\_family=AF\_INET;

hp = gethostbyname("localhost");

if (hp==0) {

fprintf(stderr, "Unknown host\n"); exit(2);

}

bcopy ((char \*)hp->h\_addr, (char \*)&server.sin\_addr, hp->h\_length);

server.sin\_port = htons(1234);

![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.010.png)

if (connect (sock,(struct sockaddr \*)&server,sizeof server)<0) {

perror("connecting stream socket");

exit(1);

}

/\* Conexoes \*/![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.011.png)

do{![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.012.png)

if ((recv(sock, (char \*)&msg, len, 0)) ==0){

perror("Fim da Conexão");

break;

} ![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.009.png)else{

qttd\_bloco++;

if(!(pacotes = (MensagemTexto \*) realloc(pacotes,

(qttd\_bloco)\*sizeof(MensagemTexto)))){ printf("Errouu");

}

preenche\_pacote(&pacotes[qttd\_bloco-1], msg.tam, msg.mensagem, msg.IdEnvio, msg.IdRecebe);

consulta\_pacote(pacotes[qttd\_bloco-1]);

} }while(1); close(sock); return pacotes;

} else{

if(protocolo == UDP){

//Creating socker file descriptor

if ((sock = socket(AF\_INET, SOCK\_DGRAM, 0)) < 0) {

perror("socket creation failed"); exit(EXIT\_FAILURE);

}

//Criação do pacote UDP

server.sin\_family = AF\_INET;

server.sin\_port= htons (1234);

/\* Acessa o DNS \*/![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.011.png)

hp = gethostbyname("localhost");

if (hp==0){

perror("Host nao encontrado"); exit(2);

}

bcopy ((char\*)hp->h\_addr, (char \*)&server.sin\_addr,![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.013.png)

hp->h\_length);

MensagemTexto sinc;

if (sendto (sock, (char \*)&sinc, len, 0, (struct sockaddr \*)&server, length) < 0)

perror("Envio da mensagem");

if ((recvfrom(sock, (char \*)&qttd\_bloco, sizeof(int), ![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.013.png)0, (struct sockaddr \*)&server, &length)) <=0)

perror("Fim da Conexão");

printf("%d\n", qttd\_bloco);

for(int i=0;i<qttd\_bloco;i++){

if ((recvfrom(sock, (char \*)&msg, len, 0, (struct

sockaddr \*)&server, &length)) <=0){

perror("Fim da Conexão"); break;

} ![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.014.png)else{

if(!(pacotes = (MensagemTexto \*) realloc(pacotes, (i+1)\*sizeof(MensagemTexto)))){

printf("Errouu");

}

preenche\_pacote(&pacotes[i], msg.tam, msg.mensagem, msg.IdEnvio, msg.IdRecebe);

consulta\_pacote(pacotes[i]);

}

}

close(sock);

return pacotes;

}

else{

printf("Protocolo não identificado");

}

}

}![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.011.png)

7. **Validação e reconstrução do arquivo**

Antes de o pacote ser enviado é calculado o valor de checksum que considera todos os dados do pacote, esse valor tem como finalidade representar de forma numérica o conteúdo da mensagem para que dessa forma após ser enviado o receptor possa verificar se o conteúdo do pacote sofreu alguma modificação ou perda durante o envio. Suponha que a representação numérica dos dados do pacote equivale a 0111, é então feito o complemento de um deste número tornando 1000, esse valor é o checksum do pacote, após o envio o mesmo cálculo para representação numérica do pacote é realizada, caso o resultado da soma seja 1111 compreende-se que o conteúdo está correto, portanto não sofreu nenhuma perda ou alteração nos dados, validando-o desta forma.

Com os dados validados a reconstrução do arquivo é iniciada, é criado um arquivo e cada mensagem dos pacotes é escrito nele, na ordem correta.

void valida\_pacotes(MensagemTexto \**pacotes*){

unsigned short csum;

int valida=0;

for(int i=0;i<qttd\_bloco;i++){

if((check((uint16\_t \*)&pacotes[i],12))==0 ) {

valida++;

}

else {

printf("Pacote Incorreto");

}

}

//  printf("okkk: %d", valida);

if(qttd\_bloco == valida) printf("Todos os pacotes estão

válidos\n"); }![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.015.png)

void reconstroi\_pacote(MensagemTexto \**pacote*){

FILE \*arquivo;

// pegar o nome do arquivo e seu tamanho ![](img/Aspoose.Words.ad5d3e48-e004-4a0e-ac3c-b571b56e0d41.016.png)printf("%d", qttd\_bloco);

// Caso ocorra algum erro na abertura do arquivo if ((arquivo = fopen("arquivo.txt", "w")) == NULL) {

// o programa aborta automaticamente

printf("Erro! Impossivel abrir o arquivo!\n"); exit(1);

}

for(int i=0; i<qttd\_bloco;i++){

fwrite(pacote[i].mensagem, pacote[i].tam, 1, arquivo); }

fclose(arquivo);
