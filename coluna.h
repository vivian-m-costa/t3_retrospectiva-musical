#ifndef COLUNA_H
#define COLUNA_H

// AVL por número do usuário = uma coluna da tabela (uma música).
typedef struct NoColuna NoColuna;

// Insere o usuário u (não faz nada se já existe), devolve a nova raiz.
NoColuna *coluna_inserir(NoColuna *raiz, int u);

// Apaga o usuário u (não faz nada se ausente), devolve a nova raiz.
NoColuna *coluna_remover(NoColuna *raiz, int u);

// Verifica se o usuário está na coluna
int coluna_contem(const NoColuna *raiz, int u);

void coluna_esvaziar(NoColuna *raiz, void (*cb)(int u, void *ctx), void *ctx);

#endif