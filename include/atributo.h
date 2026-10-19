#ifndef ATRIBUTO_H
#define ATRIBUTO_H

/*
 * Estrutura que representa um atributo no sistema.
 */
typedef struct {
    int id;
    char *nome;
} Atributo;

/*
 * Cria e aloca dinamicamente um novo atributo com seu ID e nome.
 * Uma cópia da string nome é alocada internamente.
 * Retorna o ponteiro para o Atributo criado ou NULL em caso de falha.
 */
Atributo *criar_atributo(int id, const char *nome);

/*
 * Libera a memória associada ao atributo e ao seu nome.
 */
void destruir_atributo(Atributo *atributo);

/*
 * Retorna o identificador do atributo.
 */
int obter_id_atributo(const Atributo *atributo);

/*
 * Retorna o ponteiro para a string com o nome do atributo.
 */
const char *obter_nome_atributo(const Atributo *atributo);

#endif // ATRIBUTO_H
