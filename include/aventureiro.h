#ifndef AVENTUREIRO_H
#define AVENTUREIRO_H

#include "pilha.h"

typedef struct {
    int id;
    char *nome;
    Pilha *build;
} Aventureiro;

#endif // AVENTUREIRO_H
