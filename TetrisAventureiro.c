#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

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

// Estrutura da pilha de reserva
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ======= PROTÓTIPOS =======
void inicializarFila(FilaCircular *fila);
void inicializarPilha(Pilha *pilha);
int filaCheia(FilaCircular *fila);
int filaVazia(FilaCircular *fila);
int pilhaCheia(Pilha *pilha);
int pilhaVazia(Pilha *pilha);
void enfileirar(FilaCircular *fila, Peca nova);
Peca desenfileirar(FilaCircular *fila);
void push(Pilha *pilha, Peca nova);
Peca pop(Pilha *pilha);
Peca gerarPeca();
void exibirFila(FilaCircular *fila);
void exibirPilha(Pilha *pilha);
void menu();

// ======= FUNÇÃO PRINCIPAL =======
int main() {
    srand(time(NULL));
    FilaCircular fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    do {
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: // Jogar peça
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

            case 2: // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  A pilha de reserva está cheia! Use uma peça antes de guardar outra.\n");
                } else if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    push(&pilha, reservada);
                    printf("\n💾 Peça %s (ID %d) foi reservada!\n", reservada.nome, reservada.id);

                    Peca nova = gerarPeca();
                    enfileirar(&fila, nova);
                    printf("Nova peça adicionada automaticamente: %s (ID %d)\n", nova.nome, nova.id);
                } else {
                    printf("\n⚠️  Fila vazia! Nenhuma peça para reservar.\n");
                }
                break;

            case 3: // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("\n⚠️  Nenhuma peça reservada para usar!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\n🎯 Peça reservada usada: %s (ID %d)\n", usada.nome, usada.id);
                }
                break;

            case 0:
                printf("\nSaindo do jogo... 👋\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

        // Exibir estado atual da fila e pilha após cada ação
        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}

// ======= FUNÇÕES DA FILA =======

void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;

    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        enfileirar(fila, nova);
    }
}

int filaCheia(FilaCircular *fila) {
    return fila->tamanho == TAM_FILA;
}

int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

void enfileirar(FilaCircular *fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n⚠️  Fila cheia!\n");
        return;
    }

    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = nova;
    fila->tamanho++;
}

Peca desenfileirar(FilaCircular *fila) {
    Peca removida = {"", -1};

    if (filaVazia(fila)) {
        printf("\n⚠️  Fila vazia!\n");
        return removida;
    }

    removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;

    return removida;
}

void exibirFila(FilaCircular *fila) {
    printf("\n📦 Fila de Peças Futuras:\n");
    printf("---------------------------\n");

    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
    } else {
        for (int i = 0; i < fila->tamanho; i++) {
            int indice = (fila->frente + i) % TAM_FILA;
            printf("Posição %d → Peça %s (ID %d)\n", i + 1, fila->pecas[indice].nome, fila->pecas[indice].id);
        }
    }
    printf("---------------------------\n");
}

// ======= FUNÇÕES DA PILHA =======

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

void push(Pilha *pilha, Peca nova) {
    if (pilhaCheia(pilha)) {
        printf("\n⚠️  Pilha cheia!\n");
        return;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = nova;
}

Peca pop(Pilha *pilha) {
    Peca removida = {"", -1};

    if (pilhaVazia(pilha)) {
        printf("\n⚠️  Pilha vazia!\n");
        return removida;
    }

    removida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return removida;
}

void exibirPilha(Pilha *pilha) {
    printf("\n🧱 Pilha de Peças Reservadas:\n");
    printf("---------------------------\n");

    if (pilhaVazia(pilha)) {
        printf("Nenhuma peça reservada.\n");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("Nível %d → Peça %s (ID %d)\n", i + 1, pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("---------------------------\n");
}

// ======= FUNÇÕES AUXILIARES =======

Peca gerarPeca() {
    char tipos[4][2] = {"I", "O", "T", "L"};
    Peca nova;
    int tipoIndex = rand() % 4;
    strcpy(nova.nome, tipos[tipoIndex]);
    nova.id = rand() % 1000 + 1;
    return nova;
}

void menu() {
    printf("\n===== 🧩 TETRIS STACK - NÍVEL AVENTUREIRO =====\n");
    printf("1. Jogar peça\n");
    printf("2. Reservar peça\n");
    printf("3. Usar peça reservada\n");
    printf("0. Sair\n");
    printf("===============================================\n");
}
