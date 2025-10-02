#include <stdio.h>

// Retornos da função jogarNavio
#define JOGAR_OK 0
#define JOGAR_FORA_LIMITE 1
#define JOGAR_SOBREPOSICAO 2

// Imprime o tabuleiro 10x10.
// Prioridade visual: se efeito[r][c] == 1 => imprime 5 (área de habilidade).
// Caso contrário imprime valor do tabuleiro (0 = água, 3 = navio).
void imprimirTabuleiro (int tabuleiro[10][10], int efeito[10][10]) {
    printf("Imprimindo tabuleiro...\n\n");

    // Cabeçalho com letras A-J
    printf("     ");
    for (int j = 0; j < 10; j++) {
        printf(" %c  ", 'A' + j);
    }
    printf("\n");

    // Linhas numeradas e conteúdo
    for (int i = 0; i < 10; i++) {
        printf("%02d- ", i + 1);
        for (int j = 0; j < 10; j++) {
            int mostra;
            if (efeito[i][j] == 1) {           // área de habilidade tem prioridade visual
                mostra = 5;
            } else {
                mostra = tabuleiro[i][j];      // 0 água, 3 navio
            }
            printf(" %2d ", mostra);
        }
        printf("\n");
    }
    printf("\n");
}

/*
  jogarNavio:
  orientacao:
    0 = horizontal  (linha, coluna+i)
    1 = vertical    (linha+i, coluna)
    2 = diag ↘      (linha+i, coluna+i)
    3 = diag ↙      (linha+i, coluna-i)

  Retorna:
    JOGAR_OK (0) se colocado com sucesso
    JOGAR_FORA_LIMITE (1) se o navio sair do tabuleiro
    JOGAR_SOBREPOSICAO (2) se colidir com posição já ocupada
*/
int jogarNavio(int tabuleiro[10][10], int linha, int coluna, int tamanho, int orientacao) {
    // Checar limites dependendo da orientação
    if (orientacao == 0) { // horizontal
        if (coluna < 0 || coluna + tamanho - 1 > 9 || linha < 0 || linha > 9) return JOGAR_FORA_LIMITE;
    } else if (orientacao == 1) { // vertical
        if (linha < 0 || linha + tamanho - 1 > 9 || coluna < 0 || coluna > 9) return JOGAR_FORA_LIMITE;
    } else if (orientacao == 2) { // diagonal down-right
        if (linha < 0 || linha + tamanho - 1 > 9 || coluna < 0 || coluna + tamanho - 1 > 9) return JOGAR_FORA_LIMITE;
    } else if (orientacao == 3) { // diagonal down-left
        if (linha < 0 || linha + tamanho - 1 > 9 || coluna - (tamanho - 1) < 0 || coluna > 9) return JOGAR_FORA_LIMITE;
    } else {
        return JOGAR_FORA_LIMITE; // orientação inválida tratada como fora do limite
    }

    // Verificar sobreposição antes de escrever
    for (int i = 0; i < tamanho; i++) {
        int r = linha, c = coluna;
        if (orientacao == 0) { r = linha;           c = coluna + i; }
        if (orientacao == 1) { r = linha + i;       c = coluna; }
        if (orientacao == 2) { r = linha + i;       c = coluna + i; }
        if (orientacao == 3) { r = linha + i;       c = coluna - i; }

        if (tabuleiro[r][c] != 0) {
            return JOGAR_SOBREPOSICAO;
        }
    }

    // Preencher com 3 (navio) após checagens
    for (int i = 0; i < tamanho; i++) {
        int r = linha, c = coluna;
        if (orientacao == 0) { r = linha;           c = coluna + i; }
        if (orientacao == 1) { r = linha + i;       c = coluna; }
        if (orientacao == 2) { r = linha + i;       c = coluna + i; }
        if (orientacao == 3) { r = linha + i;       c = coluna - i; }

        tabuleiro[r][c] = 3;
    }

    return JOGAR_OK;
}

/* =========================
   Construção das matrizes de habilidade (tamanho fixo 5x5)
   - Cada função preenche m[][] com 0/1 usando loops aninhados + condicionais
   - Você pode alterar N_HABILIDADE para 7 se quiser matriz maior
   ========================= */
#define N_HABILIDADE 5

void construirCone(int m[N_HABILIDADE][N_HABILIDADE]) {
    int centro = N_HABILIDADE / 2;
    // Inicializa com 0
    for (int i = 0; i < N_HABILIDADE; i++)
        for (int j = 0; j < N_HABILIDADE; j++)
            m[i][j] = 0;

    // Cone apontando para baixo: linha 0 tem largura 1 centrada, linha 1 largura 3, linha 2 largura 5 etc
    for (int i = 0; i < N_HABILIDADE; i++) {
        // largura desejada na linha i (cresce com i): 1,3,5,... limitada por N_HABILIDADE
        int largura = 1 + 2 * i;
        if (largura > N_HABILIDADE) largura = N_HABILIDADE;
        int meio = centro;
        int inicio = meio - (largura / 2);
        int fim = inicio + largura - 1;
        for (int j = inicio; j <= fim; j++) {
            if (j >= 0 && j < N_HABILIDADE) m[i][j] = 1;
        }
    }
}

void construirCruz(int m[N_HABILIDADE][N_HABILIDADE]) {
    int centro = N_HABILIDADE / 2;
    for (int i = 0; i < N_HABILIDADE; i++)
        for (int j = 0; j < N_HABILIDADE; j++)
            m[i][j] = 0;

    // Cruz: linha do centro inteira e coluna do centro inteira
    for (int k = 0; k < N_HABILIDADE; k++) {
        m[centro][k] = 1;
        m[k][centro] = 1;
    }
}

void construirOctaedro(int m[N_HABILIDADE][N_HABILIDADE]) {
    int centro = N_HABILIDADE / 2;
    for (int i = 0; i < N_HABILIDADE; i++)
        for (int j = 0; j < N_HABILIDADE; j++)
            m[i][j] = 0;

    // Losango (vista frontal do "octaedro"): abs(i-centro) + abs(j-centro) <= centro
    for (int i = 0; i < N_HABILIDADE; i++) {
        for (int j = 0; j < N_HABILIDADE; j++) {
            if ( ( (i - centro) < 0 ? -(i-centro) : (i-centro) )
               + ( (j - centro) < 0 ? -(j-centro) : (j-centro) ) <= centro ) {
                m[i][j] = 1;
            }
        }
    }
}

/*
  Aplica a matriz de habilidade (m) sobre a camada de efeito "efeito[][]".
  origemLinha/origemColuna são as coordenadas no tabuleiro que correspondem ao centro da matriz m.
  A matriz m tem tamanho N_HABILIDADE x N_HABILIDADE; a célula m[i][j]==1 marca efeito[r][c]=1,
  respeitando os limites do tabuleiro (checagem de bounds).
*/
void aplicarHabilidadeNoEfeito(int efeito[10][10], int m[N_HABILIDADE][N_HABILIDADE], int origemLinha, int origemColuna) {
    int centro = N_HABILIDADE / 2;
    for (int i = 0; i < N_HABILIDADE; i++) {
        for (int j = 0; j < N_HABILIDADE; j++) {
            if (m[i][j] == 1) {
                int r = origemLinha - centro + i;
                int c = origemColuna - centro + j;
                if (r >= 0 && r < 10 && c >= 0 && c < 10) {
                    efeito[r][c] = 1; // marca área afetada
                }
            }
        }
    }
}

int main() {
    int tabuleiro[10][10] = {0};
    int efeito[10][10] = {0}; // camada que guarda áreas de habilidade (0/1)
    const int tamanhoNavio = 3;

    int linha, coluna, status;

    // Imprime tabuleiro vazio
    imprimirTabuleiro(tabuleiro, efeito);

    // --- Navio 1: Horizontal (orientacao 0) ---
    do {
        printf("Navio 1 (Horizontal) - informe coordenadas de inicio\n");
        printf("Linha (0-9): ");
        scanf("%d", &linha);
        printf("Coluna (0-%d): ", 10 - tamanhoNavio);
        scanf("%d", &coluna);

        int maxCol = 10 - tamanhoNavio;
        if (coluna < 0) coluna = -1;

        if (linha < 0 || linha > 9 || coluna < 0 || coluna > maxCol) {
            printf("Coordenadas inválidas para navio horizontal. Tente novamente.\n\n");
            status = JOGAR_FORA_LIMITE;
            continue;
        }

        status = jogarNavio(tabuleiro, linha, coluna, tamanhoNavio, 0);
        if (status == JOGAR_SOBREPOSICAO) {
            printf("Sobreposição detectada. Escolha outra posição.\n\n");
        }
    } while (status != JOGAR_OK);

    imprimirTabuleiro(tabuleiro, efeito);

    // --- Navio 2: Vertical (orientacao 1) ---
    do {
        printf("Navio 2 (Vertical) - informe coordenadas de inicio\n");
        printf("Linha (0-%d): ", 10 - tamanhoNavio);
        scanf("%d", &linha);
        printf("Coluna (0-9): ");
        scanf("%d", &coluna);

        int maxLin = 10 - tamanhoNavio;
        if (linha < 0) linha = -1;

        if (linha < 0 || linha > maxLin || coluna < 0 || coluna > 9) {
            printf("Coordenadas inválidas para navio vertical. Tente novamente.\n\n");
            status = JOGAR_FORA_LIMITE;
            continue;
        }

        status = jogarNavio(tabuleiro, linha, coluna, tamanhoNavio, 1);
        if (status == JOGAR_SOBREPOSICAO) {
            printf("Sobreposição detectada. Escolha outra posição.\n\n");
        }
    } while (status != JOGAR_OK);

    imprimirTabuleiro(tabuleiro, efeito);

    // --- Navio 3: Diagonal down-right (orientacao 2) ---
    do {
        printf("Navio 3 (Diagonal ↘) - informe coordenadas de inicio (parte superior-esquerda do navio)\n");
        printf("Linha (0-%d): ", 10 - tamanhoNavio);
        scanf("%d", &linha);
        printf("Coluna (0-%d): ", 10 - tamanhoNavio);
        scanf("%d", &coluna);

        int maxStart = 10 - tamanhoNavio;
        if (linha < 0) linha = -1;
        if (coluna < 0) coluna = -1;

        if (linha < 0 || linha > maxStart || coluna < 0 || coluna > maxStart) {
            printf("Coordenadas inválidas para diagonal ↘. Tente novamente.\n\n");
            status = JOGAR_FORA_LIMITE;
            continue;
        }

        status = jogarNavio(tabuleiro, linha, coluna, tamanhoNavio, 2);
        if (status == JOGAR_SOBREPOSICAO) {
            printf("Sobreposição detectada. Escolha outra posição.\n\n");
        }
    } while (status != JOGAR_OK);

    imprimirTabuleiro(tabuleiro, efeito);

    // --- Navio 4: Diagonal down-left (orientacao 3) ---
    do {
        printf("Navio 4 (Diagonal ↙) - informe coordenadas de inicio (parte superior-direita do navio)\n");
        printf("Linha (0-%d): ", 10 - tamanhoNavio);
        scanf("%d", &linha);
        printf("Coluna (%d-9): ", tamanhoNavio - 1);
        scanf("%d", &coluna);

        int maxStart = 10 - tamanhoNavio;
        if (linha < 0) linha = -1;

        if (linha < 0 || linha > maxStart || coluna < (tamanhoNavio - 1) || coluna > 9) {
            printf("Coordenadas inválidas para diagonal ↙. Tente novamente.\n\n");
            status = JOGAR_FORA_LIMITE;
            continue;
        }

        status = jogarNavio(tabuleiro, linha, coluna, tamanhoNavio, 3);
        if (status == JOGAR_SOBREPOSICAO) {
            printf("Sobreposição detectada. Escolha outra posição.\n\n");
        }
    } while (status != JOGAR_OK);

    // Tabuleiro com apenas navios por enquanto
    printf("Tabuleiro com navios posicionados (antes das habilidades):\n");
    imprimirTabuleiro(tabuleiro, efeito);

    /* =============================
       Construir as matrizes de habilidade dinamicamente
       (usando loops aninhados e condicionais, conforme pedido)
       ============================= */
    int cone[N_HABILIDADE][N_HABILIDADE];
    int cruz[N_HABILIDADE][N_HABILIDADE];
    int octaedro[N_HABILIDADE][N_HABILIDADE];

    construirCone(cone);
    construirCruz(cruz);
    construirOctaedro(octaedro);

    // Definição das origens (pontos centrais no tabuleiro) fixas no código (simplificação)
    // Escolhi posições que cabem bem: centro-ish e cantos. Você pode alterar para testar.
    int origemConeLinha = 2, origemConeColuna = 2;    // perto do canto superior-esquerdo
    int origemCruzLinha = 5,  origemCruzColuna = 5;   // centro do tabuleiro
    int origemOctLinha = 1,    origemOctCol = 8;      // perto da borda direita

    // Aplica as habilidades na camada de efeito (não sobrescreve tabuleiro de navios)
    aplicarHabilidadeNoEfeito(efeito, cone, origemConeLinha, origemConeColuna);
    aplicarHabilidadeNoEfeito(efeito, cruz, origemCruzLinha, origemCruzColuna);
    aplicarHabilidadeNoEfeito(efeito, octaedro, origemOctLinha, origemOctCol);

    // Exibe o tabuleiro final com áreas de habilidade mostradas como 5
    printf("Tabuleiro final com áreas de habilidade (5 = área afetada, 3 = navio, 0 = água):\n");
    imprimirTabuleiro(tabuleiro, efeito);

    return 0;
}
