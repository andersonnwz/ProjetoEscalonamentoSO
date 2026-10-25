#include <stdio.h>
#include "sjf.h"
#include "rr.h"

int main() {
    int opcao;

    printf("1 - SJF\n");
    printf("2 - Round Robin\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        executar_menu_sjf();
    } else if (opcao == 2) {
        executar_menu_rr();
    } else {
        printf("Invalido\n");
    }

    return 0;
}
