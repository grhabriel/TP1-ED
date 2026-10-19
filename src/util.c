#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util.h"

char *duplicar_texto(const char *origem) {
    if (origem == NULL) {
        return NULL;
    }
    size_t tamanho = strlen(origem) + 1;
    char *copia = (char *)malloc(tamanho);
    if (copia != NULL) {
        memcpy(copia, origem, tamanho);
    }
    return copia;
}

double arredondar_6_casas(double valor) {
    return round(valor * 1e6) / 1e6;
}

int comparar_reais_6_casas(double a, double b) {
    double r_a = arredondar_6_casas(a);
    double r_b = arredondar_6_casas(b);
    if (r_a < r_b) {
        return -1;
    }
    if (r_a > r_b) {
        return 1;
    }
    return 0;
}
