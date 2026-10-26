#ifndef SJF_H
#define SJF_H

// Estrutura dos dados do processo
typedef struct Dados {
    int id;               // ID do processo (ex: 1, 2, 3)
    int tempo_chegada;    // Momento em que o processo chega
    int tempo_execucao;   // Duração / Burst Time
} Dados;

// Nó da lista simplesmente encadeada
typedef struct No {
    Dados informacoes;
    struct No *proximo;
} No;

// Cabeçalho da lista
typedef struct Lista {
    struct No *primeiro;
    int tamanho;
} Lista;

// Protótipos das funções
Lista* criar_lista();
void inserir_sjf(Lista *lista, Dados d);
int remover_primeiro(Lista *l, Dados *processo_removido);
void libertar_lista(Lista *l);
void executar_menu_sjf();

#endif 