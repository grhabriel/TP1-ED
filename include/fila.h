#ifndef FILA_H
#define FILA_H

/*
 * Estrutura que representa uma encomenda feita à forja por um aventureiro.
 */
typedef struct {
    int id_aventureiro;
    int id_item;
} Encomenda;

/*
 * Declaração opaca do TAD Fila.
 */
typedef struct Fila Fila;

/*
 * Cria dinamicamente uma nova encomenda com os identificadores informados.
 * Retorna o ponteiro para a encomenda ou NULL em caso de falha de alocação.
 */
Encomenda *criar_encomenda(int id_aventureiro, int id_item);

/*
 * Libera a memória alocada para uma encomenda.
 */
void destruir_encomenda(Encomenda *encomenda);

/*
 * Cria e aloca dinamicamente uma nova fila vazia.
 * Retorna o ponteiro para a fila criada ou NULL em caso de falha de alocação.
 */
Fila *criar_fila(void);

/*
 * Libera toda a memória alocada para as células e encomendas restantes na fila,
 * além da própria estrutura da fila.
 */
void destruir_fila(Fila *fila);

/*
 * Insere uma encomenda no fim da fila.
 * Retorna a nova posição da encomenda na fila (contada a partir de 1) ou -1 em caso de erro.
 */
int enfileirar(Fila *fila, Encomenda *encomenda);

/*
 * Remove e retorna a encomenda do início da fila.
 * Retorna NULL caso a fila esteja vazia.
 */
Encomenda *desenfileirar(Fila *fila);

/*
 * Retorna 1 se a fila estiver vazia e 0 caso contrário.
 */
int fila_vazia(const Fila *fila);

/*
 * Retorna a quantidade de encomendas atualmente na fila.
 */
int tamanho_fila(const Fila *fila);

#endif // FILA_H
