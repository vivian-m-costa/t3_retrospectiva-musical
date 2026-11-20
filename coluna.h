#ifndef COLUNA_H
#define COLUNA_H

/* AVL por numero do usuario = UMA COLUNA da tabela (uma musica).
 * So guarda QUEM ouviu; a contagem fica na linha do usuario. */
typedef struct NoColuna NoColuna;

/* Insere o usuario u (nao faz nada se ja existe). Devolve a nova raiz. */
NoColuna *coluna_inserir(NoColuna *raiz, int u);

/* Apaga o usuario u (nao faz nada se ausente). Devolve a nova raiz. */
NoColuna *coluna_remover(NoColuna *raiz, int u);

/* 1 se u esta na coluna, 0 se nao. */
int coluna_contem(const NoColuna *raiz, int u);

/* Chama cb(u, ctx) para cada usuario (ordem crescente) e libera a arvore.
 * cb pode ser NULL. O chamador deve fazer musica->coluna = NULL. */
void coluna_esvaziar(NoColuna *raiz, void (*cb)(int u, void *ctx), void *ctx);

#endif