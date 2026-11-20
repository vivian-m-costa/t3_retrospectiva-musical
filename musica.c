#include <stdio.h>
#include <stdlib.h>
#include "musica.h"

struct NoMusica {
    int u;                      /* chave: número do usuario */
    int altura;
    struct NoMusica *esq, *dir;
};


static int alt(const NoMusica *n) { return n ? n->altura : 0; }

static void atualiza(NoMusica *n) {
    int he = alt(n->esq), hd = alt(n->dir);
    n->altura = 1 + (he > hd ? he : hd);
}

static NoMusica *rot_dir(NoMusica *y) {
    NoMusica *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualiza(y);
    atualiza(x);
    return (x);
}

static NoMusica *rot_esq(NoMusica *x) {
    NoMusica *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualiza(x);
    atualiza(y);
    return (y);
}

static NoMusica *balancear(NoMusica *n) {
    atualiza(n);
    int fb = alt(n->esq) - alt(n->dir);
    if (fb > 1) {
        if (alt(n->esq->esq) < alt(n->esq->dir))
            n->esq = rot_esq(n->esq);
        return (rot_dir(n));
    }
    if (fb < -1) {
        if (alt(n->dir->dir) < alt(n->dir->esq))
            n->dir = rot_dir(n->dir);
        return (rot_esq(n));
    }
    return (n);
}


NoMusica *Musica_inserir(NoMusica *raiz, int u) {
    if (!raiz) {
        NoMusica *n = malloc(sizeof *n);
        if (!n) {
            fputs("sem memoria\n", stderr);
            exit(1);
        }
        n->u = u;
        n->esq = n->dir = NULL;
        n->altura = 1;
        return (n);
    }
    if (u == raiz->u){
        return (raiz);
    }
    if (u < raiz->u){
        raiz->esq = Musica_inserir(raiz->esq, u);
    }
    else{
        raiz->dir = Musica_inserir(raiz->dir, u);
    }
    return (balancear(raiz));
}

NoMusica *Musica_remover(NoMusica *raiz, int u) {
    if (!raiz){
        return (NULL);
    }

    if (u < raiz->u) {
        raiz->esq = Musica_remover(raiz->esq, u);
    } else if (u > raiz->u) {
        raiz->dir = Musica_remover(raiz->dir, u);
    } else {
        if (!raiz->esq || !raiz->dir) {
            NoMusica *filho = raiz->esq ? raiz->esq : raiz->dir;
            free(raiz);
            return (filho);
        }
        /* dois filhos: copia a chave do sucessor e apaga o sucessor */
        NoMusica *suc = raiz->dir;
        while (suc->esq) suc = suc->esq;
        raiz->u = suc->u;
        raiz->dir = Musica_remover(raiz->dir, suc->u);
    }
    return (balancear(raiz));
}

int Musica_contem(const NoMusica *raiz, int u) {
    while (raiz) {
        if (u == raiz->u) return 1;
        raiz = (u < raiz->u) ? raiz->esq : raiz->dir;
    }
    return (0);
}

void Musica_esvaziar(NoMusica *raiz, void (*cb)(int u, void *ctx), void *ctx) {
    if (!raiz){
        return;
    }
    Musica_esvaziar(raiz->esq, cb, ctx);
    if (cb){
        cb(raiz->u, ctx);
    }
    Musica_esvaziar(raiz->dir, cb, ctx);
    free(raiz);
}