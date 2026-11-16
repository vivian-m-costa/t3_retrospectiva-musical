Relatório: https://www.overleaf.com/project/6afaff1bf91cb6c11ac42296/share#e64997d0bc9ef2c7a6caeb244e687742ecbb66433bfaa83f


# Ritmoteca: retrospectiva musical

Trabalho Prático 3 de **SCC0223 – Estruturas de Dados I** (ICMC-USP, 2º semestre de 2026).

O programa lê comandos da entrada padrão e responde consultas sobre quantas vezes cada usuário ouviu cada música e sobre o ranking geral das mais tocadas. A tabela completa (100 000 × 100 000) não cabe na memória, então guardamos **apenas as células não nulas**.

## Sumário

- [Comandos](#comandos)
- [Estruturas de dados](#estruturas-de-dados)
- [Organização dos arquivos](#organização-dos-arquivos)
- [Como compilar e usar](#como-compilar-e-usar)
- [Testes](#testes)
- [Medição de desempenho](#medição-de-desempenho)
- [Status do trabalho](#status-do-trabalho)
- [Grupo](#grupo)

## Comandos

| Comando | Efeito | Saída |
|---|---|---|
| `OUVE u m` | o usuário `u` ouve a música `m` uma vez | nenhuma |
| `QUANTAS u m` | quantas vezes `u` ouviu `m` | o número |
| `POSICAO m` | posição de `m` no ranking geral | posição (a partir de 1) ou `-` |
| `KESIMA k` | música na posição `k` do ranking | número da música ou `-` |
| `FAVORITA u` | música que `u` mais ouviu (empate: menor número) | número da música ou `-` |
| `ESQUECE u` | apaga o histórico de `u` (os impulsos das músicas ficam) | nenhuma |
| `IMPULSO x` | toda música já no ranking ganha `x` execuções no total | nenhuma |
| `RETIRA m` | `m` sai do catálogo, com suas contagens e impulsos | nenhuma |
| `FAIXA a b` | quantas músicas têm total entre `a` e `b` | a contagem |
| `FIM` | encerra | nenhuma |

**Ranking geral:** músicas com total > 0, em ordem decrescente de total; no empate, vem antes a de menor número.

**Limites:** até 100 000 usuários, 100 000 músicas e 200 000 comandos; `x` do `IMPULSO` entre 1 e 1 000.

## Estruturas de dados

Três estruturas trabalham juntas, e o `main.c` faz a ligação entre elas.

| Estrutura | Chave | Guarda | Atende |
|---|---|---|---|
| **AVL da linha** (uma por usuário) | nº da música | contagem e ponteiro `melhor` da subárvore | `QUANTAS`, `FAVORITA`, `ESQUECE` |
| **AVL da coluna** (uma por música) | nº do usuário | só quem ouviu | `RETIRA` |
| **AVL do ranking** (única) | (total decrescente, nº da música crescente) | tamanho da subárvore | `POSICAO`, `KESIMA`, `FAIXA` |
| **Variável `deslocamento`** | n/a | soma dos impulsos | `IMPULSO` em O(1) |

### Ideias principais

- **FAVORITA em O(1):** cada nó da linha guarda o melhor nó da sua subárvore. A raiz sempre sabe o favorito, e o valor é recalculado no caminho de volta da recursão.
- **RETIRA sem visitar todos os usuários:** a coluna de cada música lista quem a ouviu, então só essas linhas são visitadas.
- **POSICAO, KESIMA e FAIXA em O(log n):** o tamanho da subárvore em cada nó permite contar quantos nós vêm antes de uma chave, sem percorrer a árvore.
- **IMPULSO em O(1):** somar o mesmo valor a todas as músicas não muda a ordem entre elas. O total real é `valor + deslocamento`. Uma música que entra no ranking guarda `1 - deslocamento`, então não recebe impulsos anteriores.
- **Mudança de total:** a chave do ranking muda, então o nó é **removido e reinserido** (nunca alterado no lugar).

### Custo por comando

| Comando | Estruturas tocadas | Custo |
|---|---|---|
| `OUVE` | linha + coluna + ranking | O(log n) |
| `QUANTAS` | linha | O(log k) |
| `POSICAO` | ranking | O(log n) |
| `KESIMA` | ranking | O(log n) |
| `FAVORITA` | linha | O(1) |
| `ESQUECE` | linha + coluna + ranking | O(k log n) |
| `IMPULSO` | `deslocamento` | O(1) |
| `RETIRA` | coluna + linhas + ranking | O(c log n) |
| `FAIXA` | ranking | O(log n) |

`k` é o número de músicas distintas do usuário e `c` é o número de usuários que ouviram a música. A comparação completa com outras estruturas (ABB, vetor ordenado, heap) está no relatório.

## Organização dos arquivos

### Cabeçalhos (`.h`)

| Arquivo | O que expõe |
|---|---|
| `faixa.h` | `FaixaTocada`, `ACERVO_MAXIMO_100K`, `USUARIOS_MAXIMO` |
| `usuarios.h` | `NoUsuario` (opaco), `usuario_incrementar`, `usuario_contagem`, `usuario_favorita`, `usuario_remover`, `usuario_esvaziar` |
| `coluna.h` | `NoColuna` (opaco), inserção de usuário, percurso e esvaziamento |
| `ranking.h` | `ranking_inserir`, `ranking_remover`, `ranking_tamanho`, `ranking_posicao`, `ranking_kesima`, `ranking_contar_ge` |

### Implementações (`.c`)

| Arquivo | Papel |
|---|---|
| `usuarios.c` | AVL da linha, com campo `melhor` |
| `coluna.c` | AVL da coluna, sem contagem |
| `ranking.c` | AVL com tamanho de subárvore |
| `main.c` | leitura dos comandos, variável `deslocamento` e lógica de cada comando |
| `gerador.c` | programa **separado** que imprime as rajadas de teste; não entra no executável principal |
| `Makefile` | compila os dois executáveis (`ritmoteca` e `gerador`) |

### Dependências

```
faixa.h ──► ranking.h / ranking.c
        └─► coluna.h  / coluna.c
usuarios.h / usuarios.c    (independente)
main.c ──► usa as quatro estruturas
gerador.c                  (independente, outro executável)
```

`usuarios.c`, `coluna.c` e `ranking.c` não incluem uns aos outros, o que permite testar cada TAD separadamente.

## Como compilar e usar

```bash
make                       # gera ./ritmoteca e ./gerador
./ritmoteca < entrada.txt  # lê comandos da entrada padrão
```

Flags usadas: `-std=c11 -Wall -Wextra -O2`. O código deve compilar **sem avisos**.

Exemplo de entrada:

```
OUVE 1 10
OUVE 1 10
OUVE 2 20
POSICAO 10
KESIMA 1
FIM
```

## Testes

Os três exemplos do enunciado (seções 5.1, 5.2 e 5.3) e os casos da seção 4 são o ponto de partida. Além deles:

- compare a saída com um verificador força-bruta simples em Python, usando entradas aleatórias pequenas;
- teste os casos de borda: um único usuário, 100 000 músicas distintas e 200 000 comandos;
- verifique vazamento de memória e acessos inválidos com o Valgrind:

```bash
valgrind --leak-check=full ./ritmoteca < entrada.txt
```

## Medição de desempenho

O `gerador` imprime `n` comandos `OUVE` aleatórios, depois `n` comandos de uma rajada e, por fim, `FIM`.

```bash
./gerador POSICAO 50000 > entrada.txt
time ./ritmoteca < entrada.txt
```

Rajadas disponíveis: `POSICAO`, `KESIMA`, `FAVORITA`, `IMPULSO`, `RETIRA`. O relatório usa `n = 12 500, 25 000, 50 000 e 100 000` e um gráfico log-log, em que a inclinação da reta indica o expoente de crescimento do tempo (≈ 1 para custo O(1) por comando, ≈ 2 para O(n)).

## Status do trabalho

- [ ] `faixa.h`
- [ ] `usuarios.h` / `usuarios.c`
- [ ] `coluna.h` / `coluna.c`
- [ ] `ranking.h` / `ranking.c`
- [ ] `main.c`
- [ ] `gerador.c`
- [ ] `Makefile`
- [ ] Verificação com Valgrind
- [ ] Medições e gráfico log-log
- [ ] Relatório (capa, projeto, justificativa, medição, ponto fraco, casos de teste)
- [ ] Entrega: `T3_<numeroUSP>.zip` com os arquivos na raiz, até **04/12/2026**

## Grupo

| Nome | Número USP |
|---|---|
| _preencher_ | _preencher_ |
| _preencher_ | _preencher_ |
| _preencher_ | _preencher_ |

# t3_retrospectiva-musical
<img width="941" height="535" alt="image" src="https://github.com/user-attachments/assets/00645628-b3cf-4da3-803a-e5762519210f" />

O truque do IMPULSO: cada música guarda raw, e o total real é raw + G. Uma música que entra no ranking recebe raw = 1 - G, então não ganha os impulsos anteriores. Como todas as músicas do ranking mudam juntas, a ordem relativa não se altera e a árvore continua válida. O FAIXA usa a mesma ideia, consultando [a-G, b-G].

Camada 0: o problema de fundo

A tabela completa teria 10¹⁰ células, mas só umas poucas centenas por usuário são diferentes de zero. Então guardamos só as células não nulas. O desafio é: sem a tabela inteira, como achar uma célula específica? E como fazer isso para linhas, colunas e ranking ao mesmo tempo?

Camada 1: a linha de cada usuário

Cada usuário tem uma coleção de pares (música, contagem). Precisamos de:

QUANTAS u m: achar a música m nessa coleção.
OUVE u m: achar m e somar 1, ou inserir se não existir.
FAVORITA u: achar a música de maior contagem.
ESQUECE u: percorrer tudo e apagar.

Se a coleção fosse uma lista, buscar m custaria O(k), onde k é o número de músicas do usuário. Uma AVL ordenada por número da música busca em O(log k) e garante isso mesmo se as músicas chegarem em ordem crescente (uma ABB simples viraria uma lista nesse caso).

Falta o FAVORITA. Achar o máximo percorrendo a árvore custa O(k). A solução é fazer cada nó guardar também um ponteiro para o melhor nó da sua subárvore. O melhor de um nó é ele mesmo, ou o melhor do filho esquerdo, ou o do direito, o que ganhar. Então a raiz sempre sabe quem é o favorito, e FAVORITA vira O(1).

Para pensar: quando um OUVE soma 1 na contagem de um nó lá no fundo da árvore, o “melhor” dos ancestrais dele pode mudar. O que precisa acontecer no caminho de volta da recursão para manter isso certo?

Camada 2: a coluna de cada música

RETIRA m precisa apagar m da linha de todo usuário que a ouviu. Se só existirem as linhas, teríamos que visitar os 100 000 usuários, um por um, para ver se m está lá. Isso é O(U) por RETIRA, e o enunciado avisa exatamente desse conflito (“uma representação que percorre apenas linhas tem alto custo no RETIRA”).

A solução é guardar também a visão por coluna: para cada música, uma AVL com os usuários que a ouviram. Agora RETIRA m percorre só essa árvore e apaga a música da linha de cada um desses usuários.

O preço é que cada OUVE passa a atualizar duas estruturas, e as duas precisam ficar consistentes. É a “conta” que o enunciado pede para mostrar no relatório: trocamos um custo extra pequeno em todo OUVE por um RETIRA barato.

Para pensar: a árvore da coluna precisa guardar a contagem também, ou só quem ouviu?

Camada 3: o ranking

O ranking contém as músicas com total > 0, ordenadas por total decrescente e, no empate, por menor número. Precisamos de duas perguntas opostas:

POSICAO m: em que posição está m?
KESIMA k: quem está na posição k?

Uma AVL comum ordenada por (total, número) responde “quem está na posição k” só percorrendo. O truque do FAQ (item 4) é fazer cada nó guardar quantos nós tem na sua subárvore. Com isso:

KESIMA k: olha o tamanho da subárvore esquerda e. Se k ≤ e, está à esquerda; se k = e+1, é este nó; senão, está à direita. Desce uma vez só: O(log n).
POSICAO m: conta quantos nós vêm antes de m, usando os mesmos tamanhos, e soma 1. O(log n).
FAIXA a b: é só “quantos têm total ≥ a” menos “quantos têm total > b”, duas contagens. O(log n).

Como a ordem é por duas coisas (total e número), a gente funde as duas em um único número, a chave composta. Ordenando por essa chave, sai exatamente a ordem do ranking. Vou mostrar a fórmula quando chegarmos lá.

Para pensar: quando o total de uma música muda (um OUVE), a chave dela muda. O que isso obriga a fazer na árvore do ranking? (Dica: a posição de um nó depende da chave.)

Camada 4: o IMPULSO

IMPULSO x soma x ao total de todas as músicas do ranking. Fazer isso música por música é O(n) por comando, e o gerador faz 50 000 deles. Seria desastroso.

A observação-chave é que somar o mesmo valor em todo mundo não muda a ordem entre eles. A árvore continua válida sem mexer em nada. Então guardamos uma variável global deslocamento e dizemos que:

total real de uma música = valor guardado na árvore + deslocamento

IMPULSO vira deslocamento += x, O(1). A sutileza é a música que entra no ranking depois do impulso: ela não pode receber o impulso antigo. Então, ao entrar, ela guarda 1 - deslocamento, de modo que o total real dela dê 1. É um detalhe pequeno, mas o exemplo 5.2 do enunciado testa exatamente isso (a música 40).

Para pensar: qual é o total real de uma música que entrou com 1 - deslocamento e logo depois houve um IMPULSO 5? E se o ranking estiver vazio no momento do IMPULSO, o que o enunciado manda fazer, e por que isso importa para o deslocamento?

Camada 5: ESQUECE e as “sobras” de impulso

O ESQUECE u apaga a linha de u, e as contagens dele saem do total de cada música. Mas os impulsos ficam. Por isso o exemplo 5.3 mostra uma música que continua no ranking só com impulsos. Isso diz que o total real de uma música é “soma das contagens dos usuários + impulsos”, e guardar tudo num único número (o valor guardado) funciona, porque ESQUECE só subtrai a parte dos usuários.

A regra para sair do ranking é: o total real chega a zero. Para uma música com impulso, o total real só chega a zero se... bom, essa é a terceira pergunta de pensar abaixo.

Resumo: as três estruturas e quem as usa

Estrutura	Chave	Serve para
AVL por usuário (linha)	nº da música, com melhor	QUANTAS, OUVE, FAVORITA, ESQUECE
AVL por música (coluna)	nº do usuário	RETIRA (e achar quem ouviu)
AVL do ranking	chave composta, com tamanho	POSICAO, KESIMA, FAIXA
Variável deslocamento	n/a	IMPULSO em O(1)

E o fluxo de um OUVE u m toca as três: atualiza a linha de u, registra u na coluna de m, e reposiciona m no ranking.
