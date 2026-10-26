#include <stdio.h>
#include <stdlib.h>
#include "fcfs.h"

// Inicializa a fila na memória dinâmica
FilaFCFS* criar_fila_fcfs() {
    FilaFCFS *fila = (FilaFCFS*) malloc(sizeof(FilaFCFS));
    if (fila != NULL) {
        fila->inicio = NULL;
        fila->fim = NULL;
        fila->tamanho = 0;
    }
    return fila;
}

// Insere um processo no fim da fila (Alocação Dinâmica por Nó)
void enfileirar_fcfs(FilaFCFS *fila, ProcessoFCFS p) {
    if (fila == NULL) return;

    NoFCFS *novo = (NoFCFS*) malloc(sizeof(NoFCFS));
    if (novo == NULL) {
        printf(">> Erro: Falha ao alocar memoria para o processo!\n");
        return;
    }

    novo->processo = p;
    novo->proximo = NULL;

    if (fila->inicio == NULL) {
        fila->inicio = novo;
    } else {
        fila->fim->proximo = novo;
    }

    fila->fim = novo;
    fila->tamanho++;
}

// Remove o processo do início da fila (FIFO)
int desenfileirar_fcfs(FilaFCFS *fila, ProcessoFCFS *p_saindo) {
    if (fila == NULL || fila->inicio == NULL) return 0;

    NoFCFS *aux = fila->inicio;
    *p_saindo = aux->processo;

    fila->inicio = aux->proximo;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(aux); // Libera o nó desalocado da memória
    fila->tamanho--;
    return 1;
}

// Libera toda a memória alocada dinamicamente caso restem nós
void liberar_fila_fcfs(FilaFCFS *fila) {
    if (fila == NULL) return;

    NoFCFS *atual = fila->inicio;
    while (atual != NULL) {
        NoFCFS *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(fila);
}

// Função para ordenar um vetor temporário por tempo de chegada (Bubble Sort)
static void ordenar_por_chegada(ProcessoFCFS v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (v[i].chegada > v[j].chegada) {
                ProcessoFCFS temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }
}

// Menu principal do FCFS
void executar_menu_fcfs() {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n===================================\n");
        printf("       SISTEMA DE ESCALONAMENTO FCFS\n");
        printf("===================================\n");
        printf("1. Inserir processos e Simular FCFS\n");
        printf("0. Sair para o menu principal\n");
        printf("-----------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int n;
                printf("\nDigite o numero de processos: ");
                scanf("%d", &n);

                if (n <= 0) {
                    printf(">> Quantidade invalida!\n");
                    break;
                }

                // Alocação dinâmica do vetor de entrada inicial
                ProcessoFCFS *entradas = (ProcessoFCFS*) malloc(n * sizeof(ProcessoFCFS));
                if (entradas == NULL) {
                    printf(">> Erro de alocacao de memoria!\n");
                    break;
                }

                // 1. Leitura dos dados dos processos
                for (int i = 0; i < n; i++) {
                    entradas[i].id = i + 1;
                    printf("\nProcesso P%d:\n", entradas[i].id);
                    printf("Tempo de Chegada: ");
                    scanf("%d", &entradas[i].chegada);
                    printf("Tempo de Execucao (Burst): ");
                    scanf("%d", &entradas[i].burst);
                }

                // 2. Ordena os processos por ordem de chegada
                ordenar_por_chegada(entradas, n);

                // 3. Cria a Fila Dinâmica e insere na ordem correta
                FilaFCFS *fila = criar_fila_fcfs();
                for (int i = 0; i < n; i++) {
                    enfileirar_fcfs(fila, entradas[i]);
                }

                // Liberamos o vetor auxiliar de entrada pois já populamos a Fila Dinâmica
                free(entradas);

                // 4. Executa o escalonamento retirando da Fila (FIFO)
                int tempo_atual = 0;
                float soma_espera = 0, soma_turnaround = 0;

                printf("\n=========================================================\n");
                printf("ID\tChegada\tBurst\tEspera\tTurnaround\tTermino\n");
                printf("=========================================================\n");

                ProcessoFCFS p;
                while (desenfileirar_fcfs(fila, &p)) {
                    // CPU ociosa aguardando o processo chegar
                    if (tempo_atual < p.chegada) {
                        tempo_atual = p.chegada;
                    }

                    p.termino = tempo_atual + p.burst;
                    p.turnaround = p.termino - p.chegada;
                    p.espera = p.turnaround - p.burst;
                    tempo_atual = p.termino;

                    soma_espera += p.espera;
                    soma_turnaround += p.turnaround;

                    printf("P%d\t%d\t%d\t%d\t%d\t\t%d\n",
                           p.id, p.chegada, p.burst, p.espera, p.turnaround, p.termino);
                }

                printf("=========================================================\n");
                printf("Tempo Medio de Espera: %.2f\n", soma_espera / n);
                printf("Tempo Medio de Turnaround: %.2f\n", soma_turnaround / n);

                // Destrói a estrutura da fila da memória
                liberar_fila_fcfs(fila);
                break;
            }
            case 0:
                printf("\nEncerando modulo FCFS...\n");
                break;
            default:
                printf("\n>> Opcao invalida!\n");
                break;
        }
    }
}