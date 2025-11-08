/* TetrisMestre.c
   Nível Mestre: fila circular (5) + pilha (3) + trocas, desfazer e inverter.
   Compilar: gcc -o TetrisMestre TetrisMestre.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3
#define MAX_HISTORY 100

typedef struct {
    int id;
    char nome[3]; // espaço para terminador
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo; // -1 vazio
} Pilha;

typedef struct {
    FilaCircular fila;
    Pilha pilha;
} Snapshot;

// ======= PROTÓTIPOS =======
void inicializarFila(FilaCircular *fila);
void inicializarPilha(Pilha *pilha);
Peca gerarPeca();

int filaVazia(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
void enfileirar(FilaCircular *fila, Peca p);
Peca desenfileirar(FilaCircular *fila);

int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
void push(Pilha *pilha, Peca p);
Peca pop(Pilha *pilha);

void exibirFila(FilaCircular *fila);
void exibirPilha(Pilha *pilha);
void menu();

// Histórico (undo)
Snapshot history[MAX_HISTORY];
int historyTop = -1;
void pushSnapshot(FilaCircular *fila, Pilha *pilha);
int popSnapshot(FilaCircular *fila, Pilha *pilha);

// Operações compostas (Mestre)
void operarJogar(FilaCircular *fila, Pilha *pilha);
void operarReservar(FilaCircular *fila, Pilha *pilha);
void operarUsarReservada(FilaCircular *fila, Pilha *pilha);
void operarTrocarTopoComFrente(FilaCircular *fila, Pilha *pilha);
void operarDesfazer(FilaCircular *fila, Pilha *pilha);
void operarInverter(FilaCircular *fila, Pilha *pilha);

// ======= MAIN =======
int main() {
    srand((unsigned) time(NULL));
    FilaCircular fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;
    do {
        menu();
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }
        getchar();

        switch (opcao) {
            case 1:
                operarJogar(&fila, &pilha);
                break;
            case 2:
                operarReservar(&fila, &pilha);
                break;
            case 3:
                operarUsarReservada(&fila, &pilha);
                break;
            case 4:
                operarTrocarTopoComFrente(&fila, &pilha);
                break;
            case 5:
                operarDesfazer(&fila, &pilha);
                break;
            case 6:
                operarInverter(&fila, &pilha);
                break;
            case 0:
                printf("\nSaindo... Até a próxima!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }

        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}

// ======= IMPLEMENTAÇÕES =======

// Inicialização
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    for (int i = 0; i < TAM_FILA; ++i) {
        Peca p = gerarPeca();
        enfileirar(fila, p);
    }
}

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Geração de peça
Peca gerarPeca() {
    char tipos[4][3] = {"I","O","T","L"};
    Peca p;
    strcpy(p.nome, tipos[rand() % 4]);
    p.id = rand() % 100000 + 1;
    return p;
}

// Fila
int filaVazia(FilaCircular *fila) { return fila->tamanho == 0; }
int filaCheia(FilaCircular *fila) { return fila->tamanho == TAM_FILA; }

void enfileirar(FilaCircular *fila, Peca p) {
    if (filaCheia(fila)) {
        printf("[WARN] Tentativa de enfileirar em fila cheia.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = p;
    fila->tamanho++;
}

Peca desenfileirar(FilaCircular *fila) {
    Peca vazio = { -1, "" };
    if (filaVazia(fila)) {
        printf("[WARN] Desenfileirar de fila vazia.\n");
        return vazio;
    }
    Peca ret = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    return ret;
}

// Pilha
int pilhaVazia(Pilha *pilha) { return pilha->topo == -1; }
int pilhaCheia(Pilha *pilha) { return pilha->topo == TAM_PILHA - 1; }

void push(Pilha *pilha, Peca p) {
    if (pilhaCheia(pilha)) {
        printf("[WARN] Push em pilha cheia.\n");
        return;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = p;
}

Peca pop(Pilha *pilha) {
    Peca vazio = { -1, "" };
    if (pilhaVazia(pilha)) {
        printf("[WARN] Pop em pilha vazia.\n");
        return vazio;
    }
    Peca ret = pilha->pecas[pilha->topo];
    pilha->topo--;
    return ret;
}

// Exibição
void exibirFila(FilaCircular *fila) {
    printf("\n📦 Fila (frente → tras) [tam=%d]:\n", fila->tamanho);
    printf("---------------------------\n");
    if (filaVazia(fila)) {
        printf("Fila vazia.\n");
    } else {
        for (int i = 0; i < fila->tamanho; ++i) {
            int idx = (fila->frente + i) % TAM_FILA;
            printf("Pos %d -> %s (ID %d)\n", i+1, fila->pecas[idx].nome, fila->pecas[idx].id);
        }
    }
    printf("---------------------------\n");
}

void exibirPilha(Pilha *pilha) {
    printf("\n🧱 Pilha (topo primeiro) [tam=%d]:\n", pilha->topo + 1);
    printf("---------------------------\n");
    if (pilhaVazia(pilha)) {
        printf("Nenhuma peça reservada.\n");
    } else {
        for (int i = pilha->topo; i >= 0; --i) {
            printf("Nivel %d -> %s (ID %d)\n", i+1, pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("---------------------------\n");
}

// Menu
void menu() {
    printf("\n===== 🏆 TETRIS STACK - NÍVEL MESTRE =====\n");
    printf("1 - Jogar peça (remover frente)\n");
    printf("2 - Reservar peça (push da frente)\n");
    printf("3 - Usar peça reservada (pop)\n");
    printf("4 - Trocar topo da pilha com frente da fila\n");
    printf("5 - Desfazer última ação\n");
    printf("6 - Inverter/fazer troca total (regras de capacidade aplicadas)\n");
    printf("0 - Sair\n");
    printf("==========================================\n");
}

// ======= Histórico (undo) =======
void pushSnapshot(FilaCircular *fila, Pilha *pilha) {
    if (historyTop + 1 >= MAX_HISTORY) {
        // remover o mais antigo (shift left) para abrir espaço simples
        for (int i = 0; i < historyTop; ++i) history[i] = history[i+1];
        historyTop--;
    }
    historyTop++;
    history[historyTop].fila = *fila;
    history[historyTop].pilha = *pilha;
}

int popSnapshot(FilaCircular *fila, Pilha *pilha) {
    if (historyTop < 0) return 0;
    *fila = history[historyTop].fila;
    *pilha = history[historyTop].pilha;
    historyTop--;
    return 1;
}

// ======= Operações compostas =======

void operarJogar(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n⚠️  Fila vazia — nenhuma peça para jogar.\n");
        return;
    }
    pushSnapshot(fila, pilha);
    Peca jogada = desenfileirar(fila);
    printf("\n🎮 Jogou: %s (ID %d)\n", jogada.nome, jogada.id);
    Peca nova = gerarPeca();
    enfileirar(fila, nova);
    printf("➕ Nova peça enfileirada automaticamente: %s (ID %d)\n", nova.nome, nova.id);
}

void operarReservar(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n⚠️  Fila vazia — nenhuma peça para reservar.\n");
        return;
    }
    if (pilhaCheia(pilha)) {
        printf("\n⚠️  Pilha cheia — libere espaço antes de reservar.\n");
        return;
    }
    pushSnapshot(fila, pilha);
    Peca r = desenfileirar(fila);
    push(pilha, r);
    printf("\n💾 Reservada: %s (ID %d)\n", r.nome, r.id);
    Peca nova = gerarPeca();
    enfileirar(fila, nova);
    printf("➕ Nova peça enfileirada automaticamente: %s (ID %d)\n", nova.nome, nova.id);
}

void operarUsarReservada(FilaCircular *fila, Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("\n⚠️  Nenhuma peça reservada para usar.\n");
        return;
    }
    pushSnapshot(fila, pilha);
    Peca usada = pop(pilha);
    printf("\n🎯 Peça reservada usada: %s (ID %d)\n", usada.nome, usada.id);
}

void operarTrocarTopoComFrente(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n⚠️  Fila vazia — nada para trocar.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("\n⚠️  Pilha vazia — nada para trocar.\n");
        return;
    }
    pushSnapshot(fila, pilha);

    int idx = fila->frente;
    Peca temp = fila->pecas[idx];
    fila->pecas[idx] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;

    printf("\n🔁 Troca realizada entre topo da pilha e frente da fila.\n");
}

void operarDesfazer(FilaCircular *fila, Pilha *pilha) {
    if (!popSnapshot(fila, pilha)) {
        printf("\n⚠️  Nada para desfazer.\n");
    } else {
        printf("\n↩️  Desfazer realizado: estado anterior restaurado.\n");
    }
}

void operarInverter(FilaCircular *fila, Pilha *pilha) {
    int filaTam = fila->tamanho;
    int pilhaTam = pilha->topo + 1;

    if (filaTam > TAM_PILHA) {
        printf("\n⚠️  Não é possível inverter: fila tem %d elementos e pilha só suporta %d.\n", filaTam, TAM_PILHA);
        return;
    }
    if (pilhaTam > TAM_FILA) {
        printf("\n⚠️  Não é possível inverter: pilha tem %d elementos e fila só suporta %d.\n", pilhaTam, TAM_FILA);
        return;
    }

    pushSnapshot(fila, pilha);

    // coletar elementos da fila na ordem (frente → tras) em temp
    Peca tempFila[TAM_FILA];
    for (int i = 0; i < filaTam; ++i) {
        int idx = (fila->frente + i) % TAM_FILA;
        tempFila[i] = fila->pecas[idx];
    }

    // coletar elementos da pilha do fundo ao topo (0..topo)
    Peca tempPilha[TAM_PILHA];
    for (int i = 0; i < pilhaTam; ++i) {
        tempPilha[i] = pilha->pecas[i];
    }

    // limpar estruturas
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    pilha->topo = -1;

    // colocar os elementos antigos da pilha na fila (mantendo ordem do índice 0 -> frente)
    for (int i = 0; i < pilhaTam; ++i) {
        enfileirar(fila, tempPilha[i]);
    }

    // colocar os elementos antigos da fila na pilha (mantendo ordem: tempFila[0] -> fundo da pilha)
    for (int i = 0; i < filaTam; ++i) {
        push(pilha, tempFila[i]);
    }

    printf("\n🔄 Inversão completa realizada (conteúdo trocado entre fila e pilha).\n");
}
