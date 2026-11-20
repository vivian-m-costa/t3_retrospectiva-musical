#ifndef MUSICA_H
#define MUSICA_H

//AVL por número do usuário = uma da tabela (uma música).
typedef struct NoMusica NoMusica;

// Insere o usuário u (não faz nada se já existe), devolve a nova raiz.
NoMusica *coluna_inserir(NoMusica *raiz, int u);

// Apaga o usuário u (não faz nada se ausente), devolve a nova raiz.
NoMusica *coluna_remover(NoMusica *raiz, int u);

// verifica se o usuário está na coluna
int coluna_contem(const NoMusica *raiz, int u);

void coluna_esvaziar(NoMusica *raiz, void (*cb)(int u, void *ctx), void *ctx);

#endif