#include <stdlib.h>
#include "item.h"
#include "util.h"

Item *criar_item(int id, const char *nome) {
    if (nome == NULL) {
        return NULL;
    }

    Item *item = (Item *)malloc(sizeof(Item));
    if (item == NULL) {
        return NULL;
    }

    item->id = id;
    item->nome = duplicar_texto(nome);
    if (item->nome == NULL) {
        free(item);
        return NULL;
    }

    return item;
}

void destruir_item(Item *item) {
    if (item == NULL) {
        return;
    }
    if (item->nome != NULL) {
        free(item->nome);
    }
    free(item);
}

int obter_id_item(const Item *item) {
    if (item == NULL) {
        return -1;
    }
    return item->id;
}

const char *obter_nome_item(const Item *item) {
    if (item == NULL) {
        return NULL;
    }
    return item->nome;
}
