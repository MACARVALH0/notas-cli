# notas-cli
Uma aplicação de anotações no terminal que valoriza agilidade e discrição. Gerencia notas pessoais com buscas inteligentes por palavras-chave. <!--// usando SQLite como banco de dados embarcado.-->

## Como Utilizar
**1. Busca por Contexto:** Ao iniciar o programa, o usuário insere a palavra-chave que abrirá um contexto específico, podendo:
- Ver a lista de notas escritas sob aquela palavra-chave;
- Realizar operações envolvendo as notas.

**Realizando Operações:** Dentro do contexto de uma determinada palavra-chave, o usuário pode:
- Adicionar uma nova nota;
- Reescrever uma nota existente;
- Deletar uma nota existente;
- Receber ajuda para utilizar as funcionalidades.

## As Operações
**Dentro do contexto de uma palavra-chave**, o usuário pode realizar as seguintes operações:
1. write <-s/-l> <"Conteúdo">
2. rewrite [id] <-s/-l> <"Conteúdo">
3. delete [id]
4. help

### As Flags
Atualmente, existe apenas um tipo de flag sendo utilizado no programa:
> **<SIZE FLAG>**:
>> - `<-s/--short>` ["Conteúdo"]: Indica uma operação realizada dentro do terminal, exige um argumento que traz o conteúdo para escrita/reescrita da nota.
>> - ``<-l/--long>``: Indica uma operação realizada num contexto externo ao terminal. Atualmente, abre o bloco de notas para escrita/reescrita da nota.

## Como Compilar Localmente
Para compilar o programa na sua máquina, basta seguir os seguintes passos:
**1. Clonar o repositório**
No terminal, execute:
`git clone https://github.com/MACARVALH0/notas-cli.git`
(É necessário ter a ferramenta de versionamento **`git`** instalada localmente.)

**2. Executar montador:**
Caso utilize a ferramenta de compilação `mingw`, essa etapa é mais simples, porém o comando varia de acordo com a versão instalada:
No diretório do projeto, execute `make` ou `mingw32-make` no terminal.

**3. Executar a aplicação:**
Para acessar a aplicação, basta executar `build\main` no terminal.

## Download do Programa Para Uso
Uma versão pré-compilada do programa deve estar disponível na aba `releases` do repositório no GitHub.

## Como Contribuir
Para **contribuir** com o projeto, você pode seguir estas etapas:
1. Faça fork do projeto;
2. Crie sua branch (git checkout -b feature/nova-func);
3. Faça commit de suas mudanças (git commit -m 'Add nova funcionalidade');
4. Dê um push para a branch (git push origin feature/nova-func);
5. Abra um Pull Request.

## Licença
Distribuído sob licença MIT. Veja `LICENSE` para detalhes.

---

Desenvolvido com um carinho especial por Matheus Carvalho.