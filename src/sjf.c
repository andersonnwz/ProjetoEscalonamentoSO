#include <stdio.h>
#include <stdlib.h>
#include "sjf.h"

// 1. Inicializa a lista
Lista* criar_lista() {
    Lista *l = (Lista*) malloc(sizeof(Lista));
    if (l != NULL) {
        l->primeiro = NULL;
        l->tamanho = 0;
    }
    return l;
}

// 2. Insere na lista já ordenado pelo menor tempo de execução (SJF)
void inserir_sjf(Lista *lista, Dados d) {
    if (lista == NULL) return;

    No *novo = (No*) malloc(sizeof(No));
    novo->informacoes = d;
    novo->proximo = NULL;

    // Se a lista estiver vazia ou o novo processo for menor que o primeiro
    if (lista->primeiro == NULL || d.tempo_execucao < lista->primeiro->informacoes.tempo_execucao) {
        novo->proximo = lista->primeiro;
        lista->primeiro = novo;
    } else {
        // Percorre a lista até encontrar a posição correta
        No *atual = lista->primeiro;
        while (atual->proximo != NULL && atual->proximo->informacoes.tempo_execucao <= d.tempo_execucao) {
            atual = atual->proximo;
        }
        novo->proximo = atual->proximo;
        atual->proximo = novo;
    }

    lista->tamanho++;
}

// 3. Remove o primeiro processo (o de menor tempo) para executar no CPU
int remover_primeiro(Lista *l, Dados *processo_removido) {
    if (l == NULL || l->primeiro == NULL) return 0;

    No *aux = l->primeiro;
    *processo_removido = aux->informacoes;
    l->primeiro = aux->proximo;

    free(aux);
    l->tamanho--;
    return 1;
}

// 4. Liberta a memória alocada
void libertar_lista(Lista *l) {
    if (l == NULL) return;

    No *atual = l->primeiro;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(l);
}

void executar_menu_sjf() {
    Lista *fila_sjf = criar_lista();
    if (fila_sjf == NULL) {
        printf("Erro ao alocar memória para a lista!\n");
        return;
    }

    int opcao = -1;
    int id_contador = 1;

    while (opcao != 0) {
        printf("\n=========================================\n");
        printf("      SISTEMA DE ESCALONAMENTO SJF      \n");
        printf("=========================================\n");
        printf("1. Adicionar novo processo\n");
        printf("2. Executar proximo processo da fila\n");
        printf("3. Executar TODOS os processos\n");
        printf("4. Ver quantidade de processos pendentes\n");
        printf("0. Sair do sistema\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Dados p;
                p.id = id_contador++;
                printf("\n--- Criar Processo ID %d ---\n", p.id);
                
                printf("Tempo de Chegada (Arrival Time): ");
                scanf("%d", &p.tempo_chegada);
                
                printf("Tempo de Execucao (Burst Time): ");
                scanf("%d", &p.tempo_execucao);

                inserir_sjf(fila_sjf, p);
                printf(">> Processo %d adicionado com sucesso!\n", p.id);
                break;
            }
            case 2: {
                Dados p;
                if (remover_primeiro(fila_sjf, &p)) {
                    printf("\n>> EXECUTANDO -> Processo ID: %d | Burst Time: %d | Chegada: %d\n",
                           p.id, p.tempo_execucao, p.tempo_chegada);
                } else {
                    printf("\n>> Nenhum processo na fila para executar.\n");
                }
                break;
            }
            case 3: {
                Dados p;
                if (fila_sjf->tamanho == 0) {
                    printf("\n>> A fila esta vazia.\n");
                } else {
                    printf("\n--- Ordem de Execucao Completa ---\n");
                    int ordem = 1;
                    while (remover_primeiro(fila_sjf, &p)) {
                        printf("%dº a executar -> Processo ID: %d | Burst Time: %d | Chegada: %d\n",
                               ordem++, p.id, p.tempo_execucao, p.tempo_chegada);
                    }
                }
                break;
            }
            case 4:
                printf("\n>> Total de processos aguardando na fila: %d\n", fila_sjf->tamanho);
                break;

            case 0:
                printf("\nEncerrando o sistema SJF...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }
    }

    libertar_lista(fila_sjf);
}