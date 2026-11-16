Relatório: https://www.overleaf.com/project/6afaff1bf91cb6c11ac42296/share#e64997d0bc9ef2c7a6caeb244e687742ecbb66433bfaa83f

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
