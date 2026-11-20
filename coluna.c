#include <stdio.h>
#include <stdlib.h>
#include "coluna.h"

struct NoColuna {
    int u;                      /* chave: numero do usuario */
    int altura;
    struct NoColuna *esq, *dir;
};

static int alt(const NoColuna *n) { return n ? n->altura : 0; }

static void atualiza(NoColuna *n) {
    int he = alt(n->esq), hd = alt(n->dir);
    n->altura = 1 + (he > hd ? he : hd);
}

static NoColuna *rot_dir(NoColuna *y) {
    NoColuna *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualiza(y);
    atualiza(x);
    return x;
}

static NoColuna *rot_esq(NoColuna *x) {
    NoColuna *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualiza(x);
    atualiza(y);
    return y;
}

static NoColuna *balancear(NoColuna *n) {
    atualiza(n);
    int fb = alt(n->esq) - alt(n->dir);
    if (fb > 1) {
        if (alt(n->esq->esq) < alt(n->esq->dir))
            n->esq = rot_esq(n->esq);
        return rot_dir(n);
    }
    if (fb < -1) {
        if (alt(n->dir->dir) < alt(n->dir->esq))
            n->dir = rot_dir(n->dir);
        return rot_esq(n);
    }
    return n;
}


NoColuna *coluna_inserir(NoColuna *raiz, int u) {
    if (!raiz) {
        NoColuna *n = malloc(sizeof *n);
        if (!n) {
            fputs("erro: sem memoria\n", stderr);
            exit(1);
        }
        n->u = u;
        n->esq = n->dir = NULL;
        n->altura = 1;
        return n;
    }
    if (u == raiz->u) return raiz;          /* ja estava: nada a fazer */
    if (u < raiz->u) raiz->esq = coluna_inserir(raiz->esq, u);
    else             raiz->dir = coluna_inserir(raiz->dir, u);
    return balancear(raiz);
}

NoColuna *coluna_remover(NoColuna *raiz, int u) {
    if (!raiz) return NULL;

    if (u < raiz->u) {
        raiz->esq = coluna_remover(raiz->esq, u);
    } else if (u > raiz->u) {
        raiz->dir = coluna_remover(raiz->dir, u);
    } else {
        if (!raiz->esq || !raiz->dir) {
            NoColuna *filho = raiz->esq ? raiz->esq : raiz->dir;
            free(raiz);
            return filho;
        }
        /* dois filhos: copia a chave do sucessor e apaga o sucessor */
        NoColuna *suc = raiz->dir;
        while (suc->esq) suc = suc->esq;
        raiz->u = suc->u;
        raiz->dir = coluna_remover(raiz->dir, suc->u);
    }
    return balancear(raiz);
}

int coluna_contem(const NoColuna *raiz, int u) {
    while (raiz) {
        if (u == raiz->u) return 1;
        raiz = (u < raiz->u) ? raiz->esq : raiz->dir;
    }
    return 0;
}

void coluna_esvaziar(NoColuna *raiz, void (*cb)(int u, void *ctx), void *ctx) {
    if (!raiz) return;
    coluna_esvaziar(raiz->esq, cb, ctx);
    if (cb) cb(raiz->u, ctx);
    coluna_esvaziar(raiz->dir, cb, ctx);
    free(raiz);
}