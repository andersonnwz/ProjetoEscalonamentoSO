#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura que representa um Processo
typedef struct {
    int id;
    int tempo_chegada;
    int tempo_burst;
    int tempo_restante;
    int tempo_conclusao;
    int tempo_espera;
    int tempo_retorno; // Turnaround time
} Processo;

// Nó da Fila Dinâmica
typedef struct No {
    int id_processo; // Armazena o índice do processo na lista principal
    struct No* proximo;
} No;

// Estrutura da Fila FIFO
typedef struct {
    No* frente;
    No* tras;
} Fila;

// Função para criar uma fila vazia
Fila* criarFila() {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    f->frente = NULL;
    f->tras = NULL;
    return f;
}

// Verifica se a fila está vazia
bool filaVazia(Fila* f) {
    return (f->frente == NULL);
}

// Insere elemento no final da fila
void enfileirar(Fila* f, int id_processo) {
    No* novo = (No*)malloc(sizeof(No));
    novo->id_processo = id_processo;
    novo->proximo = NULL;
    if (filaVazia(f)) {
        f->frente = novo;
        f->tras = novo;
    } else {
        f->tras->proximo = novo;
        f->tras = novo;
    }
}

// Remove elemento do início da fila
int desenfileirar(Fila* f) {
    if (filaVazia(f)) return -1;
    No* temp = f->frente;
    int id = temp->id_processo;
    f->frente = f->frente->proximo;
    if (f->frente == NULL) {
        f->tras = NULL;
    }
    free(temp);
    return id;
}

// Implementação do Algoritmo Round Robin
void executarRoundRobin(Processo proc[], int n, int quantum) {
    int tempo_atual = 0;
    int concluidos = 0;
    bool inserido_na_fila[n];

    for (int i = 0; i < n; i++) {
        inserido_na_fila[i] = false;
        proc[i].tempo_restante = proc[i].tempo_burst;
    }

    Fila* fila = criarFila();

    printf("\n=== ORDEM DE EXECUCAO DA CPU ===\n");

    // Adiciona processos que chegam no tempo 0
    for (int i = 0; i < n; i++) {
        if (proc[i].tempo_chegada == 0) {
            enfileirar(fila, i);
            inserido_na_fila[i] = true;
        }
    }

    // Se no tempo 0 nenhum processo chegou, avança o tempo até o primeiro chegar
    if (filaVazia(fila)) {
        int menor_chegada = proc[0].tempo_chegada;
        for (int i = 1; i < n; i++) {
            if (proc[i].tempo_chegada < menor_chegada) {
                menor_chegada = proc[i].tempo_chegada;
            }
        }
        tempo_atual = menor_chegada;
        for (int i = 0; i < n; i++) {
            if (proc[i].tempo_chegada == tempo_atual) {
                enfileirar(fila, i);
                inserido_na_fila[i] = true;
            }
        }
    }

    while (concluidos < n) {
        if (filaVazia(fila)) {
            // Caso a fila fique vazia mas ainda existam processos a chegar (tempo ocioso da CPU)
            tempo_atual++;
            for (int i = 0; i < n; i++) {
                if (!inserido_na_fila[i] && proc[i].tempo_chegada <= tempo_atual) {
                    enfileirar(fila, i);
                    inserido_na_fila[i] = true;
                }
            }
            continue;
        }

        int idx = desenfileirar(fila);
        int tempo_execucao = (proc[idx].tempo_restante > quantum) ? quantum : proc[idx].tempo_restante;

        printf("[Tempo %02d - %02d] Processo P%d executando\n", tempo_atual, tempo_atual + tempo_execucao, proc[idx].id);

        tempo_atual += tempo_execucao;
        proc[idx].tempo_restante -= tempo_execucao;

        // Verifica e enfileira novos processos que chegaram DURANTE essa execução
        for (int i = 0; i < n; i++) {
            if (!inserido_na_fila[i] && proc[i].tempo_chegada <= tempo_atual) {
                enfileirar(fila, i);
                inserido_na_fila[i] = true;
            }
        }

        // Se o processo atual ainda não terminou, ele volta para o final da fila
        if (proc[idx].tempo_restante > 0) {
            enfileirar(fila, idx);
        } else {
            // Processo finalizado
            concluidos++;
            proc[idx].tempo_conclusao = tempo_atual;
            proc[idx].tempo_retorno = proc[idx].tempo_conclusao - proc[idx].tempo_chegada;
            proc[idx].tempo_espera = proc[idx].tempo_retorno - proc[idx].tempo_burst;
        }
    }

    // Exibição dos resultados e estatísticas
    printf("\n=== TABELA DE RESULTADOS (ROUND ROBIN) ===\n");
    printf("ID\tChegada\tBurst\tConclusao\tRetorno (TA)\tEspera (WT)\n");
    
    float soma_espera = 0, soma_retorno = 0;
    for (int i = 0; i < n; i++) {
        soma_espera += proc[i].tempo_espera;
        soma_retorno += proc[i].tempo_retorno;
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n", 
               proc[i].id, proc[i].tempo_chegada, proc[i].tempo_burst, 
               proc[i].tempo_conclusao, proc[i].tempo_retorno, proc[i].tempo_espera);
    }

    printf("\nTempo Medio de Espera: %.2f\n", soma_espera / n);
    printf("Tempo Medio de Retorno: %.2f\n", soma_retorno / n);

    free(fila);
}

int main() {
    int n, quantum;

    printf("=== SIMULADOR ROUND ROBIN ===\n");
    printf("Digite a quantidade de processos: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 0;

    printf("Digite o valor do QUANTUM: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) return 0;

    Processo proc[n];

    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("\nProcesso P%d:\n", proc[i].id);
        printf("  Tempo de Chegada: ");
        scanf("%d", &proc[i].tempo_chegada);
        printf("  Tempo de Execucao (Burst): ");
        scanf("%d", &proc[i].tempo_burst);
    }

    executarRoundRobin(proc, n, quantum);

    return 0;
}
