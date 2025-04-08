// processo.h
#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct {
    long id;
    char numero[30];
    char data_ajuizamento[25];
    int id_classe;
    int id_assunto[10];
    int ano_eleicao;
} Processo;

int contarPorClasse(Processo *processos, int tamanho, int id_classe);

#endif
