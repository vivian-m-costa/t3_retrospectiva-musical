#ifndef RANKING_H
#define RANKING_H

#include "faixa.h"

// Insere m (m->valor ja definido). Devolve a nova raiz.
Musica *ranking_inserir(Musica *raiz, Musica *m);

// Remove m (usa o m->valor atual para achar o no). Devolve a nova raiz.
Musica *ranking_remover(Musica *raiz, Musica *m);

int ranking_tamanho(const Musica *raiz);

// equivale ao posicao: posicao (a partir de 1) de m; 0 se m nao esta na arvore.
int ranking_posicao(const Musica *raiz, const Musica *m);

// KESIMA: musica na posicao k (a partir de 1); NULL se k fora de 1..tamanho.
const Musica *ranking_kesima(const Musica *raiz, int k);

/* Quantas musicas tem valor >= v (v e o valor GUARDADO, nao o total real).
 * FAIXA a b = ge(a - G) - ge(b - G + 1), com G = deslocamento. */
int ranking_contar_ge(const Musica *raiz, long long v);

#endif
