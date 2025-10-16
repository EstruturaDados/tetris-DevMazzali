#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Struct Peca
typedef struct {
    char tipo;
    int id;
} Peca;

// Struct para Fila Circular
typedef struct {
    Peca elementos[5];
    int frente;
    int tras;
    int tamanho;
    int capacidade;
} FilaCircular;

// Struct para Pilha Linear
typedef struct {
    Peca elementos[3];
    int topo;
    int capacidade;
} Pilha;

// Variável global para controlar ID sequencial
int proximoId = 0;

// ============ PROTÓTIPOS DAS FUNÇÕES ============
// Funções da Fila
void inicializarFila(FilaCircular *fila);
bool filaCheia(FilaCircular *fila);
bool filaVazia(FilaCircular *fila);
void enqueue(FilaCircular *fila, Peca peca);
Peca dequeue(FilaCircular *fila);
void mostrarFila(FilaCircular *fila);

// Funções da Pilha
void inicializarPilha(Pilha *pilha);
bool pilhaCheia(Pilha *pilha);
bool pilhaVazia(Pilha *pilha);
void push(Pilha *pilha, Peca peca);
Peca pop(Pilha *pilha);
void mostrarPilha(Pilha *pilha);

// Funções auxiliares
Peca gerarPeca();
void limparTela();
void exibirEstado(FilaCircular *fila, Pilha *pilha);
void exibirMenu();

// Funções de integração
void trocarFrentePilha(FilaCircular *fila, Pilha *pilha);
void trocarTresElementos(FilaCircular *fila, Pilha *pilha);

// ============ IMPLEMENTAÇÃO DAS FUNÇÕES DA FILA ============

void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    fila->capacidade = 5;
    
    // Preenche a fila inicial com 5 peças
    for(int i = 0; i < 5; i++) {
        enqueue(fila, gerarPeca());
    }
}

bool filaCheia(FilaCircular *fila) {
    return fila->tamanho == fila->capacidade;
}

bool filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

void enqueue(FilaCircular *fila, Peca peca) {
    if(filaCheia(fila)) {
        printf("ERRO: Fila cheia!\n");
        return;
    }
    
    fila->elementos[fila->tras] = peca;
    fila->tras = (fila->tras + 1) % fila->capacidade;
    fila->tamanho++;
}

Peca dequeue(FilaCircular *fila) {
    if(filaVazia(fila)) {
        printf("ERRO: Fila vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    Peca peca = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % fila->capacidade;
    fila->tamanho--;
    
    return peca;
}

void mostrarFila(FilaCircular *fila) {
    printf("Fila de pecas:\n");
    
    if(filaVazia(fila)) {
        printf("(Fila vazia)\n");
        return;
    }
    
    int indice = fila->frente;
    for(int i = 0; i < fila->tamanho; i++) {
        printf("[%c %d] ", fila->elementos[indice].tipo, fila->elementos[indice].id);
        indice = (indice + 1) % fila->capacidade;
    }
    printf("\n");
}

// ============ IMPLEMENTAÇÃO DAS FUNÇÕES DA PILHA ============

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
    pilha->capacidade = 3;
}

bool pilhaCheia(Pilha *pilha) {
    return pilha->topo == pilha->capacidade - 1;
}

bool pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

void push(Pilha *pilha, Peca peca) {
    if(pilhaCheia(pilha)) {
        printf("ERRO: Pilha cheia!\n");
        return;
    }
    
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
}

Peca pop(Pilha *pilha) {
    if(pilhaVazia(pilha)) {
        printf("ERRO: Pilha vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    Peca peca = pilha->elementos[pilha->topo];
    pilha->topo--;
    
    return peca;
}

void mostrarPilha(Pilha *pilha) {
    printf("Pilha de reserva:\n");
    
    if(pilhaVazia(pilha)) {
        printf("(Pilha vazia)\n");
        return;
    }
    
    printf("(Topo -> base): ");
    for(int i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->elementos[i].tipo, pilha->elementos[i].id);
    }
    printf("\n");
}

// ============ FUNÇÕES AUXILIARES ============

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca novaPeca;
    
    novaPeca.tipo = tipos[rand() % 4];
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void exibirEstado(FilaCircular *fila, Pilha *pilha) {
    printf("\n========================================\n");
    printf("         ESTADO ATUAL\n");
    printf("========================================\n\n");
    
    mostrarFila(fila);
    printf("\n");
    mostrarPilha(pilha);
    
    printf("\n========================================\n");
}

void exibirMenu() {
    printf("\n--- TETRIS STACK - MENU ---\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
    printf("---------------------------\n");
    printf("Opcao escolhida: ");
}

// ============ FUNÇÕES DE INTEGRAÇÃO (NÍVEL MESTRE) ============

void trocarFrentePilha(FilaCircular *fila, Pilha *pilha) {
    // Validações
    if(filaVazia(fila)) {
        printf("\nERRO: Fila vazia! Nao e possivel realizar a troca.\n");
        return;
    }
    
    if(pilhaVazia(pilha)) {
        printf("\nERRO: Pilha vazia! Nao e possivel realizar a troca.\n");
        return;
    }
    
    // Troca direta nos arrays
    Peca temp = fila->elementos[fila->frente];
    fila->elementos[fila->frente] = pilha->elementos[pilha->topo];
    pilha->elementos[pilha->topo] = temp;
    
    printf("\nAcao: troca realizada entre a peca da frente da fila e o topo da pilha.\n");
}

void trocarTresElementos(FilaCircular *fila, Pilha *pilha) {
    // Validações
    if(fila->tamanho < 3) {
        printf("\nERRO: A fila precisa ter pelo menos 3 pecas!\n");
        return;
    }
    
    if(pilha->topo != 2) {
        printf("\nERRO: A pilha precisa ter exatamente 3 pecas!\n");
        return;
    }
    
    // Armazena as 3 peças da fila temporariamente
    Peca tempFila[3];
    int indice = fila->frente;
    
    for(int i = 0; i < 3; i++) {
        tempFila[i] = fila->elementos[indice];
        indice = (indice + 1) % fila->capacidade;
    }
    
    // Substitui os 3 primeiros da fila pelas 3 da pilha (topo para base)
    indice = fila->frente;
    for(int i = pilha->topo; i >= 0; i--) {
        fila->elementos[indice] = pilha->elementos[i];
        indice = (indice + 1) % fila->capacidade;
    }
    
    // Substitui as 3 da pilha pelas 3 que estavam na fila (invertendo a ordem)
    for(int i = 0; i < 3; i++) {
        pilha->elementos[i] = tempFila[2 - i];
    }
    
    printf("\nAcao: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// ============ FUNÇÃO PRINCIPAL ============

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    FilaCircular fila;
    Pilha pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    int opcao;
    
    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // Jogar peça
                if(!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\nAcao: peca [%c %d] jogada!\n", jogada.tipo, jogada.id);
                    
                    // Repõe a fila com nova peça
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("\nERRO: Fila vazia!\n");
                }
                break;
                
            case 2: // Enviar para pilha
                if(pilhaCheia(&pilha)) {
                    printf("\nERRO: Pilha cheia! Nao e possivel adicionar mais pecas.\n");
                } else if(filaVazia(&fila)) {
                    printf("\nERRO: Fila vazia!\n");
                } else {
                    Peca enviada = dequeue(&fila);
                    push(&pilha, enviada);
                    printf("\nAcao: peca [%c %d] enviada para a pilha de reserva!\n", 
                           enviada.tipo, enviada.id);
                    
                    // Repõe a fila com nova peça
                    enqueue(&fila, gerarPeca());
                }
                break;
                
            case 3: // Usar da pilha
                if(!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\nAcao: peca [%c %d] usada da pilha de reserva!\n", 
                           usada.tipo, usada.id);
                } else {
                    printf("\nERRO: Pilha vazia!\n");
                }
                break;
                
            case 4: // Trocar frente com topo
                trocarFrentePilha(&fila, &pilha);
                break;
                
            case 5: // Trocar 3 elementos
                trocarTresElementos(&fila, &pilha);
                break;
                
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
                
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
        
        if(opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
            getchar();
        }
        
    } while(opcao != 0);
    
    return 0;
}
