#ifndef USUARIOS_H
#define USUARIOS_H

/* AVL por numero da musica. Cada no guarda a contagem e o "melhor" no da sua subarvore (FAVORITA em O(1)). */
typedef struct NoUsuario NoUsuario;

/* OUVE: soma 1 na celula (cria se nao existir). *novo = 1 se criou. */
NoUsuario *usuario_incrementar(NoUsuario *raiz, int m, int *novo);

/* QUANTAS: contagem da celula, 0 se nao existe. */
int usuario_contagem(const NoUsuario *raiz, int m);

/* FAVORITA: musica de maior contagem (menor caso empate); 0 se u vazia. */
int usuario_favorita(const NoUsuario *raiz);

/* ESQUECE: chama cb(m, cnt, ctx) em cada celula e libera a arvore. */
void usuario_esvaziar(NoUsuario *raiz,
void (*cb)(int m, int cnt, void *ctx), void *ctx);

/* RETIRA: remove a celula de m do catalogo (nao faz nada se ausente). */
NoUsuario *usuario_remover(NoUsuario *raiz, int m);


#endif
