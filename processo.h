#ifndef PROCESSO_H
#define PROCESSO_H
#define MAX_PROCESSOS 20000

typedef struct {
    long id;
    char numero[30];
    char data_ajuizamento[25];
    int id_classe;
    int* id_assuntos;
    int num_assuntos;
    int ano_eleicao;
} Processo;

int carregar_processos(Processo *processos, const char *nome_arquivo);
void remover_chaves_aspas(char *str);
int contarPorClasse(Processo *processos, int tamanho, int id_classe);
int contar_assuntos_unicos(Processo *processos, int n);
void listar_processos_multiplos_assuntos(Processo *processos, int n);
void menu(Processo *processos, int total);

#endif
