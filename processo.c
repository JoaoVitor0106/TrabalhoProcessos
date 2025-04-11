#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

#define MAX_PROCESSOS 20000

void remover_chaves_aspas(char *str) {
    if (str[0] == '"') {
        memmove(str, str + 1, strlen(str));
        char *ultima_aspa = strrchr(str, '"');
        if (ultima_aspa) *ultima_aspa = '\0';
    }

    char *inicio = strchr(str, '{');
    char *fim = strrchr(str, '}');
    if (inicio && fim && fim > inicio) {
        memmove(str, inicio + 1, fim - inicio - 1);
        str[fim - inicio - 1] = '\0';
    }
}

int carregar_processos(Processo *processos, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char linha[1024];
    int total = 0;

    fgets(linha, sizeof(linha), arquivo); // ignora cabeçalho

    while (fgets(linha, sizeof(linha), arquivo) && total < MAX_PROCESSOS) {
        char *campos[6];
        int campo = 0, dentro_aspas = 0;
        char *ptr = linha;
        campos[campo++] = ptr;

        while (*ptr && campo < 6) {
            if (*ptr == '"') dentro_aspas = !dentro_aspas;
            else if (*ptr == ',' && !dentro_aspas) {
                *ptr = '\0';
                campos[campo++] = ptr + 1;
            }
            ptr++;
        }

        if (campo < 6) continue;

        processos[total].id = atol(campos[0]);
        strncpy(processos[total].numero, campos[1], sizeof(processos[total].numero));
        strncpy(processos[total].data_ajuizamento, campos[2], sizeof(processos[total].data_ajuizamento));

        char temp_classe[32];
        strncpy(temp_classe, campos[3], sizeof(temp_classe));
        remover_chaves_aspas(temp_classe);
        processos[total].id_classe = atoi(temp_classe);

        char temp_assuntos[256];
        strncpy(temp_assuntos, campos[4], sizeof(temp_assuntos));
        remover_chaves_aspas(temp_assuntos);

        int count = 1;
        for (char *c = temp_assuntos; *c; c++) {
            if (*c == ',') count++;
        }

        processos[total].num_assuntos = count;
        processos[total].id_assuntos = malloc(count * sizeof(int));

        char *token = strtok(temp_assuntos, ",");
        int j = 0;
        while (token && j < count) {
            processos[total].id_assuntos[j++] = atoi(token);
            token = strtok(NULL, ",");
        }

        processos[total].ano_eleicao = atoi(campos[5]);
        total++;
    }

    fclose(arquivo);
    return total;
}

int contarPorClasse(Processo *processos, int tamanho, int id_classe) {
    int contador = 0;
    for (int i = 0; i < tamanho; i++) {
        if (processos[i].id_classe == id_classe)
            contador++;
    }
    return contador;
}

int contar_assuntos_unicos(Processo *processos, int n) {
    int capacidade = 1000;
    int *unicos = malloc(capacidade * sizeof(int));
    int total = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            int id = processos[i].id_assuntos[j];
            int existe = 0;
            for (int k = 0; k < total; k++) {
                if (unicos[k] == id) {
                    existe = 1;
                    break;
                }
            }
            if (!existe) {
                if (total >= capacidade) {
                    capacidade *= 2;
                    unicos = realloc(unicos, capacidade * sizeof(int));
                }
                unicos[total++] = id;
            }
        }
    }

    free(unicos);
    return total;
}

void listar_processos_multiplos_assuntos(Processo *processos, int n) {
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("Processo ID: %ld, Número: %s, Assuntos: ", processos[i].id, processos[i].numero);
            for (int j = 0; j < processos[i].num_assuntos; j++) {
                printf("%d", processos[i].id_assuntos[j]);
                if (j < processos[i].num_assuntos - 1) printf(", ");
            }
            printf("\n");
        }
    }
}

int comparar_datas_desc(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    return strcmp(p2->data_ajuizamento, p1->data_ajuizamento);
}

void ordenar_por_data_e_salvar(Processo *processos, int total, const char *nome_saida) {
    qsort(processos, total, sizeof(Processo), comparar_datas_desc);

    FILE *f = fopen(nome_saida, "w");
    if (!f) {
        perror("Erro ao criar o arquivo CSV");
        return;
    }

    fprintf(f, "id,numero,data_ajuizamento,id_classe,id_assuntos,ano_eleicao\n");

    for (int i = 0; i < total; i++) {
        fprintf(f, "%ld,%s,%s,%d,\"{", 
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento,
                processos[i].id_classe);

        for (int j = 0; j < processos[i].num_assuntos; j++) {
            fprintf(f, "%d", processos[i].id_assuntos[j]);
            if (j < processos[i].num_assuntos - 1) fprintf(f, ",");
        }

        fprintf(f, "}\",%d\n", processos[i].ano_eleicao);
    }

    fclose(f);
    printf("Processos salvos em %s\n", nome_saida);
}

int comparar_ids_crescente(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    return (p1->id > p2->id) - (p1->id < p2->id);
}

void ordenar_por_id_e_salvar(Processo *processos, int total, const char *nome_saida) {
    qsort(processos, total, sizeof(Processo), comparar_ids_crescente);

    FILE *f = fopen(nome_saida, "w");
    if (!f) {
        perror("Erro ao criar o arquivo CSV");
        return;
    }

    fprintf(f, "id,numero,data_ajuizamento,id_classe,id_assuntos,ano_eleicao\n");

    for (int i = 0; i < total; i++) {
        fprintf(f, "%ld,%s,%s,%d,\"{", 
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento,
                processos[i].id_classe);

        for (int j = 0; j < processos[i].num_assuntos; j++) {
            fprintf(f, "%d", processos[i].id_assuntos[j]);
            if (j < processos[i].num_assuntos - 1) fprintf(f, ",");
        }

        fprintf(f, "}\",%d\n", processos[i].ano_eleicao);
    }

    fclose(f);
    printf("Processos salvos em %s\n", nome_saida);
}

void opcao_contar_por_classe(Processo *processos, int total) {
    int id_classe;
    printf("Digite o id_classe: ");
    scanf("%d", &id_classe);
    int r = contarPorClasse(processos, total, id_classe);
    printf("Total de processos com id_classe %d: %d\n", id_classe, r);
}

void opcao_contar_assuntos_unicos(Processo *processos, int total) {
    int total_unicos = contar_assuntos_unicos(processos, total);
    printf("Total de assuntos únicos: %d\n", total_unicos);
}
