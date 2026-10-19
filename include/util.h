#ifndef UTIL_H
#define UTIL_H

/*
 * Cria uma cópia da string de origem alocada dinamicamente no heap.
 * Retorna o ponteiro para a nova string ou NULL em caso de erro ou se origem for NULL.
 */
char *duplicar_texto(const char *origem);

/*
 * Arredonda um valor double para 6 casas decimais, conforme exigido pelo TP.
 */
double arredondar_6_casas(double valor);

/*
 * Compara dois valores double considerando arredondamento para 6 casas decimais.
 * Retorna:
 *  -1 se a < b
 *   0 se a == b
 *   1 se a > b
 */
int comparar_reais_6_casas(double a, double b);

#endif // UTIL_H
