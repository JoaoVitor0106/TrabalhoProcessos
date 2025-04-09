//processo.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

#define LINHA_MAX 256
#define MAX_PROCESSOS 20000

void remover_chaves(char *str) {
    char *inicio = strchr(str, '{');
    char *fim = strrchr(str, '}');

    if (inicio && fim && fim > inicio) {
        memmove(str, inicio + 1, fim - inicio - 1);
        str[fim - inicio - 1] = '\0';
    }
}


void remover_chaves_aspas(char *str) {
    // Remove aspas externas (se existirem)
    if (str[0] == '"') {
        memmove(str, str + 1, strlen(str)); // remove a primeira aspa
        char *ultima_aspa = strrchr(str, '"');
        if (ultima_aspa) *ultima_aspa = '\0'; // remove a última
    }

    remover_chaves(str); // Remove chaves
}

int carregar_processos(Processo *processos, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char linha[1024];
    int total = 0;

    // Ignora o cabeçalho, se houver
    if (!fgets(linha, sizeof(linha), arquivo)) {
        fprintf(stderr, "Erro ao ler o cabeçalho.\n");
        fclose(arquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) && total < MAX_PROCESSOS) {
        // Remove newline final
        linha[strcspn(linha, "\r\n")] = '\0';

        // Tokenização dos campos
        char *campos[6];
        int campo = 0;
        char *ptr = linha;
        int dentro_aspas = 0;
        campos[campo++] = ptr;

        while (*ptr && campo < 6) {
            if (*ptr == '"') {
                dentro_aspas = !dentro_aspas;
            } else if (*ptr == ',' && !dentro_aspas) {
                *ptr = '\0';
                campos[campo++] = ptr + 1;
            }
            ptr++;
        }

        if (campo < 6) {
            fprintf(stderr, "Linha mal formatada. Ignorando.\n");
            continue;
        }

        // ID
        processos[total].id = atol(campos[0]);

        // Número
        strncpy(processos[total].numero, campos[1], sizeof(processos[total].numero) - 1);
        processos[total].numero[sizeof(processos[total].numero) - 1] = '\0';

        // Data
        strncpy(processos[total].data_ajuizamento, campos[2], sizeof(processos[total].data_ajuizamento) - 1);
        processos[total].data_ajuizamento[sizeof(processos[total].data_ajuizamento) - 1] = '\0';

        // id_classe (remover chaves)
        char temp_classe[32];
        strncpy(temp_classe, campos[3], sizeof(temp_classe) - 1);
        temp_classe[sizeof(temp_classe) - 1] = '\0';
        remover_chaves_aspas(temp_classe);
        processos[total].id_classe = atoi(temp_classe);

        // id_assuntos (remover aspas e chaves)
        char temp_assuntos[256];
        strncpy(temp_assuntos, campos[4], sizeof(temp_assuntos) - 1);
        temp_assuntos[sizeof(temp_assuntos) - 1] = '\0';
        remover_chaves_aspas(temp_assuntos);

        // Contar número de assuntos (se houver vírgulas)
        int count = 1;
        for (char *c = temp_assuntos; *c; c++) {
            if (*c == ',') count++;
        }

        processos[total].num_assuntos = count;
        processos[total].id_assuntos = malloc(count * sizeof(int));
        if (!processos[total].id_assuntos) {
            fprintf(stderr, "Erro de alocação.\n");
            fclose(arquivo);
            return 0;
        }

        // Preencher id_assuntos
        char *token = strtok(temp_assuntos, ",");
        int j = 0;
        while (token && j < count) {
            processos[total].id_assuntos[j++] = atoi(token);
            token = strtok(NULL, ",");
        }

        // Ano eleição
        processos[total].ano_eleicao = atoi(campos[5]);

        total++;
    }

    fclose(arquivo);
    return total;
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
    int capacidade = 1000;
    int *assuntos_unicos = malloc(capacidade * sizeof(int));
    int num_unicos = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            int id = processos[i].id_assuntos[j];
            int encontrado = 0;
            for (int k = 0; k < num_unicos; k++) {
                if (assuntos_unicos[k] == id) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                if (num_unicos >= capacidade) {
                    capacidade *= 2;
                    assuntos_unicos = realloc(assuntos_unicos, capacidade * sizeof(int));
                    if (!assuntos_unicos) {
                        fprintf(stderr, "Erro de realocação de memória\n");
                        return -1;
                    }
                }
                assuntos_unicos[num_unicos++] = id;
            }
        }
    }

    free(assuntos_unicos);
    return num_unicos;
}


void listar_processos_multiplos_assuntos(Processo *processos, int n) {
    printf("Processos com mais de um assunto:\n");
    int encontrou = 0;
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("Processo ID: %ld, Número: %s, Assuntos: ", processos[i].id, processos[i].numero);
            for (int j = 0; j < processos[i].num_assuntos; j++) {
                printf("%d", processos[i].id_assuntos[j]);
                if (j < processos[i].num_assuntos - 1) {
                    printf(", ");
                }
            }
            printf("\n");
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Nenhum processo com múltiplos assuntos encontrado.\n");
    }
}

void opcao_contar_por_classe(Processo *processos, int total) {
    int id_classe;
    printf("Digite o id_classe para contar: ");
    scanf("%d", &id_classe);
    int resultado = contarPorClasse(processos, total, id_classe);
    printf("Total de processos com id_classe %d: %d\n", id_classe, resultado);
}

void opcao_contar_assuntos_unicos(Processo *processos, int total) {
    int resultado = contar_assuntos_unicos(processos, total);
    printf("Total de assuntos únicos: %d\n", resultado);
}
