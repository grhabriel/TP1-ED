#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "catalogo.h"

/*
 * Estrutura para armazenar o ID de um item e sua pontuação ou valor de métrica
 * durante o processo de recomendação.
 */
typedef struct {
    int id_item;
    double valor;
} ItemRanqueado;

/*
 * Compara dois itens ranqueados de acordo com as regras do enunciado:
 *  1. Maior valor vem primeiro (ordem decrescente com precisão de 6 casas decimais).
 *  2. Desempate: menor id_item vem primeiro (ordem crescente de ID).
 * Retorna:
 *  -1 se 'a' deve vir antes de 'b'
 *   0 se 'a' e 'b' são considerados iguais
 *   1 se 'b' deve vir antes de 'a'
 */
int comparar_itens_ranqueados(const ItemRanqueado *a, const ItemRanqueado *b);

/*
 * Ordena um vetor de ItemRanqueado utilizando o algoritmo Quicksort.
 */
void ordenar_quicksort(ItemRanqueado *vetor, int inicio, int fim);

/*
 * Função utilitária que ordena todo o vetor de tamanho n.
 */
void ordenar_itens(ItemRanqueado *vetor, int tamanho);

/*
 * Calcula a SINERGIA entre um item e a build atual do aventureiro:
 *   SINERGIA(i, h) = sum(v_i(a) * b_h(a)) / ( sqrt(sum(v_i(a)^2)) * sqrt(sum(b_h(a)^2)) )
 * Se o denominador for zero (build vazia ou item sem modificadores), retorna 0.0.
 */
double calcular_sinergia(const Catalogo *cat, int id_item, const int *vetor_build, long long soma_quadrados_build, int total_atributos);

#endif // ORDENACAO_H
