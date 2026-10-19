#ifndef AVENTUREIRO_H
#define AVENTUREIRO_H

#include "pilha.h"
#include "item.h"

/*
 * Estrutura que representa um aventureiro e sua build de itens equipados.
 */
typedef struct {
    int id;
    char *nome;
    Pilha *build;
} Aventureiro;

/*
 * Cria e aloca dinamicamente um novo aventureiro com seu ID e nome,
 * inicializando também sua pilha de build vazia.
 * Retorna o ponteiro para o Aventureiro criado ou NULL em caso de falha.
 */
Aventureiro *criar_aventureiro(int id, const char *nome);

/*
 * Libera a memória associada ao aventureiro, incluindo seu nome e sua pilha de build.
 */
void destruir_aventureiro(Aventureiro *aventureiro);

/*
 * Equipa um item na build do aventureiro (empilha na sua pilha de build).
 * Retorna 1 em caso de sucesso e 0 em caso de falha.
 */
int aventureiro_equipar_item(Aventureiro *aventureiro, Item *item);

/*
 * Desequipa a peça mais recente da build do aventureiro (desempilha).
 * Retorna o ponteiro para o Item desequipado ou NULL se a build estiver vazia.
 */
Item *aventureiro_desequipar_item(Aventureiro *aventureiro);

/*
 * Verifica se o aventureiro já possui um determinado item equipado em sua build.
 * Retorna 1 se o item estiver equipado e 0 caso contrário.
 */
int aventureiro_possui_item(const Aventureiro *aventureiro, int id_item);

/*
 * Retorna o identificador do aventureiro.
 */
int obter_id_aventureiro(const Aventureiro *aventureiro);

/*
 * Retorna o ponteiro para a string com o nome do aventureiro.
 */
const char *obter_nome_aventureiro(const Aventureiro *aventureiro);

/*
 * Retorna o ponteiro para a pilha da build do aventureiro.
 */
Pilha *obter_build_aventureiro(const Aventureiro *aventureiro);

#endif // AVENTUREIRO_H
