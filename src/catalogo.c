#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/catalogo.h"

/* =========================================================================
 * Estruturas Internas - Modo L: Lista Cruzada
 * ========================================================================= */
typedef struct CelulaMatriz {
    int id_item;                  // Linha
    int id_atributo;              // Coluna
    int valor;                    // Modificador != 0
    struct CelulaMatriz *direita; // Próxima célula na mesma linha (mesmo item)
    struct CelulaMatriz *abaixo;  // Próxima célula na mesma coluna (mesmo atributo)
} CelulaMatriz;

typedef struct {
    CelulaMatriz **cabecas_linhas;   // Vetor de ponteiros para o início de cada linha
    int cap_linhas;                  // Capacidade alocada do vetor de linhas
    CelulaMatriz **cabecas_colunas;  // Vetor de ponteiros para o início de cada coluna
    int cap_colunas;                 // Capacidade alocada do vetor de colunas
} ListaCruzada;

/* =========================================================================
 * Estruturas Internas - Modo T: Vetor de Tripletas
 * ========================================================================= */
typedef struct {
    int id_item;
    int id_atributo;
    int valor;
} Tripleta;

typedef struct {
    Tripleta *dados;
    int quantidade;
    int capacidade;
} VetorTripletas;

/* =========================================================================
 * Estrutura do TAD Catalogo
 * ========================================================================= */
struct Catalogo {
    ModoArmazenamento modo_ativo;
    ListaCruzada lista;
    VetorTripletas tripletas;
    int total_itens;
    int total_atributos;
};

/* =========================================================================
 * Funções Auxiliares: Redimensionamento e Busca
 * ========================================================================= */

static void garantir_capacidade_linhas(ListaCruzada *lista, int id_item) {
    if (id_item < lista->cap_linhas) {
        return;
    }
    int nova_cap = (lista->cap_linhas == 0) ? 16 : lista->cap_linhas * 2;
    while (nova_cap <= id_item) {
        nova_cap *= 2;
    }
    CelulaMatriz **novas = (CelulaMatriz **)realloc(lista->cabecas_linhas, sizeof(CelulaMatriz *) * nova_cap);
    if (novas == NULL) {
        return;
    }
    for (int i = lista->cap_linhas; i < nova_cap; i++) {
        novas[i] = NULL;
    }
    lista->cabecas_linhas = novas;
    lista->cap_linhas = nova_cap;
}

static void garantir_capacidade_colunas(ListaCruzada *lista, int id_atributo) {
    if (id_atributo < lista->cap_colunas) {
        return;
    }
    int nova_cap = (lista->cap_colunas == 0) ? 16 : lista->cap_colunas * 2;
    while (nova_cap <= id_atributo) {
        nova_cap *= 2;
    }
    CelulaMatriz **novas = (CelulaMatriz **)realloc(lista->cabecas_colunas, sizeof(CelulaMatriz *) * nova_cap);
    if (novas == NULL) {
        return;
    }
    for (int j = lista->cap_colunas; j < nova_cap; j++) {
        novas[j] = NULL;
    }
    lista->cabecas_colunas = novas;
    lista->cap_colunas = nova_cap;
}

// Busca binária no vetor de tripletas mantido ordenado por (id_item, id_atributo).
// Retorna o índice onde foi encontrado, ou onde deveria ser inserido.
// O ponteiro *encontrado recebe 1 se o elemento exato existe e 0 caso contrário.
static int busca_binaria_tripleta(const VetorTripletas *vet, int id_item, int id_atributo, int *encontrado) {
    int inicio = 0;
    int fim = vet->quantidade - 1;
    *encontrado = 0;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        const Tripleta *t = &vet->dados[meio];

        if (t->id_item == id_item && t->id_atributo == id_atributo) {
            *encontrado = 1;
            return meio;
        }

        if (t->id_item < id_item || (t->id_item == id_item && t->id_atributo < id_atributo)) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return inicio; // Posição de inserção para manter ordenado
}

/* =========================================================================
 * Operações na Lista Cruzada (Modo L)
 * ========================================================================= */

static int lista_obter(const ListaCruzada *lista, int id_item, int id_atributo) {
    if (id_item >= lista->cap_linhas || lista->cabecas_linhas == NULL) {
        return 0;
    }
    CelulaMatriz *atual = lista->cabecas_linhas[id_item];
    while (atual != NULL && atual->id_atributo <= id_atributo) {
        if (atual->id_atributo == id_atributo) {
            return atual->valor;
        }
        atual = atual->direita;
    }
    return 0;
}

static int lista_definir(ListaCruzada *lista, int id_item, int id_atributo, int valor) {
    garantir_capacidade_linhas(lista, id_item);
    garantir_capacidade_colunas(lista, id_atributo);

    // 1. Procurar na linha
    CelulaMatriz *ant_linha = NULL;
    CelulaMatriz *atual_linha = lista->cabecas_linhas[id_item];
    while (atual_linha != NULL && atual_linha->id_atributo < id_atributo) {
        ant_linha = atual_linha;
        atual_linha = atual_linha->direita;
    }

    int existe = (atual_linha != NULL && atual_linha->id_atributo == id_atributo);

    // Caso A: Remoção (valor == 0)
    if (valor == 0) {
        if (!existe) {
            return 1; // Já não existia, nada a fazer
        }

        // Desconectar da linha
        if (ant_linha == NULL) {
            lista->cabecas_linhas[id_item] = atual_linha->direita;
        } else {
            ant_linha->direita = atual_linha->direita;
        }

        // Desconectar da coluna
        CelulaMatriz *ant_col = NULL;
        CelulaMatriz *atual_col = lista->cabecas_colunas[id_atributo];
        while (atual_col != NULL && atual_col != atual_linha) {
            ant_col = atual_col;
            atual_col = atual_col->abaixo;
        }
        if (atual_col != NULL) {
            if (ant_col == NULL) {
                lista->cabecas_colunas[id_atributo] = atual_linha->abaixo;
            } else {
                ant_col->abaixo = atual_linha->abaixo;
            }
        }

        free(atual_linha);
        return 1;
    }

    // Caso B: Atualização de valor existente
    if (existe) {
        atual_linha->valor = valor;
        return 1;
    }

    // Caso C: Inserção de nova célula (valor != 0)
    CelulaMatriz *nova = (CelulaMatriz *)malloc(sizeof(CelulaMatriz));
    if (nova == NULL) {
        return 0;
    }
    nova->id_item = id_item;
    nova->id_atributo = id_atributo;
    nova->valor = valor;

    // Inserir na linha
    if (ant_linha == NULL) {
        nova->direita = lista->cabecas_linhas[id_item];
        lista->cabecas_linhas[id_item] = nova;
    } else {
        nova->direita = ant_linha->direita;
        ant_linha->direita = nova;
    }

    // Inserir na coluna ordenada por id_item crescente
    CelulaMatriz *ant_col = NULL;
    CelulaMatriz *atual_col = lista->cabecas_colunas[id_atributo];
    while (atual_col != NULL && atual_col->id_item < id_item) {
        ant_col = atual_col;
        atual_col = atual_col->abaixo;
    }

    if (ant_col == NULL) {
        nova->abaixo = lista->cabecas_colunas[id_atributo];
        lista->cabecas_colunas[id_atributo] = nova;
    } else {
        nova->abaixo = ant_col->abaixo;
        ant_col->abaixo = nova;
    }

    return 1;
}

static void lista_liberar(ListaCruzada *lista) {
    if (lista->cabecas_linhas != NULL) {
        for (int i = 0; i < lista->cap_linhas; i++) {
            CelulaMatriz *atual = lista->cabecas_linhas[i];
            while (atual != NULL) {
                CelulaMatriz *prox = atual->direita;
                free(atual);
                atual = prox;
            }
        }
        free(lista->cabecas_linhas);
        lista->cabecas_linhas = NULL;
        lista->cap_linhas = 0;
    }
    if (lista->cabecas_colunas != NULL) {
        free(lista->cabecas_colunas);
        lista->cabecas_colunas = NULL;
        lista->cap_colunas = 0;
    }
}

/* =========================================================================
 * Operações no Vetor de Tripletas (Modo T)
 * ========================================================================= */

static int tripletas_obter(const VetorTripletas *vet, int id_item, int id_atributo) {
    if (vet->quantidade == 0) {
        return 0;
    }
    int encontrado = 0;
    int pos = busca_binaria_tripleta(vet, id_item, id_atributo, &encontrado);
    if (encontrado) {
        return vet->dados[pos].valor;
    }
    return 0;
}

static int tripletas_definir(VetorTripletas *vet, int id_item, int id_atributo, int valor) {
    int encontrado = 0;
    int pos = busca_binaria_tripleta(vet, id_item, id_atributo, &encontrado);

    // Caso A: Remoção (valor == 0)
    if (valor == 0) {
        if (!encontrado) {
            return 1;
        }
        // Desloca para a esquerda para remover
        int restantes = vet->quantidade - pos - 1;
        if (restantes > 0) {
            memmove(&vet->dados[pos], &vet->dados[pos + 1], sizeof(Tripleta) * restantes);
        }
        vet->quantidade--;
        return 1;
    }

    // Caso B: Atualização
    if (encontrado) {
        vet->dados[pos].valor = valor;
        return 1;
    }

    // Caso C: Inserção
    if (vet->quantidade == vet->capacidade) {
        int nova_cap = (vet->capacidade == 0) ? 16 : vet->capacidade * 2;
        Tripleta *novos = (Tripleta *)realloc(vet->dados, sizeof(Tripleta) * nova_cap);
        if (novos == NULL) {
            return 0;
        }
        vet->dados = novos;
        vet->capacidade = nova_cap;
    }

    int restantes = vet->quantidade - pos;
    if (restantes > 0) {
        memmove(&vet->dados[pos + 1], &vet->dados[pos], sizeof(Tripleta) * restantes);
    }
    vet->dados[pos].id_item = id_item;
    vet->dados[pos].id_atributo = id_atributo;
    vet->dados[pos].valor = valor;
    vet->quantidade++;

    return 1;
}

static void tripletas_liberar(VetorTripletas *vet) {
    if (vet->dados != NULL) {
        free(vet->dados);
        vet->dados = NULL;
    }
    vet->quantidade = 0;
    vet->capacidade = 0;
}

/* =========================================================================
 * Criação, Destruição e Migração do Catálogo
 * ========================================================================= */

Catalogo *criar_catalogo(ModoArmazenamento modo_inicial) {
    Catalogo *cat = (Catalogo *)malloc(sizeof(Catalogo));
    if (cat == NULL) {
        return NULL;
    }
    cat->modo_ativo = modo_inicial;
    cat->lista.cabecas_linhas = NULL;
    cat->lista.cap_linhas = 0;
    cat->lista.cabecas_colunas = NULL;
    cat->lista.cap_colunas = 0;

    cat->tripletas.dados = NULL;
    cat->tripletas.quantidade = 0;
    cat->tripletas.capacidade = 0;

    cat->total_itens = 0;
    cat->total_atributos = 0;
    return cat;
}

void destruir_catalogo(Catalogo *cat) {
    if (cat == NULL) {
        return;
    }
    lista_liberar(&cat->lista);
    tripletas_liberar(&cat->tripletas);
    free(cat);
}

ModoArmazenamento catalogo_obter_modo(const Catalogo *catalogo) {
    return catalogo ? catalogo->modo_ativo : MODO_LISTA_CRUZADA;
}

int catalogo_alterar_modo(Catalogo *cat, ModoArmazenamento novo_modo) {
    if (cat == NULL) {
        return 0;
    }
    if (cat->modo_ativo == novo_modo) {
        return 1; // Já está no modo solicitado
    }

    if (novo_modo == MODO_TRIPLETAS) {
        // Migrar de Lista Cruzada (L) para Tripletas (T)
        // Percorremos linha a linha da lista cruzada (já sairá ordenado!)
        tripletas_liberar(&cat->tripletas);

        for (int i = 0; i < cat->lista.cap_linhas; i++) {
            CelulaMatriz *atual = cat->lista.cabecas_linhas[i];
            while (atual != NULL) {
                // Como percorremos i crescente e a linha tem id_atributo crescente,
                // a inserção no fim mantém o vetor ordenado em O(1) amortizado!
                if (cat->tripletas.quantidade == cat->tripletas.capacidade) {
                    int nova_cap = (cat->tripletas.capacidade == 0) ? 16 : cat->tripletas.capacidade * 2;
                    Tripleta *novos = (Tripleta *)realloc(cat->tripletas.dados, sizeof(Tripleta) * nova_cap);
                    if (novos == NULL) {
                        return 0;
                    }
                    cat->tripletas.dados = novos;
                    cat->tripletas.capacidade = nova_cap;
                }
                int idx = cat->tripletas.quantidade++;
                cat->tripletas.dados[idx].id_item = atual->id_item;
                cat->tripletas.dados[idx].id_atributo = atual->id_atributo;
                cat->tripletas.dados[idx].valor = atual->valor;

                atual = atual->direita;
            }
        }
        // Libera a memória da lista cruzada
        lista_liberar(&cat->lista);
        cat->modo_ativo = MODO_TRIPLETAS;
    } else {
        // Migrar de Tripletas (T) para Lista Cruzada (L)
        lista_liberar(&cat->lista);
        for (int k = 0; k < cat->tripletas.quantidade; k++) {
            const Tripleta *t = &cat->tripletas.dados[k];
            lista_definir(&cat->lista, t->id_item, t->id_atributo, t->valor);
        }
        // Libera a memória do vetor de tripletas
        tripletas_liberar(&cat->tripletas);
        cat->modo_ativo = MODO_LISTA_CRUZADA;
    }

    return 1;
}

void catalogo_adicionar_item(Catalogo *cat, int id_item) {
    if (cat == NULL) {
        return;
    }
    if (id_item >= cat->total_itens) {
        cat->total_itens = id_item + 1;
    }
    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        garantir_capacidade_linhas(&cat->lista, id_item);
    }
}

void catalogo_adicionar_atributo(Catalogo *cat, int id_atributo) {
    if (cat == NULL) {
        return;
    }
    if (id_atributo >= cat->total_atributos) {
        cat->total_atributos = id_atributo + 1;
    }
    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        garantir_capacidade_colunas(&cat->lista, id_atributo);
    }
}

int catalogo_definir_modificador(Catalogo *cat, int id_item, int id_atributo, int valor) {
    if (cat == NULL) {
        return 0;
    }
    catalogo_adicionar_item(cat, id_item);
    catalogo_adicionar_atributo(cat, id_atributo);

    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        return lista_definir(&cat->lista, id_item, id_atributo, valor);
    } else {
        return tripletas_definir(&cat->tripletas, id_item, id_atributo, valor);
    }
}

int catalogo_obter_modificador(const Catalogo *cat, int id_item, int id_atributo) {
    if (cat == NULL) {
        return 0;
    }
    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        return lista_obter(&cat->lista, id_item, id_atributo);
    } else {
        return tripletas_obter(&cat->tripletas, id_item, id_atributo);
    }
}

int catalogo_remover_modificador(Catalogo *cat, int id_item, int id_atributo) {
    return catalogo_definir_modificador(cat, id_item, id_atributo, 0);
}

/* =========================================================================
 * Percursos de Linha (LI) e de Coluna (LA)
 * ========================================================================= */

int catalogo_obter_modificadores_item(const Catalogo *cat, int id_item, EntradaModificador **saida) {
    if (cat == NULL || saida == NULL) {
        return 0;
    }
    *saida = NULL;

    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        if (id_item >= cat->lista.cap_linhas || cat->lista.cabecas_linhas == NULL) {
            return 0;
        }
        int cont = 0;
        CelulaMatriz *atual = cat->lista.cabecas_linhas[id_item];
        while (atual != NULL) {
            cont++;
            atual = atual->direita;
        }
        if (cont == 0) {
            return 0;
        }

        EntradaModificador *arr = (EntradaModificador *)malloc(sizeof(EntradaModificador) * cont);
        if (arr == NULL) {
            return 0;
        }

        int idx = 0;
        atual = cat->lista.cabecas_linhas[id_item];
        while (atual != NULL) {
            arr[idx].id = atual->id_atributo;
            arr[idx].valor = atual->valor;
            idx++;
            atual = atual->direita;
        }
        *saida = arr;
        return cont;
    } else {
        // Modo Tripletas
        if (cat->tripletas.quantidade == 0) {
            return 0;
        }
        int encontrado = 0;
        int pos = busca_binaria_tripleta(&cat->tripletas, id_item, 0, &encontrado);
        // pos aponta para onde começaria id_item
        int cont = 0;
        for (int i = pos; i < cat->tripletas.quantidade && cat->tripletas.dados[i].id_item == id_item; i++) {
            cont++;
        }
        if (cont == 0) {
            return 0;
        }

        EntradaModificador *arr = (EntradaModificador *)malloc(sizeof(EntradaModificador) * cont);
        if (arr == NULL) {
            return 0;
        }

        for (int i = 0; i < cont; i++) {
            arr[i].id = cat->tripletas.dados[pos + i].id_atributo;
            arr[i].valor = cat->tripletas.dados[pos + i].valor;
        }
        *saida = arr;
        return cont;
    }
}

int catalogo_obter_itens_do_atributo(const Catalogo *cat, int id_atributo, EntradaModificador **saida) {
    if (cat == NULL || saida == NULL) {
        return 0;
    }
    *saida = NULL;

    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        if (id_atributo >= cat->lista.cap_colunas || cat->lista.cabecas_colunas == NULL) {
            return 0;
        }
        int cont = 0;
        CelulaMatriz *atual = cat->lista.cabecas_colunas[id_atributo];
        while (atual != NULL) {
            cont++;
            atual = atual->abaixo;
        }
        if (cont == 0) {
            return 0;
        }

        EntradaModificador *arr = (EntradaModificador *)malloc(sizeof(EntradaModificador) * cont);
        if (arr == NULL) {
            return 0;
        }

        int idx = 0;
        atual = cat->lista.cabecas_colunas[id_atributo];
        while (atual != NULL) {
            arr[idx].id = atual->id_item;
            arr[idx].valor = atual->valor;
            idx++;
            atual = atual->abaixo;
        }
        *saida = arr;
        return cont;
    } else {
        // Modo Tripletas: varre todas as tripletas filtrando por id_atributo
        // Como o vetor está ordenado por (id_item, id_atributo), os itens já saem ordenados por id_item!
        int cont = 0;
        for (int i = 0; i < cat->tripletas.quantidade; i++) {
            if (cat->tripletas.dados[i].id_atributo == id_atributo) {
                cont++;
            }
        }
        if (cont == 0) {
            return 0;
        }

        EntradaModificador *arr = (EntradaModificador *)malloc(sizeof(EntradaModificador) * cont);
        if (arr == NULL) {
            return 0;
        }

        int idx = 0;
        for (int i = 0; i < cat->tripletas.quantidade; i++) {
            if (cat->tripletas.dados[i].id_atributo == id_atributo) {
                arr[idx].id = cat->tripletas.dados[i].id_item;
                arr[idx].valor = cat->tripletas.dados[i].valor;
                idx++;
            }
        }
        *saida = arr;
        return cont;
    }
}

/* =========================================================================
 * Cálculos Matemáticos e Métricas (PODER, EQUILIBRIO, SINERGIA)
 * ========================================================================= */

int catalogo_calcular_poder(const Catalogo *cat, int id_item) {
    if (cat == NULL) {
        return 0;
    }
    int soma = 0;

    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        if (id_item < cat->lista.cap_linhas && cat->lista.cabecas_linhas != NULL) {
            CelulaMatriz *atual = cat->lista.cabecas_linhas[id_item];
            while (atual != NULL) {
                soma += atual->valor;
                atual = atual->direita;
            }
        }
    } else {
        int encontrado = 0;
        int pos = busca_binaria_tripleta(&cat->tripletas, id_item, 0, &encontrado);
        for (int i = pos; i < cat->tripletas.quantidade && cat->tripletas.dados[i].id_item == id_item; i++) {
            soma += cat->tripletas.dados[i].valor;
        }
    }
    return soma;
}

double catalogo_calcular_equilibrio(const Catalogo *cat, int id_item, int total_atributos) {
    if (total_atributos == 0) {
        return 1.0;
    }
    int poder = catalogo_calcular_poder(cat, id_item);
    double mu_i = (double)poder / (double)total_atributos;

    double soma_desvios = 0.0;
    for (int a = 0; a < total_atributos; a++) {
        int v_ia = catalogo_obter_modificador(cat, id_item, a);
        double diff = (double)v_ia - mu_i;
        soma_desvios += (diff * diff);
    }

    double sigma_i = sqrt(soma_desvios / (double)total_atributos);

    return 1.0 / (1.0 + sigma_i);
}

long long catalogo_calcular_soma_quadrados(const Catalogo *cat, int id_item) {
    if (cat == NULL) {
        return 0;
    }
    long long soma = 0;

    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        if (id_item < cat->lista.cap_linhas && cat->lista.cabecas_linhas != NULL) {
            CelulaMatriz *atual = cat->lista.cabecas_linhas[id_item];
            while (atual != NULL) {
                long long val = atual->valor;
                soma += (val * val);
                atual = atual->direita;
            }
        }
    } else {
        int encontrado = 0;
        int pos = busca_binaria_tripleta(&cat->tripletas, id_item, 0, &encontrado);
        for (int i = pos; i < cat->tripletas.quantidade && cat->tripletas.dados[i].id_item == id_item; i++) {
            long long val = cat->tripletas.dados[i].valor;
            soma += (val * val);
        }
    }
    return soma;
}

long long catalogo_produto_interno_com_vetor(const Catalogo *cat, int id_item, const int *vetor_build, int total_atributos) {
    if (cat == NULL || vetor_build == NULL || total_atributos == 0) {
        return 0;
    }
    long long produto = 0;

    if (cat->modo_ativo == MODO_LISTA_CRUZADA) {
        if (id_item < cat->lista.cap_linhas && cat->lista.cabecas_linhas != NULL) {
            CelulaMatriz *atual = cat->lista.cabecas_linhas[id_item];
            while (atual != NULL) {
                if (atual->id_atributo < total_atributos) {
                    produto += (long long)atual->valor * (long long)vetor_build[atual->id_atributo];
                }
                atual = atual->direita;
            }
        }
    } else {
        int encontrado = 0;
        int pos = busca_binaria_tripleta(&cat->tripletas, id_item, 0, &encontrado);
        for (int i = pos; i < cat->tripletas.quantidade && cat->tripletas.dados[i].id_item == id_item; i++) {
            int attr = cat->tripletas.dados[i].id_atributo;
            if (attr < total_atributos) {
                produto += (long long)cat->tripletas.dados[i].valor * (long long)vetor_build[attr];
            }
        }
    }
    return produto;
}
