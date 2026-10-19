#ifndef ITEM_H
#define ITEM_H

/*
 * Estrutura que representa um item cadastrado na forja.
 */
typedef struct {
    int id;
    char *nome;
} Item;

/*
 * Cria e aloca dinamicamente um novo item com seu ID e nome.
 * Uma cópia da string nome é alocada internamente.
 * Retorna o ponteiro para o Item criado ou NULL em caso de falha.
 */
Item *criar_item(int id, const char *nome);

/*
 * Libera a memória associada ao item e ao seu nome.
 */
void destruir_item(Item *item);

/*
 * Retorna o identificador do item.
 */
int obter_id_item(const Item *item);

/*
 * Retorna o ponteiro para a string com o nome do item.
 */
const char *obter_nome_item(const Item *item);

#endif // ITEM_H
