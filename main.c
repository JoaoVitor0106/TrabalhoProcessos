// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

#define MAX_PROCESSOS 20000

void remover_chaves(char *str) {
    char *inicio = strchr(str, '{');
    char *fim = strchr(str, '}');
    if (inicio && fim) {
        memmove(str, inicio + 1, fim - inicio - 1);
        str[fim - inicio - 1] = '\0';
    }
}

int carregar_processos(Processo *processos, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char linha[256];
    int i = 0;

    fgets(linha, sizeof(linha), arquivo); // Pular o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_PROCESSOS) {
        char *token = strtok(linha, ",");
        processos[i].id = atol(token);

        token = strtok(NULL, ",");
        strcpy(processos[i].numero, token);

        token = strtok(NULL, ",");
        strcpy(processos[i].data_ajuizamento, token);

        token = strtok(NULL, ",");
        remover_chaves(token);
        processos[i].id_classe = atoi(token);

        token = strtok(NULL, ",");
        remover_chaves(token);
        processos[i].num_assuntos = 1;
        processos[i].id_assuntos = malloc(sizeof(int));
        processos[i].id_assuntos[0] = atoi(token);

        token = strtok(NULL, ",");
        processos[i].ano_eleicao = atoi(token);

        i++;
    }

    fclose(arquivo);
    return i;
}

int main() {
    Processo processos[MAX_PROCESSOS];
    int total = carregar_processos(processos, "processo_043_202409032338.csv");

    int id_classe;
    printf("Digite o id_classe para contar: ");
    scanf("%d", &id_classe);

    int total_por_classe = contarPorClasse(processos, total, id_classe);
    printf("Total de processos com id_classe %d: %d\n", id_classe, total_por_classe);

    return 0;
}
