#include <stdio.h>
#include "ranking.h"

static int alt(const Musica *n) { return n ? n->altura : 0; }

int ranking_tamanho(const Musica *raiz) { return raiz ? raiz->tam : 0; }

// a vem antes de b no ranking?
static int antes(const Musica *a, const Musica *b) {
    if (a->valor != b->valor) return a->valor > b->valor;
    return a->id < b->id;
}

// Recalcula altura e tam a partir dos filhos
static void atualiza(Musica *n) {
    int he = alt(n->esq), hd = alt(n->dir);
    n->altura = 1 + (he > hd ? he : hd);
    n->tam = 1 + ranking_tamanho(n->esq) + ranking_tamanho(n->dir);
}

static Musica *rot_dir(Musica *y) {
    Musica *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualiza(y);            // y agora é filho: atualiza primeiro
    atualiza(x);
    return x;
}

static Musica *rot_esq(Musica *x) {
    Musica *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualiza(x);
    atualiza(y);
    return y;
}

// Atualiza n e rebalanceia se preciso. Devolve a raiz da subarvore.
static Musica *balancear(Musica *n) {
    atualiza(n);
    int fb = alt(n->esq) - alt(n->dir);
    if (fb > 1) {
        if (alt(n->esq->esq) < alt(n->esq->dir))   // caso esq-dir
            n->esq = rot_esq(n->esq);
        return rot_dir(n);
    }
    if (fb < -1) {
        if (alt(n->dir->dir) < alt(n->dir->esq))   // caso dir-esq
            n->dir = rot_dir(n->dir);
        return rot_esq(n);
    }
    return n;
}


Musica *ranking_inserir(Musica *raiz, Musica *m) {
    if (!raiz) {
        m->esq = m->dir = NULL;
        m->altura = 1;
        m->tam = 1;
        return m;
    }
    if (antes(m, raiz)) raiz->esq = ranking_inserir(raiz->esq, m);
    else                raiz->dir = ranking_inserir(raiz->dir, m);
    return balancear(raiz);
}

// Tira o menor no da subarvore; devolve a nova raiz e o no em *min.
static Musica *remover_min(Musica *raiz, Musica **min) {
    if (!raiz->esq) {
        *min = raiz;
        return raiz->dir;
    }
    raiz->esq = remover_min(raiz->esq, min);
    return balancear(raiz);
}

Musica *ranking_remover(Musica *raiz, Musica *m) {
    if (!raiz) return NULL;

    if (raiz == m) {
        Musica *sub;
        if (!raiz->esq)      sub = raiz->dir;
        else if (!raiz->dir) sub = raiz->esq;
        else {
            // Dois filhos: o sucessor (menor da direita) assume o lugar.
            Musica *suc;
            Musica *nova_dir = remover_min(raiz->dir, &suc);
            suc->esq = raiz->esq;
            suc->dir = nova_dir;
            sub = balancear(suc);
        }
        raiz->esq = raiz->dir = NULL; 
        raiz->altura = 1;
        raiz->tam = 1;
        return sub;
    }

    if (antes(m, raiz)) raiz->esq = ranking_remover(raiz->esq, m);
    else                raiz->dir = ranking_remover(raiz->dir, m);
    return balancear(raiz);
}


int ranking_posicao(const Musica *raiz, const Musica *m) {
    int antes_de_m = 0;
    while (raiz) {
        if (raiz == m) return antes_de_m + ranking_tamanho(raiz->esq) + 1;
        if (antes(m, raiz)) {
            raiz = raiz->esq;
        } else {
            antes_de_m += ranking_tamanho(raiz->esq) + 1;
            raiz = raiz->dir;
        }
    }
    return 0;
}

const Musica *ranking_kesima(const Musica *raiz, int k) {
    while (raiz) {
        int e = ranking_tamanho(raiz->esq);
        if (k <= e)          raiz = raiz->esq;
        else if (k == e + 1) return raiz;
        else { k -= e + 1;   raiz = raiz->dir; }
    }
    return NULL;
}

int ranking_contar_ge(const Musica *raiz, long long v) {
    int cont = 0;
    while (raiz) {
        if (raiz->valor >= v) {
            cont += ranking_tamanho(raiz->esq) + 1;
            raiz = raiz->dir;
        } else {
            raiz = raiz->esq;
        }
    }
    return cont;
}
