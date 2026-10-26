#include <stdlib.h>
#include "../include/fila.h"

/*
 * Célula interna da fila encadeada, armazenando o ponteiro para Encomenda.
 */
typedef struct Celula {
    Encomenda *encomenda;
    struct Celula *proxima;
} Celula;

/*
 * Definição da estrutura Fila com controle de início, fim e tamanho.
 */
struct Fila {
    Celula *inicio;
    Celula *fim;
    int tamanho;
};

Encomenda *criar_encomenda(int id_aventureiro, int id_item) {
    Encomenda *encomenda = (Encomenda *)malloc(sizeof(Encomenda));
    if (encomenda == NULL) {
        return NULL;
    }
    encomenda->id_aventureiro = id_aventureiro;
    encomenda->id_item = id_item;
    return encomenda;
}

void destruir_encomenda(Encomenda *encomenda) {
    if (encomenda != NULL) {
        free(encomenda);
    }
}

Fila *criar_fila(void) {
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    if (fila == NULL) {
        return NULL;
    }
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    return fila;
}

void destruir_fila(Fila *fila) {
    if (fila == NULL) {
        return;
    }
    Celula *atual = fila->inicio;
    while (atual != NULL) {
        Celula *proxima = atual->proxima;
        if (atual->encomenda != NULL) {
            destruir_encomenda(atual->encomenda);
        }
        free(atual);
        atual = proxima;
    }
    free(fila);
}

int enfileirar(Fila *fila, Encomenda *encomenda) {
    if (fila == NULL || encomenda == NULL) {
        return -1;
    }

    Celula *nova_celula = (Celula *)malloc(sizeof(Celula));
    if (nova_celula == NULL) {
        return -1;
    }

    nova_celula->encomenda = encomenda;
    nova_celula->proxima = NULL;

    if (fila->inicio == NULL) {
        fila->inicio = nova_celula;
        fila->fim = nova_celula;
    } else {
        fila->fim->proxima = nova_celula;
        fila->fim = nova_celula;
    }

    fila->tamanho++;
    return fila->tamanho; // Retorna a posição 1-indexada da encomenda
}

Encomenda *desenfileirar(Fila *fila) {
    if (fila == NULL || fila->inicio == NULL) {
        return NULL;
    }

    Celula *celula_removida = fila->inicio;
    Encomenda *encomenda_removida = celula_removida->encomenda;

    fila->inicio = celula_removida->proxima;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(celula_removida);
    fila->tamanho--;

    return encomenda_removida;
}

int fila_vazia(const Fila *fila) {
    if (fila == NULL) {
        return 1;
    }
    return fila->tamanho == 0;
}

int tamanho_fila(const Fila *fila) {
    if (fila == NULL) {
        return 0;
    }
    return fila->tamanho;
}
