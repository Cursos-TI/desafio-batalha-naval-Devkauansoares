#include <stdio.h>

// Função para imprimir o tabuleiro
void imprimirTabuleiro (int tabuleiro[10][10]) {
    printf("Imprimindo tabuleiro...\n");

    // Cabeçalho com letras A-J
    printf("     ");
    for (int j = 0; j < 10; j++) {
        printf(" %c ", 'A' + j);
    }
    printf("\n");

    // Linhas numeradas
    for (int i = 0; i < 10; i++) {
        if (i < 9)
            printf("0%d-  ", i+1);
        else
            printf("%d-  ", i+1);

        for (int j = 0; j < 10; j++) {
            printf("%d  ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

// Função para posicionar navio
// orientacao: 0 = horizontal, 1 = vertical
void jogarNavio(int tabuleiro[10][10], int linha, int coluna, int tamanho, int orientacao) {
    if (orientacao == 0) { // horizontal
        for (int i = 0; i < tamanho; i++) {
            tabuleiro[linha][coluna + i] = 3;
        }
    } else { // vertical
        for (int i = 0; i < tamanho; i++) {
            tabuleiro[linha + i][coluna] = 3;
        }
    }
}

int main() {
    // Tabuleiro inicializado com água (0)
    int tabuleiro[10][10] = {0};

    // Tamanho fixo dos navios
    int tamanhoNavio = 3;

    // Coordenadas iniciais para os navios
    int linhaNavio1, colunaNavio1;
    int linhaNavio2, colunaNavio2;

    // Primeiro print (tabuleiro vazio)
    imprimirTabuleiro(tabuleiro);

    // --- Posicionando Navio 1 (horizontal) ---
    printf("\nCoordenadas de início do Navio 1 (horizontal)\n");
    printf("Linha (0-9): ");
    scanf("%d", &linhaNavio1);
    while (linhaNavio1 < 0 || linhaNavio1 > 9) {
        printf("Coordenada inválida! Digite entre 0 e 9: ");
        scanf("%d", &linhaNavio1);
    }

    printf("Coluna (0-7 para caber 3 casas): ");
    scanf("%d", &colunaNavio1);
    while (colunaNavio1 < 0 || colunaNavio1 > 7) {
        printf("Coordenada inválida! Digite entre 0 e 7: ");
        scanf("%d", &colunaNavio1);
    }

    jogarNavio(tabuleiro, linhaNavio1, colunaNavio1, tamanhoNavio, 0);

    // --- Posicionando Navio 2 (vertical) ---
    printf("\nCoordenadas de início do Navio 2 (vertical)\n");
    printf("Linha (0-7 para caber 3 casas): ");
    scanf("%d", &linhaNavio2);
    while (linhaNavio2 < 0 || linhaNavio2 > 7) {
        printf("Coordenada inválida! Digite entre 0 e 7: ");
        scanf("%d", &linhaNavio2);
    }

    printf("Coluna (0-9): ");
    scanf("%d", &colunaNavio2);
    while (colunaNavio2 < 0 || colunaNavio2 > 9) {
        printf("Coordenada inválida! Digite entre 0 e 9: ");
        scanf("%d", &colunaNavio2);
    }

    // Verifica se há sobreposição simples
    int sobrepoe = 0;
    for (int i = 0; i < tamanhoNavio; i++) {
        if (tabuleiro[linhaNavio2 + i][colunaNavio2] == 3) {
            sobrepoe = 1;
        }
    }

    if (sobrepoe) {
        printf("\nERRO: O Navio 2 não pode ser colocado, ele sobrepõe o Navio 1!\n");
    } else {
        jogarNavio(tabuleiro, linhaNavio2, colunaNavio2, tamanhoNavio, 1);
    }

    // Exibe o tabuleiro atualizado
    printf("\nTabuleiro final:\n");
    imprimirTabuleiro(tabuleiro);

    return 0;
}
