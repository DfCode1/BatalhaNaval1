# Projeto: Batalha Naval com Habilidades Especiais - Linguagem C

Este projeto tem como objetivo:

- Aplicar conceitos de **vetores e matrizes** para representar o tabuleiro de uma Batalha Naval.
- Utilizar **estruturas de repetição aninhadas** para manipular o tabuleiro.
- Aplicar **condicionais** para simular habilidades com áreas de efeito específicas (cone, cruz, octaedro).
- Desenvolver lógica de jogo estratégica com **sistemas de ataque e defesa** baseados em área.

Como criar o projeto no Visual Studio 2022

1. **Abra o Visual Studio 2022.**
2. Vá em `Arquivo` > `Novo` > `Projeto...`.
3. Selecione a opção **"Aplicativo de Console (C)"**.
4. Dê um nome ao projeto: `BatalhaNaval`.
5. Escolha um local para salvar e clique em `Criar`.
6. No arquivo `main.c`, insira o código-fonte do jogo.
7. Pressione `Ctrl + S` para salvar.

Como compilar e executar

1. Com o projeto aberto, clique em `Compilar > Compilar Solução` (ou `Ctrl + Shift + B`).
2. Após a compilação bem-sucedida, clique em `Depurar > Iniciar sem Depuração` (ou `Ctrl + F5`) para rodar o jogo.
3. Interaja com o jogo pelo console conforme as instruções fornecidas na execução.

Resumo Técnico

- **Linguagem**: C
- **IDE**: Visual Studio 2022
- **Estruturas utilizadas**:
  - Matrizes 2D (`char tabuleiro[10][10]`)
  - Loops aninhados `for`
  - Condicionais `if/else` para aplicar habilidades
- **Habilidades implementadas**:
  - **Cruz**: Ataca em formato de cruz a partir de um ponto central.
  - **Cone**: Ataca posições à frente da coordenada escolhida.
  - **Octaedro**: Ataca em formato de diamante (quatro direções diagonais).

👨‍💻 Ricardo Augusto

Desenvolvido como parte de um projeto acadêmico para aplicar lógica de programação e estruturas em C. 


