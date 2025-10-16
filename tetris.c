#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================
// ESTRUTURAS DE DADOS
// ============================================

// Estrutura para representar um cômodo da mansão (Árvore Binária)
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Estrutura para pista na árvore BST
typedef struct Pista {
    char texto[100];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

// Estrutura para lista encadeada de pistas (usada na hash)
typedef struct NoPista {
    char pista[100];
    char suspeito[50];
    struct NoPista* proximo;
} NoPista;

// Estrutura da Tabela Hash
#define TAMANHO_HASH 10
typedef struct {
    NoPista* tabela[TAMANHO_HASH];
} TabelaHash;

// ============================================
// FUNÇÕES DA ÁRVORE BINÁRIA (MAPA DA MANSÃO)
// ============================================

/**
 * criarSala() - Cria dinamicamente um cômodo da mansão
 * @nome: Nome do cômodo a ser criado
 * 
 * Aloca memória para uma nova sala e inicializa seus ponteiros
 * esquerda e direita como NULL.
 * 
 * Retorna: Ponteiro para a nova sala criada
 */
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

/**
 * obterPistaDaSala() - Retorna a pista associada a um cômodo específico
 * @nomeSala: Nome do cômodo para buscar a pista
 * @pistaOut: Buffer onde a pista será armazenada
 * @suspeitoOut: Buffer onde o suspeito será armazenado
 * 
 * Define as pistas fixas associadas a cada sala da mansão.
 * Cada sala possui uma pista única que aponta para um suspeito.
 * 
 * Retorna: 1 se a sala possui pista, 0 caso contrário
 */
int obterPistaDaSala(const char* nomeSala, char* pistaOut, char* suspeitoOut) {
    if (strcmp(nomeSala, "Biblioteca") == 0) {
        strcpy(pistaOut, "Livro com paginas rasgadas");
        strcpy(suspeitoOut, "Professor");
        return 1;
    } else if (strcmp(nomeSala, "Cozinha") == 0) {
        strcpy(pistaOut, "Faca com manchas suspeitas");
        strcpy(suspeitoOut, "Cozinheiro");
        return 1;
    } else if (strcmp(nomeSala, "Sotao") == 0) {
        strcpy(pistaOut, "Carta anonima ameacadora");
        strcpy(suspeitoOut, "Mordomo");
        return 1;
    } else if (strcmp(nomeSala, "Quarto") == 0) {
        strcpy(pistaOut, "Relogio parado as 23h");
        strcpy(suspeitoOut, "Professor");
        return 1;
    } else if (strcmp(nomeSala, "Jardim") == 0) {
        strcpy(pistaOut, "Pegadas na terra molhada");
        strcpy(suspeitoOut, "Jardineiro");
        return 1;
    } else if (strcmp(nomeSala, "Adega") == 0) {
        strcpy(pistaOut, "Garrafa de veneno vazia");
        strcpy(suspeitoOut, "Cozinheiro");
        return 1;
    } else if (strcmp(nomeSala, "Escritorio") == 0) {
        strcpy(pistaOut, "Documento falsificado");
        strcpy(suspeitoOut, "Mordomo");
        return 1;
    }
    return 0;
}

// ============================================
// FUNÇÕES DA BST (ARMAZENAMENTO DE PISTAS)
// ============================================

/**
 * inserirPista() - Insere uma pista na árvore BST de forma ordenada
 * @raiz: Raiz da árvore BST
 * @texto: Texto da pista a ser inserida
 * 
 * Utiliza strcmp para comparar strings e inserir a pista
 * na posição correta da árvore, mantendo a ordem alfabética.
 * Usa alocação dinâmica para criar novos nós.
 * 
 * Retorna: Nova raiz da árvore após inserção
 */
Pista* inserirPista(Pista* raiz, const char* texto) {
    if (raiz == NULL) {
        Pista* novaPista = (Pista*)malloc(sizeof(Pista));
        strcpy(novaPista->texto, texto);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    
    return raiz;
}

/**
 * listarPistas() - Exibe todas as pistas em ordem alfabética
 * @raiz: Raiz da árvore BST de pistas
 * 
 * Utiliza percurso em-ordem (in-order traversal) recursivo
 * para exibir as pistas em ordem alfabética crescente.
 */
void listarPistas(Pista* raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("  - %s\n", raiz->texto);
        listarPistas(raiz->direita);
    }
}

// ============================================
// FUNÇÕES DA TABELA HASH
// ============================================

/**
 * funcaoHash() - Calcula o índice hash para uma chave
 * @chave: String usada como chave (pista)
 * 
 * Implementa função hash simples usando soma dos valores ASCII
 * dos caracteres da string, com deslocamento de bits para
 * melhor distribuição.
 * 
 * Retorna: Índice da tabela hash (0 a TAMANHO_HASH-1)
 */
unsigned int funcaoHash(const char* chave) {
    unsigned int hash = 0;
    while (*chave) {
        hash = (hash << 5) + *chave++;
    }
    return hash % TAMANHO_HASH;
}

/**
 * inicializarHash() - Inicializa a tabela hash
 * @tabela: Ponteiro para a tabela hash
 * 
 * Define todos os ponteiros da tabela como NULL,
 * preparando-a para receber inserções.
 */
void inicializarHash(TabelaHash* tabela) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabela->tabela[i] = NULL;
    }
}

/**
 * inserirNaHash() - Insere associação pista/suspeito na tabela hash
 * @tabela: Ponteiro para a tabela hash
 * @pista: Texto da pista (chave)
 * @suspeito: Nome do suspeito (valor)
 * 
 * Usa chaining (lista encadeada) para resolver colisões.
 * Insere o novo elemento no início da lista correspondente
 * ao índice calculado pela função hash.
 */
void inserirNaHash(TabelaHash* tabela, const char* pista, const char* suspeito) {
    unsigned int indice = funcaoHash(pista);
    
    NoPista* novoNo = (NoPista*)malloc(sizeof(NoPista));
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = tabela->tabela[indice];
    tabela->tabela[indice] = novoNo;
}

/**
 * encontrarSuspeito() - Consulta o suspeito correspondente a uma pista
 * @tabela: Ponteiro para a tabela hash
 * @pista: Pista a ser buscada
 * 
 * Calcula o índice hash e percorre a lista encadeada
 * correspondente até encontrar a pista desejada.
 * 
 * Retorna: Nome do suspeito ou NULL se não encontrado
 */
char* encontrarSuspeito(TabelaHash* tabela, const char* pista) {
    unsigned int indice = funcaoHash(pista);
    NoPista* atual = tabela->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/**
 * contarPistasPorSuspeito() - Conta quantas pistas apontam para um suspeito
 * @tabela: Ponteiro para a tabela hash
 * @nomeSuspeito: Nome do suspeito a ser contado
 * 
 * Percorre toda a tabela hash e conta quantas vezes
 * o suspeito aparece associado a pistas diferentes.
 * Usa recursão para percorrer as listas encadeadas.
 * 
 * Retorna: Número de pistas que apontam para o suspeito
 */
int contarPistasPorSuspeito(TabelaHash* tabela, const char* nomeSuspeito) {
    int contador = 0;
    
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoPista* atual = tabela->tabela[i];
        while (atual != NULL) {
            if (strcmp(atual->suspeito, nomeSuspeito) == 0) {
                contador++;
            }
            atual = atual->proximo;
        }
    }
    
    return contador;
}

// ============================================
// FUNÇÕES DE EXPLORAÇÃO E GAMEPLAY
// ============================================

/**
 * explorarSalas() - Navega pela árvore de salas e ativa o sistema de pistas
 * @salaAtual: Sala onde o jogador está atualmente
 * @pistasBST: Ponteiro para a raiz da BST de pistas
 * @tabelaHash: Ponteiro para a tabela hash de associações
 * 
 * Função interativa que permite ao jogador navegar pela mansão.
 * Ao visitar cada sala, verifica se há pista e a adiciona
 * automaticamente à BST e à tabela hash.
 * Aceita comandos: 'e' (esquerda), 'd' (direita), 's' (sair)
 */
void explorarSalas(Sala* salaAtual, Pista** pistasBST, TabelaHash* tabelaHash) {
    char comando;
    
    while (1) {
        printf("\n========================================\n");
        printf("Voce esta no(a): %s\n", salaAtual->nome);
        printf("========================================\n");
        
        // Verificar se há pista nesta sala
        char pista[100], suspeito[50];
        if (obterPistaDaSala(salaAtual->nome, pista, suspeito)) {
            printf("\n[!] PISTA ENCONTRADA: %s\n", pista);
            printf("[!] Esta pista pode estar relacionada a: %s\n", suspeito);
            
            // Adicionar pista na BST
            *pistasBST = inserirPista(*pistasBST, pista);
            
            // Adicionar na tabela hash
            inserirNaHash(tabelaHash, pista, suspeito);
        } else {
            printf("\nNenhuma pista encontrada nesta sala.\n");
        }
        
        // Mostrar opções de navegação
        printf("\nPara onde deseja ir?\n");
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Ir para %s (esquerda)\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("  [d] Ir para %s (direita)\n", salaAtual->direita->nome);
        }
        printf("  [s] Sair e fazer acusacao\n");
        
        printf("\nComando: ");
        scanf(" %c", &comando);
        
        if (comando == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (comando == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (comando == 's') {
            break;
        } else {
            printf("\n[X] Comando invalido! Tente novamente.\n");
        }
    }
}

/**
 * verificarSuspeitoFinal() - Conduz à fase de julgamento final
 * @tabelaHash: Ponteiro para a tabela hash de associações
 * 
 * Solicita ao jogador que acuse um suspeito e verifica
 * se existem pelo menos 2 pistas que sustentem a acusação.
 * Exibe o desfecho do jogo baseado na decisão do jogador.
 */
void verificarSuspeitoFinal(TabelaHash* tabelaHash) {
    char acusado[50];
    
    printf("\n========================================\n");
    printf("        HORA DA ACUSACAO FINAL!\n");
    printf("========================================\n");
    printf("\nSuspeitos disponiveis:\n");
    printf("  1. Professor\n");
    printf("  2. Cozinheiro\n");
    printf("  3. Mordomo\n");
    printf("  4. Jardineiro\n");
    
    printf("\nQuem voce acusa? Digite o nome: ");
    scanf("%s", acusado);
    
    int numPistas = contarPistasPorSuspeito(tabelaHash, acusado);
    
    printf("\n========================================\n");
    printf("           VEREDICTO FINAL\n");
    printf("========================================\n");
    printf("\nVoce acusou: %s\n", acusado);
    printf("Numero de pistas coletadas: %d\n", numPistas);
    
    if (numPistas >= 2) {
        printf("\n[SUCESSO] Parabens, detetive!\n");
        printf("Voce reuniu evidencias suficientes (%d pistas)\n", numPistas);
        printf("para provar que %s eh o culpado!\n", acusado);
        printf("\nO caso foi resolvido com maestria!\n");
    } else {
        printf("\n[FALHA] Infelizmente, voce nao reuniu evidencias\n");
        printf("suficientes para sustentar sua acusacao.\n");
        printf("Sao necessarias pelo menos 2 pistas.\n");
        printf("\nO verdadeiro culpado escapou...\n");
    }
}

// ============================================
// FUNÇÃO PRINCIPAL
// ============================================

int main() {
    printf("\n");
    printf("*******************************************\n");
    printf("*                                         *\n");
    printf("*        DETECTIVE QUEST                 *\n");
    printf("*      O Misterio da Mansao Sombria      *\n");
    printf("*                                         *\n");
    printf("*******************************************\n");
    printf("\nBem-vindo, detetive!\n");
    printf("Um crime horrivel foi cometido na mansao.\n");
    printf("Explore os comodos, colete pistas e\n");
    printf("descubra quem eh o culpado!\n");
    
    // Criar o mapa da mansão (Árvore Binária fixa)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sotao");
    Sala* quarto = criarSala("Quarto");
    Sala* jardim = criarSala("Jardim");
    Sala* adega = criarSala("Adega");
    Sala* escritorio = criarSala("Escritorio");
    
    // Montar a estrutura da árvore
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = sotao;
    biblioteca->direita = quarto;
    cozinha->esquerda = jardim;
    cozinha->direita = adega;
    jardim->esquerda = escritorio;
    
    // Inicializar a BST de pistas e a tabela hash
    Pista* pistasBST = NULL;
    TabelaHash tabelaHash;
    inicializarHash(&tabelaHash);
    
    // Iniciar exploração
    explorarSalas(hall, &pistasBST, &tabelaHash);
    
    // Mostrar todas as pistas coletadas em ordem
    printf("\n========================================\n");
    printf("     PISTAS COLETADAS (em ordem)\n");
    printf("========================================\n");
    listarPistas(pistasBST);
    
    // Fase de acusação final
    verificarSuspeitoFinal(&tabelaHash);
    
    printf("\n========================================\n");
    printf("         Obrigado por jogar!\n");
    printf("========================================\n\n");
    
    return 0;
}
