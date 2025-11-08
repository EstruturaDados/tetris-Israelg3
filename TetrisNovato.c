#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5

// Estrutura que representa uma peça
typedef struct {
    int id;
    char nome[2]; // Ex: "I", "O", "T", "L"
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Protótipos das funções
void inicializarFila(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
int filaVazia(FilaCircular *fila);
void enfileirar(FilaCircular *fila, Peca nova);
Peca desenfileirar(FilaCircular *fila);
void exibirFila(FilaCircular *fila);
Peca gerarPeca();
void menu();

// Função principal
int main() {
    srand(time(NULL)); // Gera números aleatórios diferentes a cada execução
    FilaCircular fila;
    inicializarFila(&fila);

    int opcao;

    do {
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer

        switch (opcao) {
            case 1:
                exibirFila(&fila);
                break;
            case 2:
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada: %s (ID %d)\n", jogada.nome, jogada.id);

                    Peca nova = gerarPeca();
                    enfileirar(&fila, nova);
                    printf("Nova peça adicionada automaticamente: %s (ID %d)\n", nova.nome, nova.id);
                } else {
                    printf("\n⚠️  Fila vazia! Nenhuma peça para jogar.\n");
                }
                break;
            case 3:
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca();
                    enfileirar(&fila, nova);
                    printf("\n✅ Nova peça adicionada: %s (ID %d)\n", nova.nome, nova.id);
                } else {
                    printf("\n⚠️  Fila cheia! Jogue uma peça primeiro.\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo... 👋\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ====================== FUNÇÕES ======================

// Inicializa a fila com 5 peças geradas automaticamente
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;

    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        enfileirar(fila, nova);
    }
}

// Verifica se a fila está cheia
int filaCheia(FilaCircular *fila) {
    return fila->tamanho == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

// Enfileira uma nova peça
void enfileirar(FilaCircular *fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n⚠️  A fila está cheia!\n");
        return;
    }

    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = nova;
    fila->tamanho++;
}

// Desenfileira (remove) a peça da frente
Peca desenfileirar(FilaCircular *fila) {
    Peca removida = {"", -1};

    if (filaVazia(fila)) {
        printf("\n⚠️  A fila está vazia!\n");
        return removida;
    }

    removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;

    return removida;
}

// Exibe o estado atual da fila
void exibirFila(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("\n📭 Fila vazia!\n");
        return;
    }

    printf("\n📦 Fila de Peças Futuras:\n");
    printf("---------------------------\n");

    int i, indice;
    for (i = 0; i < fila->tamanho; i++) {
        indice = (fila->frente + i) % TAM_FILA;
        printf("Posição %d → Peça %s (ID %d)\n", i + 1, fila->pecas[indice].nome, fila->pecas[indice].id);
    }
    printf("---------------------------\n");
}

// Gera automaticamente uma nova peça aleatória
Peca gerarPeca() {
    char tipos[4][2] = {"I", "O", "T", "L"};
    Peca nova;

    int tipoIndex = rand() % 4;
    strcpy(nova.nome, tipos[tipoIndex]);
    nova.id = rand() % 1000 + 1;

    return nova;
}

// Exibe o menu principal
void menu() {
    printf("\n===== 🎮 TETRIS STACK - NÍVEL NOVATO =====\n");
    printf("1. Visualizar fila de peças\n");
    printf("2. Jogar (remover frente e adicionar nova)\n");
    printf("3. Inserir nova peça manualmente\n");
    printf("0. Sair\n");
    printf("==========================================\n");
}
