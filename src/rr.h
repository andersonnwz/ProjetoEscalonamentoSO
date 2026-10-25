#ifndef RR_H
#define RR_H

// Estrutura que representa um Processo
typedef struct {
    int id;
    int tempo_chegada;
    int tempo_burst;
    int tempo_restante;
    int tempo_conclusao;
    int tempo_espera;
    int tempo_retorno; // Turnaround time
} ProcessoRR;

// Nó da Fila Dinâmica
typedef struct NoRR {
    int id_processo; 
    struct NoRR* proximo;
} NoRR;

// Estrutura da Fila FIFO
typedef struct {
    NoRR* frente;
    NoRR* tras;
} FilaRR;

// Protótipos das funções do Round Robin
void executar_menu_rr();

#endif // RR_H
