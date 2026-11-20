#include <stdio.h>
#include <stdlib.h>
#include "usuarios.h"

struct NoUsuario {
    int m;                      // chave: id da música
    int cnt;                    // quantas vezes o usuario ouviu a música
    int altura;
    struct NoUsuario *esq, *dir;
    /* melhor celula da subarvore (guardada por VALOR, nao por ponteiro,
     * para nao ficar pendurada quando um no e liberado na remocao) */
    int melhor_m, melhor_cnt;
};


static int alt(const NoUsuario *n) { return n ? n->altura : 0; }

/* (c1, m1) e melhor que (c2, m2)? Maior contagem; empate: menor musica. */
static int melhor_que(int c1, int m1, int c2, int m2) {
    return (c1 > c2 || (c1 == c2 && m1 < m2));
}

// Recalcula altura e melhor, a partir do próprio nó e dos filhos.
static void atualiza(NoUsuario *n) {
    int he = alt(n->esq), hd = alt(n->dir);
    n->altura = 1 + (he > hd ? he : hd);

    n->melhor_m = n->m;
    n->melhor_cnt = n->cnt;
    if (n->esq && melhor_que(n->esq->melhor_cnt, n->esq->melhor_m,
                             n->melhor_cnt, n->melhor_m)) {
        n->melhor_m = n->esq->melhor_m;
        n->melhor_cnt = n->esq->melhor_cnt;
    }
    if (n->dir && melhor_que(n->dir->melhor_cnt, n->dir->melhor_m,
                             n->melhor_cnt, n->melhor_m)) {
        n->melhor_m = n->dir->melhor_m;
        n->melhor_cnt = n->dir->melhor_cnt;
    }
}

static NoUsuario *rot_dir(NoUsuario *y) {
    NoUsuario *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualiza(y);
    atualiza(x);
    return (x);
}

static NoUsuario *rot_esq(NoUsuario *x) {
    NoUsuario *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualiza(x);
    atualiza(y);
    return (y);
}

static NoUsuario *balancear(NoUsuario *n) {
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

NoUsuario *usuario_incrementar(NoUsuario *raiz, int m, int *novo) {
    if (!raiz) {
        NoUsuario *n = malloc(sizeof *n);
        if (!n) {
            fputs("erro: sem memoria\n", stderr);
            exit(1);
        }
        n->m = m;
        n->cnt = 1;
        n->esq = n->dir = NULL;
        atualiza(n);
        *novo = 1;
        return (n);
    }
    if (m == raiz->m) {
        raiz->cnt++;
        *novo = 0;
        /* a estrutura não muda, mas o melhor dos ancestrais pode mudar:
         * atualiza aqui e, na volta da recursão, cada ancestral refaz o seu */
        atualiza(raiz);
        return (raiz);
    }
    if (m < raiz->m){
        raiz->esq = usuario_incrementar(raiz->esq, m, novo);
    }else{
        raiz->dir = usuario_incrementar(raiz->dir, m, novo);
    }
    return (balancear(raiz));
}

int usuario_contagem(const NoUsuario *raiz, int m) {
    while (raiz) {
        if (m == raiz->m){
            return (raiz->cnt);
        }
        raiz = (m < raiz->m) ? raiz->esq : raiz->dir;
    }
    return (0);
}

int usuario_favorita(const NoUsuario *raiz) {
    return (raiz ? raiz->melhor_m : 0);
}

void usuario_esvaziar(NoUsuario *raiz,
    void (*cb)(int m, int cnt, void *ctx), void *ctx) {
    if (!raiz){
        return;
    }
    usuario_esvaziar(raiz->esq, cb, ctx);
    if (cb){
        cb(raiz->m, raiz->cnt, ctx);
    }
    usuario_esvaziar(raiz->dir, cb, ctx);
    free(raiz);
}

NoUsuario *usuario_remover(NoUsuario *raiz, int m) {
    if (!raiz){
        return (NULL);
    }
    if (m < raiz->m) {
        raiz->esq = usuario_remover(raiz->esq, m);
    } else if (m > raiz->m) {
        raiz->dir = usuario_remover(raiz->dir, m);
    } else {
        if (!raiz->esq || !raiz->dir) {
            NoUsuario *filho = raiz->esq ? raiz->esq : raiz->dir;
            free(raiz);
            return (filho);
        }
        NoUsuario *suc = raiz->dir;
        while (suc->esq) suc = suc->esq;
        raiz->m = suc->m;
        raiz->cnt = suc->cnt;
        raiz->dir = usuario_remover(raiz->dir, suc->m);
    }
    return (balancear(raiz));
}