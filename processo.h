// processo.h
#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct {
    long id;    //Identificador do processo
    char numero[30];    //Número do processo
    char data_ajuizamento[25];  // Data do Ajuizamento
    int id_classe;  //ID da classe
    int* id_assuntos;     //Array de id Assuntos (pode ser multiplo)
    int num_assuntos;   //Número de assuntos associados ao processo
    int ano_eleicao;    //Ano de eleição
} Processo;

int contarPorClasse(Processo *processos, int tamanho, int id_classe);
int contar_assuntos_unicos(Processo *processos, int n);
void listar_processos_multiplos_assuntos(Processo *processos, int n);

#endif
