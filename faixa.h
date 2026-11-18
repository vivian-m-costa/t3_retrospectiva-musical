#ifndef FAIXA_H
#define FAIXA_H

#define Max 100000
#define Usuariosmax    100000

/* No da AVL da coluna de uma musica: so guarda QUEM ouviu (sem contagem;
 * a contagem fica na linha do usuario). Definido em coluna.c. */
struct NoColuna;

/* Uma musica. O mesmo registro serve como no da AVL do ranking, entao
 * as musicas ficam num vetor indexado pelo numero (sem malloc por no). Nao poder ser uma heap,
 devido a falta de ordem interna*/
typedef struct Musica {
    int id;                     /* numero da musica (1..ACERVO_MAXIMO_100K) */

    /* total real = valor + deslocamento (deslocamento = soma dos IMPULSOs).
     * long long so para nao se preocupar com estouro nas contas. */
    long long valor;
    int noRanking;              /* 1 se total real > 0 */

    /* campos da AVL do ranking */
    struct Musica *esq, *dir;
    int altura;
    int tam;                    /* nos na subarvore, para POSICAO/KESIMA/FAIXA */

    /* coluna: usuarios que ouviram esta musica (para o RETIRA) */
    struct NoColuna *coluna;
} Musica;

#endif
