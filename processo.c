#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

void remover_chaves_aspas(char *str) {
    char *inicio = strchr(str, '{');
    char *fim = strrchr(str, '}');
    if (inicio && fim && fim > inicio) {
        memmove(str, inicio + 1, fim - inicio - 1);
        str[fim - inicio - 1] = '\0';
    }
    char *aspas = strchr(str, '"');
    while (aspas) {
        memmove(aspas, aspas + 1, strlen(aspas));
        aspas = strchr(str, '"');
    }
}

int carregar_processos(Processo *processos, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char linha[1024];
    int i = 0;

    fgets(linha, sizeof(linha), arquivo); // pula o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_PROCESSOS) {
        char *token;

        token = strtok(linha, ",");
        if (!token) continue;
        processos[i].id = atol(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(processos[i].numero, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(processos[i].data_ajuizamento, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        remover_chaves_aspas(token);
        processos[i].id_classe = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        remover_chaves_aspas(token);

        // Separar assuntos
        char *ptr = strtok(token, ",");
        int assuntos[50], count = 0;
        while (ptr && count < 50) {
            assuntos[count++] = atoi(ptr);
            ptr = strtok(NULL, ",");
        }
        processos[i].id_assuntos = malloc(count * sizeof(int));
        processos[i].num_assuntos = count;
        for (int j = 0; j < count; j++) {
            processos[i].id_assuntos[j] = assuntos[j];
        }

        token = strtok(NULL, ",\n\r");
        if (!token) continue;
        processos[i].ano_eleicao = atoi(token);

        i++;
    }

    fclose(arquivo);
    return i;
}

int contarPorClasse(Processo *processos, int tamanho, int id_classe) {
    int contador = 0;
    for (int i = 0; i < tamanho; i++) {
        if (processos[i].id_classe == id_classe) {
            contador++;
        }
    }
    return contador;
}

int contar_assuntos_unicos(Processo *processos, int n) {
    int *unicos = malloc(n * 10 * sizeof(int));
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            int id = processos[i].id_assuntos[j];
            int encontrado = 0;
            for (int k = 0; k < total; k++) {
                if (unicos[k] == id) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                unicos[total++] = id;
            }
        }
    }
    free(unicos);
    return total;
}

void listar_processos_multiplos_assuntos(Processo *processos, int n) {
    printf("Processos com mais de um assunto:\n");
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("Processo ID: %ld, Número: %s, Assuntos: ", processos[i].id, processos[i].numero);
            for (int j = 0; j < processos[i].num_assuntos; j++) {
                printf("%d", processos[i].id_assuntos[j]);
                if (j < processos[i].num_assuntos - 1) printf(",");
            }
            printf("\n");
        }
    }
}

void menu(Processo *processos, int total) {
    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1. Contar por id_classe\n");
        printf("2. Contar assuntos únicos\n");
        printf("3. Listar processos com múltiplos assuntos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int id_classe;
                printf("Digite o id_classe: ");
                scanf("%d", &id_classe);
                int total_por_classe = contarPorClasse(processos, total, id_classe);
                printf("Total de processos com id_classe %d: %d\n", id_classe, total_por_classe);
                break;
            }
            case 2: {
                int total_unicos = contar_assuntos_unicos(processos, total);
                printf("Total de assuntos únicos: %d\n", total_unicos);
                break;
            }
            case 3:
                listar_processos_multiplos_assuntos(processos, total);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}
