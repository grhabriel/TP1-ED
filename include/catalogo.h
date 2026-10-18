#ifndef CATALOGO_H
#define CATALOGO_H

typedef enum {
    MODO_LISTA_CRUZADA = 'L',
    MODO_TRIPLETAS = 'T'
} ModoArmazenamento;

/*
 * TAD Catalogo: Matriz esparsa de modificadores (itens x atributos).
 * Suporta alternância dinâmica entre Modo L (Lista Cruzada) e Modo T (Vetor de Tripletas).
 */
typedef struct Catalogo Catalogo;

#endif // CATALOGO_H
