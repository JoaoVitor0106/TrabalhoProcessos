// processo.c
#include "processo.h"

int contarPorClasse(Processo *processos, int tamanho, int id_classe) 
{
    int contador = 0;
    for (int i = 0; i < tamanho; i++) {
        if (processos[i].id_classe == id_classe) {
            contador++;
        }
    }
    return contador;
}

int contar_assuntos_unicos(Processo *processos, int n)
{
    int *assuntos_unicos = (int*)malloc(n * sizeof(int));
    int num_unicos = 0;

    // Verificando todos os processos e seus assuntos
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            int encontrado = 0;
            // Verificando se o id_assunto já foi contado
            for (int k = 0; k < num_unicos; k++) {
                if (assuntos_unicos[k] == processos[i].id_assuntos[j]) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                assuntos_unicos[num_unicos++] = processos[i].id_assuntos[j];
            }
        }
    }

    free(assuntos_unicos);
    return num_unicos;
}

void listar_processos_multiplos_assuntos(Processo *processos, int n) 
{
    printf("Processos com mais de um assunto:\n");
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("Processo ID: %ld, Número: %s\n", processos[i].id, processos[i].numero);
        }
    }
}