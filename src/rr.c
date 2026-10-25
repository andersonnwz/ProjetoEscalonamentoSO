#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "rr.h"

// Função para criar uma fila vazia
static FilaRR* criarFilaRR() {
    FilaRR* f = (FilaRR*)malloc(sizeof(FilaRR));
    f->frente = NULL;
    f->tras = NULL;
    return f;
}

// Verifica se a fila está vazia
static bool filaVaziaRR(FilaRR* f) {
    return (f->frente == NULL);
}

// Insere elemento no final da fila
static void enfileirarRR(FilaRR* f, int id_processo) {
    NoRR* novo = (NoRR*)malloc(sizeof(NoRR));
    novo->id_processo = id_processo;
    novo->proximo = NULL;
    if (filaVaziaRR(f)) {
        f->frente = novo;
        f->tras = novo;
    } else {
        f->tras->proximo = novo;
        f->tras = novo;
    }
}

// Remove elemento do início da fila
static int desenfileirarRR(FilaRR* f) {
    if (filaVaziaRR(f)) return -1;
    NoRR* temp = f->frente;
    int id = temp->id_processo;
    f->frente = f->frente->proximo;
    if (f->frente == NULL) {
        f->tras = NULL;
    }
    free(temp);
    return id;
}

// Lógica principal do Algoritmo Round Robin
static void executarRoundRobinLogica(ProcessoRR proc[], int n, int quantum) {
    int tempo_atual = 0;
    int concluidos = 0;
    bool inserido_na_fila[n];

    for (int i = 0; i < n; i++) {
        inserido_na_fila[i] = false;
        proc[i].tempo_restante = proc[i].tempo_burst;
    }

    FilaRR* fila = criarFilaRR();

    printf("\n=== ORDEM DE EXECUCAO DA CPU ===\n");

    // Adiciona processos que chegam no tempo 0
    for (int i = 0; i < n; i++) {
        if (proc[i].tempo_chegada == 0) {
            enfileirarRR(fila, i);
            inserido_na_fila[i] = true;
        }
    }

    // Se no tempo 0 nenhum processo chegou, avança o tempo até o primeiro chegar
    if (filaVaziaRR(fila)) {
        int menor_chegada = proc[0].tempo_chegada;
        for (int i = 1; i < n; i++) {
            if (proc[i].tempo_chegada < menor_chegada) {
                menor_chegada = proc[i].tempo_chegada;
            }
        }
        tempo_atual = menor_chegada;
        for (int i = 0; i < n; i++) {
            if (proc[i].tempo_chegada == tempo_atual) {
                enfileirarRR(fila, i);
                inserido_na_fila[i] = true;
            }
        }
    }

    while (concluidos < n) {
        if (filaVaziaRR(fila)) {
            tempo_atual++;
            for (int i = 0; i < n; i++) {
                if (!inserido_na_fila[i] && proc[i].tempo_chegada <= tempo_atual) {
                    enfileirarRR(fila, i);
                    inserido_na_fila[i] = true;
                }
            }
            continue;
        }

        int idx = desenfileirarRR(fila);
        int tempo_execucao = (proc[idx].tempo_restante > quantum) ? quantum : proc[idx].tempo_restante;

        printf("[Tempo %02d - %02d] Processo P%d executando\n", tempo_atual, tempo_atual + tempo_execucao, proc[idx].id);

        tempo_atual += tempo_execucao;
        proc[idx].tempo_restante -= tempo_execucao;

        // Verifica novos processos que chegaram durante essa execução
        for (int i = 0; i < n; i++) {
            if (!inserido_na_fila[i] && proc[i].tempo_chegada <= tempo_atual) {
                enfileirarRR(fila, i);
                inserido_na_fila[i] = true;
            }
        }

        // Se ainda não terminou, volta para o fim da fila
        if (proc[idx].tempo_restante > 0) {
            enfileirarRR(fila, idx);
        } else {
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

// Função chamada pelo menu principal
void executar_menu_rr() {
    int n, quantum;

    printf("\n=== SIMULADOR ROUND ROBIN ===\n");
    printf("Digite a quantidade de processos: ");
    if (scanf("%d", &n) != 1 || n <= 0) return;

    printf("Digite o valor do QUANTUM: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) return;

    ProcessoRR proc[n];

    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("\nProcesso P%d:\n", proc[i].id);
        printf("  Tempo de Chegada: ");
        scanf("%d", &proc[i].tempo_chegada);
        printf("  Tempo de Execucao (Burst): ");
        scanf("%d", &proc[i].tempo_burst);
    }

    executarRoundRobinLogica(proc, n, quantum);
}
