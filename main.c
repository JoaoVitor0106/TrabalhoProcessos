//main.c
#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

#define MAX_PROCESSOS 20000

int main() {
    Processo processos[MAX_PROCESSOS];
    int total = carregar_processos(processos, "processo_043_202409032338.csv");
    if (total == 0) return 1;

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Contar processos por id_classe\n");
        printf("2. Contar assuntos únicos\n");
        printf("3. Listar processos com múltiplos assuntos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                opcao_contar_por_classe(processos, total);
                break;
            case 2:
                opcao_contar_assuntos_unicos(processos, total);
                break;
            case 3:
                listar_processos_multiplos_assuntos(processos, total);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    // Liberar memória alocada
    for (int i = 0; i < total; i++) {
        free(processos[i].id_assuntos);
    }

    printf("Pressione Enter para sair...");
    getchar(); // Aguarda o usuário pressionar Enter

    return 0;
}
