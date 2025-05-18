#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Para geração de números aleatórios (opcional para posicionamento de navios)
#include <ctype.h> // Para toupper()

// Constantes do jogo
#define TAMANHO_TABULEIRO 10
#define AGUA '~'
#define NAVIO 'N'
#define ACERTO 'X'
#define ERRO 'O'
#define TIRO_ESPECIAL_ATINGIDO 'S' // Marca para área de efeito que atingiu um navio
#define TIRO_ESPECIAL_AGUA 'A'     // Marca para área de efeito que atingiu água

// Protótipos das funções
void inicializar_tabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void exibir_tabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_inimigo_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int mostrar_navios_inimigo);
void posicionar_navios_jogador(char tabuleiro_jogador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void posicionar_navios_computador(char tabuleiro_computador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]); // Simplificado
int realizar_jogada(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna);
int verificar_vitoria(char tabuleiro_inimigo[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void habilidade_cone(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int direcao);
void habilidade_cruz(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna);
void habilidade_octaedro(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna);
void limpar_marcas_especiais(char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);

int main() {
    char tabuleiro_jogador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    char tabuleiro_computador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    char tabuleiro_computador_visivel_para_jogador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]; // O que o jogador vê do inimigo
    char tabuleiro_jogador_visivel_para_computador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]; // O que o computador vê do jogador (para IA futura)

    int linha, coluna;
    int jogada_valida;
    int jogo_terminou = 0;
    int turno_jogador = 1; // 1 para jogador, 0 para computador
    int navios_jogador_afundados = 0;
    int navios_computador_afundados = 0;
    int opcao_habilidade;
    int direcao_cone;

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Inicializa os tabuleiros
    inicializar_tabuleiro(tabuleiro_jogador);
    inicializar_tabuleiro(tabuleiro_computador);
    inicializar_tabuleiro(tabuleiro_computador_visivel_para_jogador);
    inicializar_tabuleiro(tabuleiro_jogador_visivel_para_computador);

    printf("Bem-vindo ao Batalha Naval da Oceanic Games!\n");

    // Posicionar navios do jogador
    printf("\n--- Posicione seus navios ---\n");
    posicionar_navios_jogador(tabuleiro_jogador);

    // Posicionar navios do computador (de forma simplificada e aleatória)
    posicionar_navios_computador(tabuleiro_computador);
    printf("\nNavios do computador posicionados secretamente.\n");

    // Loop principal do jogo
    while (!jogo_terminou) {
        if (turno_jogador) {
            printf("\n--- Seu Turno ---\n");
            printf("Seu Tabuleiro:\n");
            exibir_tabuleiro(tabuleiro_jogador, tabuleiro_jogador_visivel_para_computador, 1); // Mostrar seus próprios navios
            printf("\nTabuleiro do Inimigo (Seus Tiros):\n");
            exibir_tabuleiro(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, 0); // Não mostrar navios inimigos

            limpar_marcas_especiais(tabuleiro_computador_visivel_para_jogador); // Limpa 'S' e 'A' antes da nova jogada

            do {
                printf("\nEscolha sua acao:\n");
                printf("1. Tiro Normal\n");
                printf("2. Habilidade: Cone\n");
                printf("3. Habilidade: Cruz\n");
                printf("4. Habilidade: Octaedro\n");
                printf("Opcao: ");
                scanf("%d", &opcao_habilidade);

                printf("Digite a linha para atacar (0-%d): ", TAMANHO_TABULEIRO - 1);
                scanf("%d", &linha);
                printf("Digite a coluna para atacar (0-%d): ", TAMANHO_TABULEIRO - 1);
                scanf("%d", &coluna);

                jogada_valida = (linha >= 0 && linha < TAMANHO_TABULEIRO && coluna >= 0 && coluna < TAMANHO_TABULEIRO);

                if (!jogada_valida) {
                    printf("Coordenadas invalidas! Tente novamente.\n");
                }
                else if (tabuleiro_computador_visivel_para_jogador[linha][coluna] != AGUA &&
                    tabuleiro_computador_visivel_para_jogador[linha][coluna] != NAVIO && // NAVIO aqui é para o caso de estar testando e mostrando
                    tabuleiro_computador_visivel_para_jogador[linha][coluna] != TIRO_ESPECIAL_AGUA &&
                    tabuleiro_computador_visivel_para_jogador[linha][coluna] != TIRO_ESPECIAL_ATINGIDO) {
                    printf("Voce ja atirou nessa posicao! Tente novamente.\n");
                    jogada_valida = 0;
                }

            } while (!jogada_valida);

            switch (opcao_habilidade) {
            case 1: // Tiro Normal
                if (realizar_jogada(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, linha, coluna)) {
                    printf("ACERTO!\n");
                    navios_computador_afundados++;
                }
                else {
                    printf("AGUA!\n");
                }
                break;
            case 2: // Habilidade Cone
                printf("Escolha a direcao do cone:\n");
                printf("1. Cima\n2. Baixo\n3. Esquerda\n4. Direita\nDirecao: ");
                scanf("%d", &direcao_cone);
                if (direcao_cone >= 1 && direcao_cone <= 4) {
                    habilidade_cone(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, linha, coluna, direcao_cone);
                }
                else {
                    printf("Direcao invalida para o cone. Tiro normal efetuado no centro.\n");
                    if (realizar_jogada(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, linha, coluna)) {
                        printf("ACERTO no centro!\n");
                    }
                    else {
                        printf("AGUA no centro!\n");
                    }
                }
                break;
            case 3: // Habilidade Cruz
                habilidade_cruz(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, linha, coluna);
                break;
            case 4: // Habilidade Octaedro
                habilidade_octaedro(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, linha, coluna);
                break;
            default:
                printf("Opcao de habilidade invalida. Tiro normal efetuado.\n");
                if (realizar_jogada(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, linha, coluna)) {
                    printf("ACERTO!\n");
                }
                else {
                    printf("AGUA!\n");
                }
                break;
            }

            printf("\nTabuleiro do Inimigo Apos Seu Ataque:\n");
            exibir_tabuleiro(tabuleiro_computador, tabuleiro_computador_visivel_para_jogador, 0); // Mostra o resultado do ataque


            if (verificar_vitoria(tabuleiro_computador)) {
                printf("\nParabens! Voce venceu!\n");
                jogo_terminou = 1;
            }
            else {
                turno_jogador = 0; // Passa o turno para o computador
            }

        }
        else { // Turno do Computador (IA muito simples)
            printf("\n--- Turno do Computador ---\n");
            do {
                linha = rand() % TAMANHO_TABULEIRO;
                coluna = rand() % TAMANHO_TABULEIRO;
                // Computador não vai usar habilidades nesta versão simplificada
            } while (tabuleiro_jogador_visivel_para_computador[linha][coluna] != AGUA &&
                tabuleiro_jogador_visivel_para_computador[linha][coluna] != NAVIO); // NAVIO para teste

            printf("Computador atirou em (%d, %d)...\n", linha, coluna);
            if (realizar_jogada(tabuleiro_jogador, tabuleiro_jogador_visivel_para_computador, linha, coluna)) {
                printf("O computador acertou um de seus navios!\n");
                navios_jogador_afundados++;
            }
            else {
                printf("O computador atirou na agua!\n");
            }

            printf("Seu Tabuleiro Apos Ataque do Computador:\n");
            exibir_tabuleiro(tabuleiro_jogador, tabuleiro_jogador_visivel_para_computador, 1);


            if (verificar_vitoria(tabuleiro_jogador)) {
                printf("\nO computador venceu! Tente novamente.\n");
                jogo_terminou = 1;
            }
            else {
                turno_jogador = 1; // Passa o turno para o jogador
            }
        }
        // Pequena pausa para o jogador ler o resultado
        printf("\nPressione Enter para continuar...");
        getchar(); // Consome o '\n' do scanf anterior
        getchar(); // Espera pelo Enter
        system("cls || clear"); // Limpa a tela (cls para Windows, clear para Linux/Mac)
    }

    printf("\n--- Fim de Jogo ---\n");
    printf("Seu Tabuleiro Final:\n");
    exibir_tabuleiro(tabuleiro_jogador, tabuleiro_jogador_visivel_para_computador, 1);
    printf("\nTabuleiro Final do Computador (Revelado):\n");
    exibir_tabuleiro(tabuleiro_computador, tabuleiro_computador, 1); // Revela todos os navios do computador

    return 0;
}

void inicializar_tabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

void exibir_tabuleiro(char tabuleiro_real[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int mostrar_navios_proprios) {
    printf("  "); // Espaço para os números das colunas
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d ", i); // Número da linha
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (mostrar_navios_proprios) { // Para o tabuleiro do próprio jogador ou o tabuleiro revelado do inimigo
                if (tabuleiro_real[i][j] == NAVIO && (tabuleiro_visivel[i][j] == ACERTO || tabuleiro_visivel[i][j] == TIRO_ESPECIAL_ATINGIDO)) {
                    printf("%c ", ACERTO); // Mostra acerto em navio próprio
                }
                else if (tabuleiro_real[i][j] == NAVIO) {
                    printf("%c ", NAVIO);
                }
                else {
                    printf("%c ", tabuleiro_visivel[i][j]); // Mostra água, erro, ou marcas de tiro especial
                }
            }
            else { // Para o tabuleiro do inimigo durante o jogo
                printf("%c ", tabuleiro_visivel[i][j]); // Mostra apenas o que foi revelado (água, acerto, erro, especial)
            }
        }
        printf("\n");
    }
}

void limpar_marcas_especiais(char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro_visivel[i][j] == TIRO_ESPECIAL_AGUA) {
                tabuleiro_visivel[i][j] = ERRO; // Converte para erro normal após exibição
            }
            else if (tabuleiro_visivel[i][j] == TIRO_ESPECIAL_ATINGIDO) {
                tabuleiro_visivel[i][j] = ACERTO; // Converte para acerto normal após exibição
            }
        }
    }
}


// Função para posicionar navios do jogador (simplificada, sem validação de sobreposição ou tamanho exato dos navios)
// O jogador insere as coordenadas de 5 navios de 1x1.
void posicionar_navios_jogador(char tabuleiro_jogador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int navios_posicionados = 0;
    int linha, coluna;
    printf("Voce deve posicionar 5 navios (tamanho 1x1 cada).\n");
    while (navios_posicionados < 5) {
        printf("Posicionar navio %d:\n", navios_posicionados + 1);
        printf("Digite a linha (0-%d): ", TAMANHO_TABULEIRO - 1);
        scanf("%d", &linha);
        printf("Digite a coluna (0-%d): ", TAMANHO_TABULEIRO - 1);
        scanf("%d", &coluna);

        if (linha >= 0 && linha < TAMANHO_TABULEIRO && coluna >= 0 && coluna < TAMANHO_TABULEIRO) {
            if (tabuleiro_jogador[linha][coluna] == AGUA) {
                tabuleiro_jogador[linha][coluna] = NAVIO;
                navios_posicionados++;
                printf("Navio posicionado em (%d, %d).\n", linha, coluna);
                // Exibe o tabuleiro após cada navio posicionado
                printf("Seu Tabuleiro Atual:\n");
                char temp_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]; // Tabuleiro temporário para exibição
                inicializar_tabuleiro(temp_visivel); // Inicializa com água
                for (int i = 0; i < TAMANHO_TABULEIRO; ++i)
                    for (int j = 0; j < TAMANHO_TABULEIRO; ++j)
                        if (tabuleiro_jogador[i][j] == NAVIO) temp_visivel[i][j] = NAVIO; // Copia apenas navios para visualização
                exibir_tabuleiro(tabuleiro_jogador, temp_visivel, 1);
            }
            else {
                printf("Posicao ja ocupada! Tente novamente.\n");
            }
        }
        else {
            printf("Coordenadas invalidas! Tente novamente.\n");
        }
    }
}

// Função para posicionar navios do computador (aleatoriamente e simplificado)
void posicionar_navios_computador(char tabuleiro_computador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int navios_posicionados = 0;
    int linha, coluna;
    while (navios_posicionados < 5) { // Computador também posiciona 5 navios de 1x1
        linha = rand() % TAMANHO_TABULEIRO;
        coluna = rand() % TAMANHO_TABULEIRO;
        if (tabuleiro_computador[linha][coluna] == AGUA) {
            tabuleiro_computador[linha][coluna] = NAVIO;
            navios_posicionados++;
        }
    }
}

int realizar_jogada(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna) {
    if (linha < 0 || linha >= TAMANHO_TABULEIRO || coluna < 0 || coluna >= TAMANHO_TABULEIRO) {
        return 0; // Fora do tabuleiro, considera erro mas não marca nada
    }

    if (tabuleiro_ataque[linha][coluna] == NAVIO) {
        tabuleiro_ataque[linha][coluna] = ACERTO; // Marca o navio como acertado no tabuleiro real
        tabuleiro_visivel[linha][coluna] = ACERTO;
        return 1; // Acerto
    }
    else if (tabuleiro_ataque[linha][coluna] == AGUA) {
        tabuleiro_visivel[linha][coluna] = ERRO;
        return 0; // Água
    }
    return 0; // Já foi atirado aqui (ou estado inesperado)
}

int verificar_vitoria(char tabuleiro_inimigo[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro_inimigo[i][j] == NAVIO) {
                return 0; // Ainda existem navios não acertados
            }
        }
    }
    return 1; // Todos os navios foram acertados (convertidos para 'X')
}


// Função para aplicar o resultado de um tiro de habilidade especial
void aplicar_tiro_habilidade(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int r, int c) {
    if (r >= 0 && r < TAMANHO_TABULEIRO && c >= 0 && c < TAMANHO_TABULEIRO) {
        if (tabuleiro_visivel[r][c] == AGUA || tabuleiro_visivel[r][c] == NAVIO) { // Só atinge se não foi atingido antes
            if (tabuleiro_ataque[r][c] == NAVIO) {
                tabuleiro_ataque[r][c] = ACERTO; // Importante: marca o acerto no tabuleiro real
                tabuleiro_visivel[r][c] = TIRO_ESPECIAL_ATINGIDO;
                printf("Habilidade especial acertou um navio em (%d, %d)!\n", r, c);
            }
            else if (tabuleiro_ataque[r][c] == AGUA) {
                tabuleiro_visivel[r][c] = TIRO_ESPECIAL_AGUA;
                printf("Habilidade especial atingiu agua em (%d, %d).\n", r, c);
            }
        }
    }
}

void habilidade_cone(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int direcao) {
    printf("Habilidade Cone ativada em (%d, %d) na direcao %d.\n", linha, coluna, direcao);
    // Ponto central
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna);

    // Direção 1: Cima (formato ^)
    //  X
    // XXX
    //XXXXX (o ponto central é o X do meio da segunda linha)
    if (direcao == 1) { // Cima
        // Linha acima do centro
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna);
        // Linha do centro (adjacentes)
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 1);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 1);
        // Linha abaixo do centro
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna - 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna - 1);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna + 1);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna + 2);
    }
    // Direção 2: Baixo (formato v)
    //XXXXX
    // XXX
    //  X
    else if (direcao == 2) { // Baixo
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 1);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 1);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna - 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna - 1);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna + 1);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna + 2);
    }
    // Direção 3: Esquerda (formato <)
    //  X
    // XX
    //XXX <- centro
    // XX
    //  X
    else if (direcao == 3) { // Esquerda
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 1); // Ponto à esquerda do centro
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna - 1); // Diagonal superior esquerda
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna - 1); // Diagonal inferior esquerda
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 2, coluna - 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna - 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna - 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 2, coluna - 2);

    }
    // Direção 4: Direita (formato >)
    //  X
    //  XX
    //centro -> XXX
    //  XX
    //  X
    else if (direcao == 4) { // Direita
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 1); // Ponto à direita do centro
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna + 1); // Diagonal superior direita
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna + 1); // Diagonal inferior direita
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 2, coluna + 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna + 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna + 2);
        aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 2, coluna + 2);
    }
}


void habilidade_cruz(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna) {
    printf("Habilidade Cruz ativada em (%d, %d).\n", linha, coluna);
    // Ponto central
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna);
    // Pontos da cruz (distância 1)
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna); // Cima
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna); // Baixo
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 1); // Esquerda
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 1); // Direita
    // Pontos da cruz (distância 2)
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 2, coluna); // Cima
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 2, coluna); // Baixo
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 2); // Esquerda
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 2); // Direita
}

void habilidade_octaedro(char tabuleiro_ataque[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_visivel[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna) {
    printf("Habilidade Octaedro ativada em (%d, %d).\n", linha, coluna);
    //  X
    // XXX
    //XXXXX (centro é o X do meio da linha do meio)
    // XXX
    //  X
    // Ponto central
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna);

    // Anel interno (distância 1)
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna);     // Cima
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna);     // Baixo
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 1);     // Esquerda
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 1);     // Direita
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna - 1); // Diagonal Superior Esquerda
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 1, coluna + 1); // Diagonal Superior Direita
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna - 1); // Diagonal Inferior Esquerda
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 1, coluna + 1); // Diagonal Inferior Direita

    // Pontas do anel externo (distância 2 nas direções cardinais)
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha - 2, coluna);     // Cima (2)
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha + 2, coluna);     // Baixo (2)
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna - 2);     // Esquerda (2)
    aplicar_tiro_habilidade(tabuleiro_ataque, tabuleiro_visivel, linha, coluna + 2);     // Direita (2)
}