#ifndef CATALOGO_H
#define CATALOGO_H

/*
 * Modos de armazenamento obrigatórios para a matriz esparsa.
 */
typedef enum {
    MODO_LISTA_CRUZADA = 'L',
    MODO_TRIPLETAS = 'T'
} ModoArmazenamento;

/*
 * Estrutura auxiliar para retornar pares (id, valor) nas consultas de percurso
 * de uma linha (LI) ou de uma coluna (LA).
 */
typedef struct {
    int id;
    int valor;
} EntradaModificador;

/*
 * Declaração opaca do TAD Catalogo.
 */
typedef struct Catalogo Catalogo;

/*
 * Cria e inicializa o catálogo com o modo de armazenamento especificado.
 */
Catalogo *criar_catalogo(ModoArmazenamento modo_inicial);

/*
 * Libera toda a memória associada ao catálogo e sua representação ativa.
 */
void destruir_catalogo(Catalogo *catalogo);

/*
 * Retorna o modo de armazenamento atualmente ativo ('L' ou 'T').
 */
ModoArmazenamento catalogo_obter_modo(const Catalogo *catalogo);

/*
 * Migra a matriz para a representação indicada preservando todas as células.
 * Retorna 1 em caso de sucesso e 0 em caso de erro.
 */
int catalogo_alterar_modo(Catalogo *catalogo, ModoArmazenamento novo_modo);

/*
 * Notifica o catálogo da adição de um novo item ou atributo (para expansão dinâmica
 * das cabeças de linha ou coluna, se necessário).
 */
void catalogo_adicionar_item(Catalogo *catalogo, int id_item);
void catalogo_adicionar_atributo(Catalogo *catalogo, int id_atributo);

/*
 * Define o modificador do item no atributo.
 * Se valor == 0, remove a célula correspondente.
 * Retorna 1 em caso de sucesso e 0 em caso de falha.
 */
int catalogo_definir_modificador(Catalogo *catalogo, int id_item, int id_atributo, int valor);

/*
 * Consulta o valor do modificador para o par (id_item, id_atributo).
 * Retorna 0 se a célula não existir.
 */
int catalogo_obter_modificador(const Catalogo *catalogo, int id_item, int id_atributo);

/*
 * Remove explicitamente uma célula (equivalente a definir valor = 0).
 */
int catalogo_remover_modificador(Catalogo *catalogo, int id_item, int id_atributo);

/*
 * Percurso de Linha (LI):
 * Retorna a quantidade de modificadores não-nulos do item e aloca em *saida um array
 * com as entradas (id_atributo, valor) ordenadas por id_atributo crescente.
 * O chamador deve liberar *saida com free() se a quantidade for > 0.
 */
int catalogo_obter_modificadores_item(const Catalogo *catalogo, int id_item, EntradaModificador **saida);

/*
 * Percurso de Coluna (LA):
 * Retorna a quantidade de itens que modificam o atributo e aloca em *saida um array
 * com as entradas (id_item, valor) ordenadas por id_item crescente.
 * O chamador deve liberar *saida com free() se a quantidade for > 0.
 */
int catalogo_obter_itens_do_atributo(const Catalogo *catalogo, int id_atributo, EntradaModificador **saida);

/*
 * Métricas e cálculos exigidos:
 */

// Soma de todos os modificadores do item: PODER(i)
int catalogo_calcular_poder(const Catalogo *catalogo, int id_item);

// Desvio populacional e EQUILIBRIO(i) para um dado total de atributos T
double catalogo_calcular_equilibrio(const Catalogo *catalogo, int id_item, int total_atributos);

// Soma dos quadrados dos modificadores do item: sum(v_i(a)^2)
long long catalogo_calcular_soma_quadrados(const Catalogo *catalogo, int id_item);

// Produto interno entre a linha do item e um vetor denso da build de tamanho total_atributos: sum(v_i(a) * b_h(a))
long long catalogo_produto_interno_com_vetor(const Catalogo *catalogo, int id_item, const int *vetor_build, int total_atributos);

#endif // CATALOGO_H
