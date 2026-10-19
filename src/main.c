#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atributo.h"
#include "item.h"
#include "aventureiro.h"
#include "pilha.h"
#include "fila.h"
#include "catalogo.h"
#include "ordenacao.h"
#include "util.h"

typedef struct {
    char nome[32];
    double peso;
} MetricaConsulta;

/*
 * Calcula o vetor denso da build do aventureiro de tamanho total_atributos:
 * soma os modificadores de todos os itens atualmente equipados em sua pilha.
 */
static void calcular_vetor_build(const Aventureiro *av, const Catalogo *cat, int *vetor_build, int total_atributos) {
    memset(vetor_build, 0, sizeof(int) * total_atributos);
    if (av == NULL) {
        return;
    }

    Pilha *build = obter_build_aventureiro(av);
    if (build == NULL) {
        return;
    }

    Item **itens_equipados = NULL;
    int total_equipados = pilha_obter_itens(build, &itens_equipados);
    for (int i = 0; i < total_equipados; i++) {
        EntradaModificador *mods = NULL;
        int qtd_mods = catalogo_obter_modificadores_item(cat, obter_id_item(itens_equipados[i]), &mods);
        for (int j = 0; j < qtd_mods; j++) {
            if (mods[j].id < total_atributos) {
                vetor_build[mods[j].id] += mods[j].valor;
            }
        }
        if (mods != NULL) {
            free(mods);
        }
    }

    if (itens_equipados != NULL) {
        free(itens_equipados);
    }
}

int main(void) {
    Catalogo *catalogo = NULL;
    Fila *fila = criar_fila();

    // Vetores dinâmicos para gerenciar as entidades cadastradas
    Atributo **atributos = NULL;
    int total_atributos = 0;
    int cap_atributos = 0;

    Item **itens = NULL;
    int total_itens = 0;
    int cap_itens = 0;

    Aventureiro **aventureiros = NULL;
    int total_aventureiros = 0;
    int cap_aventureiros = 0;

    char comando[16];

    while (scanf("%15s", comando) == 1) {
        if (strcmp(comando, "E") == 0) {
            char modo;
            if (scanf(" %c", &modo) == 1) {
                if (catalogo == NULL) {
                    catalogo = criar_catalogo((ModoArmazenamento)modo);
                } else {
                    catalogo_alterar_modo(catalogo, (ModoArmazenamento)modo);
                }
                printf("E %c\n", modo);
            }
        } else if (strcmp(comando, "A") == 0) {
            char nome[256];
            if (scanf("%255s", nome) == 1) {
                int id = total_atributos;
                if (total_atributos == cap_atributos) {
                    cap_atributos = (cap_atributos == 0) ? 16 : cap_atributos * 2;
                    atributos = (Atributo **)realloc(atributos, sizeof(Atributo *) * cap_atributos);
                }
                atributos[total_atributos] = criar_atributo(id, nome);
                total_atributos++;
                if (catalogo != NULL) {
                    catalogo_adicionar_atributo(catalogo, id);
                }
                printf("A %d\n", id);
            }
        } else if (strcmp(comando, "I") == 0) {
            char nome[256];
            if (scanf("%255s", nome) == 1) {
                int id = total_itens;
                if (total_itens == cap_itens) {
                    cap_itens = (cap_itens == 0) ? 16 : cap_itens * 2;
                    itens = (Item **)realloc(itens, sizeof(Item *) * cap_itens);
                }
                itens[total_itens] = criar_item(id, nome);
                total_itens++;
                if (catalogo != NULL) {
                    catalogo_adicionar_item(catalogo, id);
                }
                printf("I %d\n", id);
            }
        } else if (strcmp(comando, "M") == 0) {
            int id_item, id_atributo, valor;
            if (scanf("%d %d %d", &id_item, &id_atributo, &valor) == 3) {
                catalogo_definir_modificador(catalogo, id_item, id_atributo, valor);
                printf("M %s %s %d\n",
                       obter_nome_item(itens[id_item]),
                       obter_nome_atributo(atributos[id_atributo]),
                       valor);
            }
        } else if (strcmp(comando, "H") == 0) {
            char nome[256];
            if (scanf("%255s", nome) == 1) {
                int id = total_aventureiros;
                if (total_aventureiros == cap_aventureiros) {
                    cap_aventureiros = (cap_aventureiros == 0) ? 16 : cap_aventureiros * 2;
                    aventureiros = (Aventureiro **)realloc(aventureiros, sizeof(Aventureiro *) * cap_aventureiros);
                }
                aventureiros[total_aventureiros] = criar_aventureiro(id, nome);
                total_aventureiros++;
                printf("H %d\n", id);
            }
        } else if (strcmp(comando, "F") == 0) {
            int id_aventureiro, id_item;
            if (scanf("%d %d", &id_aventureiro, &id_item) == 2) {
                Encomenda *enc = criar_encomenda(id_aventureiro, id_item);
                int pos = enfileirar(fila, enc);
                printf("F %d\n", pos);
            }
        } else if (strcmp(comando, "Z") == 0) {
            int k;
            if (scanf("%d", &k) == 1) {
                Encomenda **forjadas = (Encomenda **)malloc(sizeof(Encomenda *) * k);
                int n_forjadas = 0;

                for (int i = 0; i < k; i++) {
                    Encomenda *enc = desenfileirar(fila);
                    if (enc == NULL) {
                        break;
                    }
                    forjadas[n_forjadas++] = enc;
                    // Equipa o item no aventureiro correspondente
                    aventureiro_equipar_item(aventureiros[enc->id_aventureiro], itens[enc->id_item]);
                }

                printf("Z %d\n", n_forjadas);
                if (n_forjadas > 0) {
                    for (int i = 0; i < n_forjadas; i++) {
                        printf("encomenda_%d %d %d%s",
                               i + 1,
                               forjadas[i]->id_aventureiro,
                               forjadas[i]->id_item,
                               (i == n_forjadas - 1) ? "" : " ");
                        destruir_encomenda(forjadas[i]);
                    }
                    printf("\n");
                }
                free(forjadas);
            }
        } else if (strcmp(comando, "D") == 0) {
            int id_aventureiro;
            if (scanf("%d", &id_aventureiro) == 1) {
                Item *desequipado = aventureiro_desequipar_item(aventureiros[id_aventureiro]);
                if (desequipado == NULL) {
                    printf("D %s VAZIO\n", obter_nome_aventureiro(aventureiros[id_aventureiro]));
                } else {
                    printf("D %s %s\n",
                           obter_nome_aventureiro(aventureiros[id_aventureiro]),
                           obter_nome_item(desequipado));
                }
            }
        } else if (strcmp(comando, "LI") == 0) {
            int id_item;
            if (scanf("%d", &id_item) == 1) {
                EntradaModificador *mods = NULL;
                int qtd = catalogo_obter_modificadores_item(catalogo, id_item, &mods);
                if (qtd == 0) {
                    printf("LI %s VAZIO\n", obter_nome_item(itens[id_item]));
                } else {
                    printf("LI %s", obter_nome_item(itens[id_item]));
                    for (int i = 0; i < qtd; i++) {
                        printf(" atributo_%d %d %d", i + 1, mods[i].id, mods[i].valor);
                    }
                    printf("\n");
                    free(mods);
                }
            }
        } else if (strcmp(comando, "LA") == 0) {
            int id_atributo;
            if (scanf("%d", &id_atributo) == 1) {
                EntradaModificador *mods = NULL;
                int qtd = catalogo_obter_itens_do_atributo(catalogo, id_atributo, &mods);
                if (qtd == 0) {
                    printf("LA %s VAZIO\n", obter_nome_atributo(atributos[id_atributo]));
                } else {
                    printf("LA %s", obter_nome_atributo(atributos[id_atributo]));
                    for (int i = 0; i < qtd; i++) {
                        printf(" item_%d %d %d", i + 1, mods[i].id, mods[i].valor);
                    }
                    printf("\n");
                    free(mods);
                }
            }
        } else if (strcmp(comando, "LH") == 0) {
            int id_aventureiro;
            if (scanf("%d", &id_aventureiro) == 1) {
                int *vetor_build = (int *)malloc(sizeof(int) * (total_atributos > 0 ? total_atributos : 1));
                calcular_vetor_build(aventureiros[id_aventureiro], catalogo, vetor_build, total_atributos);

                int nao_nulos = 0;
                for (int a = 0; a < total_atributos; a++) {
                    if (vetor_build[a] != 0) {
                        nao_nulos++;
                    }
                }

                if (nao_nulos == 0) {
                    printf("LH %s VAZIO\n", obter_nome_aventureiro(aventureiros[id_aventureiro]));
                } else {
                    printf("LH %s", obter_nome_aventureiro(aventureiros[id_aventureiro]));
                    int idx = 1;
                    for (int a = 0; a < total_atributos; a++) {
                        if (vetor_build[a] != 0) {
                            printf(" atributo_%d %d %d", idx++, a, vetor_build[a]);
                        }
                    }
                    printf("\n");
                }
                free(vetor_build);
            }
        } else if (strcmp(comando, "Q") == 0) {
            int id_aventureiro, n, m;
            if (scanf("%d %d %d", &id_aventureiro, &n, &m) == 3) {
                MetricaConsulta *metricas = (MetricaConsulta *)malloc(sizeof(MetricaConsulta) * m);
                for (int j = 0; j < m; j++) {
                    scanf("%31s %lf", metricas[j].nome, &metricas[j].peso);
                }

                // 1. Calcula vetor da build e soma dos quadrados da build
                int *vetor_build = (int *)malloc(sizeof(int) * (total_atributos > 0 ? total_atributos : 1));
                calcular_vetor_build(aventureiros[id_aventureiro], catalogo, vetor_build, total_atributos);

                long long soma_quadrados_build = 0;
                for (int a = 0; a < total_atributos; a++) {
                    soma_quadrados_build += ((long long)vetor_build[a] * (long long)vetor_build[a]);
                }

                // 2. Pontuação acumulada para cada um dos N itens do catálogo
                double *pontuacao_acumulada = (double *)calloc(total_itens > 0 ? total_itens : 1, sizeof(double));

                // 3. Avalia cada métrica individualmente
                for (int j = 0; j < m; j++) {
                    ItemRanqueado *ranking_metrica = (ItemRanqueado *)malloc(sizeof(ItemRanqueado) * total_itens);
                    for (int i = 0; i < total_itens; i++) {
                        ranking_metrica[i].id_item = i;
                        if (strcmp(metricas[j].nome, "PODER") == 0) {
                            ranking_metrica[i].valor = (double)catalogo_calcular_poder(catalogo, i);
                        } else if (strcmp(metricas[j].nome, "EQUILIBRIO") == 0) {
                            ranking_metrica[i].valor = catalogo_calcular_equilibrio(catalogo, i, total_atributos);
                        } else if (strcmp(metricas[j].nome, "SINERGIA") == 0) {
                            ranking_metrica[i].valor = calcular_sinergia(catalogo, i, vetor_build, soma_quadrados_build, total_atributos);
                        } else {
                            ranking_metrica[i].valor = 0.0;
                        }
                    }

                    // Ordena de acordo com o valor da métrica (6 casas decimais e desempate por id_item)
                    ordenar_itens(ranking_metrica, total_itens);

                    // Atribui pontos: posição p (0-indexada) recebe (N - p) * peso
                    for (int p = 0; p < total_itens; p++) {
                        int id_it = ranking_metrica[p].id_item;
                        double pontos = (double)(total_itens - p) * metricas[j].peso;
                        pontuacao_acumulada[id_it] += pontos;
                    }

                    free(ranking_metrica);
                }

                // 4. Descarta itens atualmente equipados pelo aventureiro
                ItemRanqueado *candidatos = (ItemRanqueado *)malloc(sizeof(ItemRanqueado) * (total_itens > 0 ? total_itens : 1));
                int total_candidatos = 0;

                for (int i = 0; i < total_itens; i++) {
                    if (!aventureiro_possui_item(aventureiros[id_aventureiro], i)) {
                        candidatos[total_candidatos].id_item = i;
                        candidatos[total_candidatos].valor = pontuacao_acumulada[i];
                        total_candidatos++;
                    }
                }

                // 5. Ordena os candidatos por pontuação final decrescente (desempate por id_item crescente)
                ordenar_itens(candidatos, total_candidatos);

                // 6. Imprime os até <n> primeiros candidatos
                int limite = (total_candidatos < n) ? total_candidatos : n;
                for (int p = 0; p < limite; p++) {
                    printf("Q %d %d %.2f\n", p + 1, candidatos[p].id_item, candidatos[p].valor);
                }

                free(candidatos);
                free(pontuacao_acumulada);
                free(vetor_build);
                free(metricas);
            }
        }
    }

    // Liberação final de todos os recursos alocados
    if (fila != NULL) {
        destruir_fila(fila);
    }
    if (catalogo != NULL) {
        destruir_catalogo(catalogo);
    }

    for (int i = 0; i < total_atributos; i++) {
        destruir_atributo(atributos[i]);
    }
    if (atributos != NULL) {
        free(atributos);
    }

    for (int i = 0; i < total_itens; i++) {
        destruir_item(itens[i]);
    }
    if (itens != NULL) {
        free(itens);
    }

    for (int i = 0; i < total_aventureiros; i++) {
        destruir_aventureiro(aventureiros[i]);
    }
    if (aventureiros != NULL) {
        free(aventureiros);
    }

    return 0;
}
