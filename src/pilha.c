#include <stdlib.h>
#include "../include/pilha.h"

/*
 * Célula interna da pilha encadeada.
 * Armazena diretamente o ponteiro para o Item equipado.
 */
typedef struct CelulaPilha {
    Item *item;
    struct CelulaPilha *abaixo;
} CelulaPilha;

/*
 * Definição da estrutura Pilha com ponteiro para o topo e contagem de itens.
 */
struct Pilha {
    CelulaPilha *topo;
    int tamanho;
};

Pilha *criar_pilha(void) {
    Pilha *pilha = (Pilha *)malloc(sizeof(Pilha));
    if (pilha == NULL) {
        return NULL;
    }
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

void destruir_pilha(Pilha *pilha) {
    if (pilha == NULL) {
        return;
    }
    CelulaPilha *atual = pilha->topo;
    while (atual != NULL) {
        CelulaPilha *abaixo = atual->abaixo;
        free(atual);
        atual = abaixo;
    }
    free(pilha);
}

int empilhar(Pilha *pilha, Item *item) {
    if (pilha == NULL || item == NULL) {
        return 0;
    }

    CelulaPilha *nova_celula = (CelulaPilha *)malloc(sizeof(CelulaPilha));
    if (nova_celula == NULL) {
        return 0;
    }

    nova_celula->item = item;
    nova_celula->abaixo = pilha->topo;
    pilha->topo = nova_celula;
    pilha->tamanho++;

    return 1;
}

Item *desempilhar(Pilha *pilha) {
    if (pilha == NULL || pilha->topo == NULL) {
        return NULL;
    }

    CelulaPilha *celula_removida = pilha->topo;
    Item *item_removido = celula_removida->item;

    pilha->topo = celula_removida->abaixo;
    free(celula_removida);
    pilha->tamanho--;

    return item_removido;
}

Item *topo_pilha(const Pilha *pilha) {
    if (pilha == NULL || pilha->topo == NULL) {
        return NULL;
    }
    return pilha->topo->item;
}

int pilha_vazia(const Pilha *pilha) {
    if (pilha == NULL) {
        return 1;
    }
    return pilha->tamanho == 0;
}

int tamanho_pilha(const Pilha *pilha) {
    if (pilha == NULL) {
        return 0;
    }
    return pilha->tamanho;
}

int pilha_contem_item(const Pilha *pilha, int id_item) {
    if (pilha == NULL) {
        return 0;
    }
    CelulaPilha *atual = pilha->topo;
    while (atual != NULL) {
        if (atual->item != NULL && atual->item->id == id_item) {
            return 1;
        }
        atual = atual->abaixo;
    }
    return 0;
}

int pilha_obter_itens(const Pilha *pilha, Item ***itens_saida) {
    if (itens_saida == NULL) {
        return 0;
    }
    *itens_saida = NULL;
    if (pilha == NULL || pilha->tamanho == 0) {
        return 0;
    }

    Item **vetor = (Item **)malloc(sizeof(Item *) * pilha->tamanho);
    if (vetor == NULL) {
        return 0;
    }

    int idx = 0;
    CelulaPilha *atual = pilha->topo;
    while (atual != NULL) {
        vetor[idx++] = atual->item;
        atual = atual->abaixo;
    }

    *itens_saida = vetor;
    return pilha->tamanho;
}
