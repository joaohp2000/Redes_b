# Transfêrencia de Arquivos
<!---Esses são exemplos. Veja https://shields.io para outras pessoas ou para personalizar este conjunto de escudos. Você pode querer incluir dependências, status do projeto e informações de licença aqui--->

![GitHub repo size](https://img.shields.io/github/repo-size/joaohp2000/Redes_b)
![GitHub commit activity](https://img.shields.io/github/commit-activity/w/joaohp2000/Redes_b)


<img src="https://gyazo.com/46c315e6ec0f960b911dd8b5e6696e7f.png" alt="exemplo imagem">

> Programa feito na linguagem C para a transferência de arquivos usando o procotopo TCP e UDP. Fazendo todas as verificações dos pacotes que foram enviados e recebidos.

### Ajustes e melhorias

O projeto ainda está em desenvolvimento e as próximas atualizações serão voltadas nas seguintes tarefas:

- [x] Função que cria um novo pacote
- [x] Função que destrua um pacote
- [x] Função que recebe, por parâmetro: o caminho para um arquivo (que neste caso será um txt) e também a uma quantidade máxima de bits a qual esse arquivo será fracionado. Essa função retorna um array ou uma estrutura de dados com o arquivo totalmente fragmentado conforme solicitado.
- [x] Criação de pacotes a serem transferidos, já com conteúdo completo. Usando os fragmentos gerados pela função anterior.
- [x] Criar uma função de envio de pacotes que recebe um pacote e um parâmetro indicando qual protocolo de camada de transporte será usado.
- [x] Criar um outro programa que servirá para receber os pacotes transmitidos.


## 💻 Pré-requisitos

Antes de começar, verifique se você atendeu aos seguintes requisitos:
<!---Estes são apenas requisitos de exemplo. Adicionar, duplicar ou remover conforme necessário--->
* Você instalou a versão mais recente do compilador [GCC](https://gcc.gnu.org/) `<linguagem / dependência / requeridos>`
* Você tem uma máquina Linux ou Windows (usando o WSL) `<Windows / Linux / Mac>`. 
* Você leu [File Transfer using TCP Socket in C](https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/)

## 🚀 Instalando 

Para instalar o projeto, siga estas etapas:

1. Baixe o código fonte do projeto com o seguinte comando 
```
git clone https://github.com/joaohp2000/Redes_b.git
```
2. Descompacte os arquivos usando algum descompressor a sua escolha
3. Mude para a pasta baixada e descompactada
```
cd Redes_b
```
4. Abra um outro terminal nessa mesma pasta para facilitar a compilação

## ☕ Compilando 

É necessário abrir dois terminais para continuar com a instalação.
Para compilar, siga estas etapas:

1. No Terminal 1 - compile o servidor juntamente com a biblioteca criada.
```
gcc serv.c fragmentos.c pacotes.c -o serv
```
2. No Terminal 2 - compile o cliente juntamente com a biblioteca criada.
```
gcc cliente.c fragmentos.c pacotes.c -o cliente
```
3. Execute primeiramente o servidor no Terminal 1 com o seguinte comando:
```
./serv
```
4. Será requisitado um nome do arquivo a ser compartilhado, o informe.
5. Compile o cliente no Terminal 2 com o seguinte comando:
```
./cliente
```
6. Feito, agora você já deve estar vendo o conteúdo do arquivo no Terminal 1, juntamente com várias verificações de integridade.
<img src="https://gyazo.com/1b041ea8c81554cf53e64c096c85cf87.png" alt="terminal 1">


## 📫 Contribuindo para o nosso projeto
<!---Se o seu README for longo ou se você tiver algum processo ou etapas específicas que deseja que os contribuidores sigam, considere a criação de um arquivo CONTRIBUTING.md separado--->
Para contribuir com o projeto, siga estas etapas:

1. Bifurque este repositório.
2. Crie um branch: `git checkout -b <nome_branch>`.
3. Faça suas alterações e confirme-as: `git commit -m '<mensagem_commit>'`
4. Envie para o branch original: `git push origin <nome_do_projeto> / <local>`
5. Crie a solicitação de pull.

Como alternativa, consulte a documentação do GitHub em [como criar uma solicitação pull](https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request).

## 🤝 Colaboradores

Agradecemos às seguintes pessoas que contribuíram para este projeto:

<table>
  <tr>
    <td align="center">
      <a href="#">
        <img src="https://avatars3.githubusercontent.com/u/31936044" width="100px;" alt="Foto do Iuri Silva no GitHub"/><br>
        <sub>
          <b>Iuri Silva</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="#">
        <img src="https://s2.glbimg.com/FUcw2usZfSTL6yCCGj3L3v3SpJ8=/smart/e.glbimg.com/og/ed/f/original/2019/04/25/zuckerberg_podcast.jpg" width="100px;" alt="Foto do Mark Zuckerberg"/><br>
        <sub>
          <b>Mark Zuckerberg</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="#">
        <img src="https://miro.medium.com/max/360/0*1SkS3mSorArvY9kS.jpg" width="100px;" alt="Foto do Steve Jobs"/><br>
        <sub>
          <b>Steve Jobs</b>
        </sub>
      </a>
    </td>
  </tr>
</table>


## 😄 Seja um dos contribuidores<br>

Quer fazer parte desse projeto? Clique [AQUI](CONTRIBUTING.md) e leia como contribuir.



[⬆ Voltar ao topo](#nome-do-projeto)<br>
