#include <stdlib.h>
#include "atributo.h"
#include "util.h"

Atributo *criar_atributo(int id, const char *nome) {
    if (nome == NULL) {
        return NULL;
    }

    Atributo *atributo = (Atributo *)malloc(sizeof(Atributo));
    if (atributo == NULL) {
        return NULL;
    }

    atributo->id = id;
    atributo->nome = duplicar_texto(nome);
    if (atributo->nome == NULL) {
        free(atributo);
        return NULL;
    }

    return atributo;
}

void destruir_atributo(Atributo *atributo) {
    if (atributo == NULL) {
        return;
    }
    if (atributo->nome != NULL) {
        free(atributo->nome);
    }
    free(atributo);
}

int obter_id_atributo(const Atributo *atributo) {
    if (atributo == NULL) {
        return -1;
    }
    return atributo->id;
}

const char *obter_nome_atributo(const Atributo *atributo) {
    if (atributo == NULL) {
        return NULL;
    }
    return atributo->nome;
}
