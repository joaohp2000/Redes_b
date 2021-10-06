# Transfêrencia de Arquivos

![GitHub repo size](https://img.shields.io/github/repo-size/joaohp2000/Redes_b)
![GitHub commit activity](https://img.shields.io/github/commit-activity/w/joaohp2000/Redes_b)
![Forks](https://img.shields.io/github/forks/joaohp2000/Redes_b?style=social)
![Likes](https://img.shields.io/github/stars/joaohp2000/Redes_b?style=social)

<img src="https://gyazo.com/296580837e831cb73465d4b27a94d5b7" alt="exemplo imagem">

> Programa feito na linguagem C para a transferência de arquivos usando o procotopo TCP e UDP. Fazendo todas as verificações dos pacotes que foram enviados e recebidos.

## 🔑 Ajustes e melhorias

O projeto ainda está em desenvolvimento e as próximas atualizações serão voltadas nas seguintes tarefas:

- [x] Função que cria um novo pacote.
- [x] Função que destrua um pacote.
- [x] Função que recebe, por parâmetro: o caminho para um arquivo (que neste caso será um txt) e também a uma quantidade máxima de bits a qual esse arquivo será fracionado. Essa função retorna um array ou uma estrutura de dados com o arquivo totalmente fragmentado conforme solicitado.
- [x] Criação de pacotes a serem transferidos, já com conteúdo completo. Usando os fragmentos gerados pela função anterior.
- [x] Criar uma função de envio de pacotes que recebe um pacote e um parâmetro indicando qual protocolo de camada de transporte será usado.
- [x] Criar um outro programa que servirá para receber os pacotes transmitidos.
- [x] Criação de um novo pacote vazio.
- [x] Adicionar os IP's de origem (seu IP) e o de destino.
- [x] Funções que preenche todos os campos, incluindo o payload que tem o conteúdo do pacote da camada superior (camada de transporte).


## 💻 Pré-requisitos

Antes de começar, verifique se você atendeu aos seguintes requisitos:
* Você instalou a versão mais recente do compilador [GCC](https://gcc.gnu.org/).
* Você tem uma máquina Linux ou Windows (usando o WSL). 
* Você leu [File Transfer using TCP Socket in C](https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/).

## 🚀 Instalando: 

Para instalar o projeto, siga estas etapas:

1. Baixe o código fonte do projeto com o seguinte comando:
```
git clone https://github.com/joaohp2000/Redes_b.git
```
2. Descompacte os arquivos usando algum descompressor a sua escolha.
3. Mude para a pasta baixada e descompactada:
```
cd Redes_b
```
4. Abra um outro terminal nessa mesma pasta para facilitar a compilação.

## ☕ Compilando 

É necessário abrir dois terminais para continuar com a execução.
Para compilar, siga estas etapas:

1. No Terminal 1 - compile o servidor juntamente com a biblioteca criada.
```
./compila.sh servidor
```
2. No Terminal 2 - compile o cliente juntamente com a biblioteca criada.
```
./compila.sh cliente
```
3. Execute primeiramente o servidor no Terminal 1 com o seguinte comando:
```
./serv
```
4. Será requisitado um nome do arquivo a ser enviado. (Verifique se o mesmo consta na pasta atual).
5. Execute o cliente no Terminal 2 com o seguinte comando:
```
./cliente
```
6. Feito, agora você já deve estar vendo o conteúdo do arquivo no Terminal 1, juntamente com várias verificações de integridade.
<img src="https://gyazo.com/296580837e831cb73465d4b27a94d5b7" alt="terminal 1">


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
        <img src="https://avatars.githubusercontent.com/u/38138832?v=4.png" width="100px;" alt="Foto do Carlos Henrique"/><br>
        <sub>
          <b>Carlos Henrique</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="#">
        <img src="https://avatars.githubusercontent.com/u/32877842?v=4.png" width="100px;" alt="Foto do Giuliano Sanfins"/><br>
        <sub>
          <b>Giuliano Sanfins</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="#">
        <img src="https://avatars.githubusercontent.com/u/38138807?v=4.png" width="100px;" alt="Foto do João Henrique"/><br>
        <sub>
          <b>João Henrique</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="#">
        <img src="https://avatars.githubusercontent.com/u/50489803?v=4.png" width="100px;" alt="Foto do Matheus Moretti"/><br>
        <sub>
          <b>Matheus Moretti</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

