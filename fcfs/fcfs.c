#include <stdio.h>

typedef struct {
    int id;
    int chegada;
    int burst;
    int espera;
    int turnaround;
    int termino;
} Processo;

int main() {
    int n;
    
    printf("Simulador de FCFS\n");
    printf("Digite o numero de processos: ");
    scanf("%d", &n);

    Processo p[n];
    Processo temp; 
    
    
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("\nProcesso %d:\n", p[i].id);
        printf("Tempo chegada: ");
        scanf("%d", &p[i].chegada);
        printf("Tempo de execucao: ");
        scanf("%d", &p[i].burst);
    } 

   
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].chegada > p[j].chegada) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    
    int tempo_atual = 0;
    for (int i = 0; i < n; i++) {
        if (tempo_atual < p[i].chegada) {
            tempo_atual = p[i].chegada;            
        }
        
        p[i].termino = tempo_atual + p[i].burst;
        p[i].turnaround = p[i].termino - p[i].chegada;
        p[i].espera = p[i].turnaround - p[i].burst;
        tempo_atual = p[i].termino;
    }

    
    printf("\n=========================================\n");
    printf("ID\tChegada\tBurst\tEspera\tTurnaround\tTermino\n");
    printf("=========================================\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t\t%d\n", 
            p[i].id, p[i].chegada, p[i].burst, p[i].espera, p[i].turnaround, p[i].termino);
    }
    printf("=========================================\n");

    return 0;
}