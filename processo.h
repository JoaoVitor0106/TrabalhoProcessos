#ifndef PROCESSO_H
#define PROCESSO_H

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
int contarPorClasse(Processo *processos, int tamanho, int id_classe);
int contar_assuntos_unicos(Processo *processos, int n);
void listar_processos_multiplos_assuntos(Processo *processos, int n);
void remover_chaves_aspas(char *str);
void opcao_contar_por_classe(Processo *processos, int total);
void opcao_contar_assuntos_unicos(Processo *processos, int total);
void ordenar_por_data_e_salvar(Processo *processos, int total, const char *nome_saida);
void ordenar_por_id_e_salvar(Processo *processos, int total, const char *nome_saida);

#endif
