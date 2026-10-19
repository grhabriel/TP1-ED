#include <stdlib.h>
#include "aventureiro.h"
#include "util.h"

Aventureiro *criar_aventureiro(int id, const char *nome) {
    if (nome == NULL) {
        return NULL;
    }

    Aventureiro *aventureiro = (Aventureiro *)malloc(sizeof(Aventureiro));
    if (aventureiro == NULL) {
        return NULL;
    }

    aventureiro->id = id;
    aventureiro->nome = duplicar_texto(nome);
    if (aventureiro->nome == NULL) {
        free(aventureiro);
        return NULL;
    }

    aventureiro->build = criar_pilha();
    if (aventureiro->build == NULL) {
        free(aventureiro->nome);
        free(aventureiro);
        return NULL;
    }

    return aventureiro;
}

void destruir_aventureiro(Aventureiro *aventureiro) {
    if (aventureiro == NULL) {
        return;
    }
    if (aventureiro->nome != NULL) {
        free(aventureiro->nome);
    }
    if (aventureiro->build != NULL) {
        destruir_pilha(aventureiro->build);
    }
    free(aventureiro);
}

int aventureiro_equipar_item(Aventureiro *aventureiro, Item *item) {
    if (aventureiro == NULL || item == NULL) {
        return 0;
    }
    return empilhar(aventureiro->build, item);
}

Item *aventureiro_desequipar_item(Aventureiro *aventureiro) {
    if (aventureiro == NULL) {
        return NULL;
    }
    return desempilhar(aventureiro->build);
}

int aventureiro_possui_item(const Aventureiro *aventureiro, int id_item) {
    if (aventureiro == NULL) {
        return 0;
    }
    return pilha_contem_item(aventureiro->build, id_item);
}

int obter_id_aventureiro(const Aventureiro *aventureiro) {
    if (aventureiro == NULL) {
        return -1;
    }
    return aventureiro->id;
}

const char *obter_nome_aventureiro(const Aventureiro *aventureiro) {
    if (aventureiro == NULL) {
        return NULL;
    }
    return aventureiro->nome;
}

Pilha *obter_build_aventureiro(const Aventureiro *aventureiro) {
    if (aventureiro == NULL) {
        return NULL;
    }
    return aventureiro->build;
}
