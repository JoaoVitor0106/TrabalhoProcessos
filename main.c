#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    Processo processos[MAX_PROCESSOS];
    int total = carregar_processos(processos, "processo_043_202409032338.csv");

    menu(processos, total);

    return 0;
}
