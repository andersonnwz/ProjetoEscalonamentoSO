#ifndef FCFS_H
#define FCFS_H

// Estrutura com os dados do processo
typedef struct {
    int id;
    int chegada;
    int burst;
    int espera;
    int turnaround;
    int termino;
} ProcessoFCFS;

// Nó da Fila Encadeada
typedef struct NoFCFS {
    ProcessoFCFS processo;
    struct NoFCFS *proximo;
} NoFCFS;

// Estrutura de controle da Fila
typedef struct {
    NoFCFS *inicio;
    NoFCFS *fim;
    int tamanho;
} FilaFCFS;

// Funções públicas
FilaFCFS* criar_fila_fcfs();
void enfileirar_fcfs(FilaFCFS *fila, ProcessoFCFS p);
int desenfileirar_fcfs(FilaFCFS *fila, ProcessoFCFS *p_saindo);
void liberar_fila_fcfs(FilaFCFS *fila);
void executar_menu_fcfs();

#endif