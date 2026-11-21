#include <stdio.h>
#include <string.h>
#include "faixa.h"
#include "ranking.h"
#include "usuarios.h"
#include "coluna.h"

static Musica musicas[Max + 1];
static NoUsuario *linhas[Usuariosmax + 1];
static Musica *raiz_ranking = NULL;
static long long deslocamento = 0;

static void comando_ouve(int u, int m) {
    Musica *mu = &musicas[m];
    int novo;

    linhas[u] = usuario_incrementar(linhas[u], m, &novo);
    if (novo)
        mu->coluna = coluna_inserir(mu->coluna, u);

    if (mu->noRanking) {
        raiz_ranking = ranking_remover(raiz_ranking, mu);
        mu->valor += 1;
    } else {
        mu->valor = 1 - deslocamento;
        mu->noRanking = 1;
    }
    raiz_ranking = ranking_inserir(raiz_ranking, mu);
}

static void comando_quantas(int u, int m) {
    printf("%d\n", usuario_contagem(linhas[u], m));
}

static void comando_posicao(int m) {
    if (!musicas[m].noRanking) puts("-");
    else printf("%d\n", ranking_posicao(raiz_ranking, &musicas[m]));
}

static void comando_kesima(int k) {
    const Musica *r = ranking_kesima(raiz_ranking, k);
    if (r) printf("%d\n", r->id);
    else   puts("-");
}

static void comando_favorita(int u) {
    int m = usuario_favorita(linhas[u]);
    if (m) printf("%d\n", m);
    else   puts("-");
}

/* Para cada celula (m, cnt) da linha apagada: u sai da coluna de m e o
 * total de m cai cnt. Se o total real chega a zero, m sai do ranking;
 * os impulsos ficam, entao musica com impulsos continua. */
static void esquece_cb(int m, int cnt, void *ctx) {
    int u = *(int *)ctx;
    Musica *mu = &musicas[m];

    mu->coluna = coluna_remover(mu->coluna, u);
    raiz_ranking = ranking_remover(raiz_ranking, mu);
    mu->valor -= cnt;
    if (mu->valor + deslocamento > 0) {
        raiz_ranking = ranking_inserir(raiz_ranking, mu);
    } else {
        mu->noRanking = 0;
        mu->valor = 0;
    }
}

static void comando_esquece(int u) {
    NoUsuario *linha = linhas[u];
    linhas[u] = NULL;
    usuario_esvaziar(linha, esquece_cb, &u);
}

/* IMPULSO x: O(1). Somar o mesmo x a todo o ranking nao muda a ordem. */
static void comando_impulso(int x) {
    if (raiz_ranking){
        deslocamento += x;
}}

/* Para cada usuario u que ouviu m: apaga m da linha de u. */
static void retira_cb(int u, void *ctx) {
    int m = *(int *)ctx;
    linhas[u] = usuario_remover(linhas[u], m);
}

static void comando_retira(int m) {
    Musica *mu = &musicas[m];
    if (!mu->noRanking){
        return;
    }
    coluna_esvaziar(mu->coluna, retira_cb, &m);
    mu->coluna = NULL;
    raiz_ranking = ranking_remover(raiz_ranking, mu);
    mu->noRanking = 0;
    mu->valor = 0;
}

/* FAIXA a b: total real em [a, b]  <=>  valor em [a - G, b - G]. */
static void comando_faixa(int a, int b) {
    int ate_b_mais_1 = ranking_contar_ge(raiz_ranking, (long long)b - deslocamento + 1);
    int de_a = ranking_contar_ge(raiz_ranking, (long long)a - deslocamento);
    printf("%d\n", de_a - ate_b_mais_1);
}

static void libera_tudo(void) {
    for (int u = 1; u <= Usuariosmax; u++)
        usuario_esvaziar(linhas[u], NULL, NULL);
    for (int m = 1; m <= Max; m++)
        coluna_esvaziar(musicas[m].coluna, NULL, NULL);
}

int main(void) {
    char cmd[16];
    int a, b;

    for (int m = 1; m <= Max; m++) musicas[m].id = m;

    while (scanf("%15s", cmd) == 1 && strcmp(cmd, "FIM") != 0) {
        if (strcmp(cmd, "OUVE") == 0) {
            if (scanf("%d %d", &a, &b) == 2) comando_ouve(a, b);
        } else if (strcmp(cmd, "QUANTAS") == 0) {
            if (scanf("%d %d", &a, &b) == 2) comando_quantas(a, b);
        } else if (strcmp(cmd, "POSICAO") == 0) {
            if (scanf("%d", &a) == 1) comando_posicao(a);
        } else if (strcmp(cmd, "KESIMA") == 0) {
            if (scanf("%d", &a) == 1) comando_kesima(a);
        } else if (strcmp(cmd, "FAVORITA") == 0) {
            if (scanf("%d", &a) == 1) comando_favorita(a);
        } else if (strcmp(cmd, "ESQUECE") == 0) {
            if (scanf("%d", &a) == 1) comando_esquece(a);
        } else if (strcmp(cmd, "IMPULSO") == 0) {
            if (scanf("%d", &a) == 1) comando_impulso(a);
        } else if (strcmp(cmd, "RETIRA") == 0) {
            if (scanf("%d", &a) == 1) comando_retira(a);
        } else if (strcmp(cmd, "FAIXA") == 0) {
            if (scanf("%d %d", &a, &b) == 2) comando_faixa(a, b);
        }
    }

    libera_tudo();
    return 0;
}