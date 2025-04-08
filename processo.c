// processo.c
#include "processo.h"

int contarPorClasse(Processo *processos, int tamanho, int id_classe) {
    int contador = 0;
    for (int i = 0; i < tamanho; i++) {
        if (processos[i].id_classe == id_classe) {
            contador++;
        }
    }
    return contador;
}
