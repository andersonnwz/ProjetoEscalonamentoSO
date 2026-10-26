#include <stdio.h>
#include "sjf.h"
#include "rr.h"
#include "fcfs.h"

int main() {
    int opcao;

    printf("1 - FCFS\n");
    printf("2 - SJF\n");
    printf("3 - Round Robin\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        executar_menu_fcfs();
    } else if (opcao == 2) {
        executar_menu_sjf();
    } else if (opcao == 3){
        executar_menu_rr();
    } else {
        printf("Invalido\n");
    }

    return 0;
}
