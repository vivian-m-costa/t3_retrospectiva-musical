# t3_retrospectiva-musical
Comando	Como	Custo
OUVE	incrementa a célula na AVL do usuário, tira a música do ranking, soma 1 e reinsere	O(log n)
QUANTAS	busca na AVL do usuário	O(log d)
POSICAO	desce na AVL do ranking somando os tamanhos das subárvores	O(log n)
KESIMA	seleção por tamanho de subárvore	O(log n)
FAVORITA	lê o “melhor” da raiz da linha	O(1)
ESQUECE	percorre a linha, ajusta cada música no ranking e libera a linha	O(d log n)
RETIRA	percorre a coluna removendo a célula de cada usuário e tira a música do ranking	O(c log n)
FAIXA	duas contagens “quantos têm total ≥ v” na AVL do ranking	O(log n)
IMPULSO	soma x numa variável global G, sem tocar nas músicas	O(1)
