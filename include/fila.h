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


void destruir_encomenda(Encomenda *encomenda);


Fila *criar_fila(void);


void destruir_fila(Fila *fila);
 
int enfileirar(Fila *fila, Encomenda *encomenda);

Encomenda *desenfileirar(Fila *fila);

int fila_vazia(const Fila *fila);

int tamanho_fila(const Fila *fila);

#endif // FILA_H
