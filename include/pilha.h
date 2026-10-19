#ifndef PILHA_H
#define PILHA_H

#include "item.h"

/*
 * Declaração da Pilha (representa a build do aventureiro).
 */
typedef struct Pilha Pilha;

/*
 * Cria e aloca dinamicamente uma nova pilha vazia.
 * Retorna o ponteiro para a pilha criada ou NULL em caso de falha de alocação.
 */
Pilha *criar_pilha(void);

/*
 * Libera toda a memória das células da pilha e da própria estrutura.
 * Nota: Os itens em si não são liberados aqui, pois pertencem ao catálogo geral.
 */
void destruir_pilha(Pilha *pilha);

/*
 * Insere um item no topo da pilha (equipa o item na build).
 * Retorna 1 em caso de sucesso e 0 em caso de falha de alocação.
 */
int empilhar(Pilha *pilha, Item *item);

/*
 * Remove e retorna o item do topo da pilha (desequipa a peça mais recente).
 * Retorna NULL caso a pilha esteja vazia.
 */
Item *desempilhar(Pilha *pilha);

/*
 * Retorna o item do topo da pilha sem removê-lo.
 * Retorna NULL caso a pilha esteja vazia.
 */
Item *topo_pilha(const Pilha *pilha);

/*
 * Retorna 1 se a pilha estiver vazia e 0 caso contrário.
 */
int pilha_vazia(const Pilha *pilha);

/*
 * Retorna a quantidade de itens atualmente equipados na pilha.
 */
int tamanho_pilha(const Pilha *pilha);

/*
 * Verifica se um determinado item (por id) já está equipado na pilha.
 * Retorna 1 se o item estiver equipado e 0 caso contrário.
 */
int pilha_contem_item(const Pilha *pilha, int id_item);

#endif // PILHA_H
