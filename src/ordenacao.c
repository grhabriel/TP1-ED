#include <stdlib.h>
#include <math.h>
#include "ordenacao.h"
#include "util.h"

int comparar_itens_ranqueados(const ItemRanqueado *a, const ItemRanqueado *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    // 1. Critério primário: maior valor primeiro (decrescente com 6 casas decimais)
    int cmp = comparar_reais_6_casas(a->valor, b->valor);
    if (cmp != 0) {
        return -cmp; // Inverte o sinal porque queremos ordem decrescente de valor
    }

    // 2. Critério de desempate: menor id_item primeiro (crescente de ID)
    if (a->id_item < b->id_item) {
        return -1;
    }
    if (a->id_item > b->id_item) {
        return 1;
    }
    return 0;
}

static void trocar(ItemRanqueado *a, ItemRanqueado *b) {
    ItemRanqueado temp = *a;
    *a = *b;
    *b = temp;
}

static int particionar(ItemRanqueado *vetor, int inicio, int fim) {
    ItemRanqueado pivo = vetor[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        // Se vetor[j] deve vir antes ou é equivalente ao pivô
        if (comparar_itens_ranqueados(&vetor[j], &pivo) <= 0) {
            i++;
            trocar(&vetor[i], &vetor[j]);
        }
    }
    trocar(&vetor[i + 1], &vetor[fim]);
    return i + 1;
}

void ordenar_quicksort(ItemRanqueado *vetor, int inicio, int fim) {
    if (inicio < fim) {
        int indice_pivo = particionar(vetor, inicio, fim);
        ordenar_quicksort(vetor, inicio, indice_pivo - 1);
        ordenar_quicksort(vetor, indice_pivo + 1, fim);
    }
}

void ordenar_itens(ItemRanqueado *vetor, int tamanho) {
    if (vetor == NULL || tamanho <= 1) {
        return;
    }
    ordenar_quicksort(vetor, 0, tamanho - 1);
}

double calcular_sinergia(const Catalogo *cat, int id_item, const int *vetor_build, long long soma_quadrados_build, int total_atributos) {
    if (cat == NULL || vetor_build == NULL || total_atributos == 0 || soma_quadrados_build == 0) {
        return 0.0;
    }

    long long soma_quadrados_item = catalogo_calcular_soma_quadrados(cat, id_item);
    if (soma_quadrados_item == 0) {
        return 0.0; // Item sem nenhum modificador
    }

    long long produto_interno = catalogo_produto_interno_com_vetor(cat, id_item, vetor_build, total_atributos);

    double denominador = sqrt((double)soma_quadrados_item) * sqrt((double)soma_quadrados_build);
    if (denominador == 0.0) {
        return 0.0;
    }

    return (double)produto_interno / denominador;
}
