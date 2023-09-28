Gustavo Mariz - 121073784
Laboratório 4

Nesse laboratório fizemos a comparação entre 3 execuções diferentes:
- exclusão mútua
- rwlock
- implementação das funções de entrada e saída

Era esperado que tanto a rwlock quanto a com implementação das funções de entrada e saída
tivesse um resultado em relação ao tempo de execução melhor do que o por exclusão mútua, dado que
temos um programa que trabalha majoritariamente com leituras.
Todavia, nas execuções, tivemos diversos imprevistos. Os tempos não foram como os esperados,
em que a execuçao mais rápida foi a por exclusão mútua.
Assim, acredito que tal resultado seja enviesado por algum problema na minha máquina ou versão do gcc,
já que esse foge do esperado e do lógico.

exclusão mútua:
Tempo para 1 thread: 0.910655
Tempo para 2 thread: 1.644733
Tempo para 4 thread: 1.460221

rwlock:
Tempo para 1 thread: 1.418061
Tempo para 2 thread: 3.151083
Tempo para 4 thread: 4.148536

funções implementadas:
Tempo para 1 thread: 0.686626
Tempo para 2 thread: 1.500641
Tempo para 4 thread: 6.711075