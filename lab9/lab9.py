#Nome: Gustavo Vilares Mariz de Oliveira
#DRE: 121073784
#Computação Concorrente - Laboratório 9


from multiprocessing.pool import Pool
import time
import math
import sys


#Função para definir se um número é primo
def ehPrimo(n):
   if (n<=1):
       return 0
   if (n==2):
       return 1
   if (n%2==0):
       return 0
   i = 3
   while(i<math.sqrt(n)+1):
       if(n%i==0):
           return 0
       i+=2
   return 1

#Definindo um método sequencial para verificar corretude
def PrimoSequencial(n):
    cnt = 0
    for i in range(n):
        if(ehPrimo(i) == 1):
            cnt += 1
    return cnt

#funcao que sera executada de forma concorrente
def task(x):
  return ehPrimo(x)


if __name__ == '__main__':
   
   #Obter argumento pela linha de comando e garantir que ele é válido
    if len(sys.argv) == 1:
        print("Escreva um N desejado na linha de comando ao executar")
        exit()

    N = 0
    try:
        N = int(sys.argv[1])
        if N <= 0:
            raise Exception
    except:
        print("N inválido!")
        exit()

   
    start = time.time()
    pool = Pool() #por default, cria um processo distinto para cada processador da maquina


    numbers = list(range(1, N))
    #map aceita uma funcao e um objeto iteravel, o pool pega cada valor do objeto iteravel e passa para um processo disponivel no poll que aplica a funcao sobre esse valor
    results = pool.map(task, numbers)

    #Os processos vão armazenar no map 0 ou 1 em cada posição
    #Assim, somamos todas e garantimos quantos primos foram identificados
    final = sum(results)

    end = time.time()

    #Executando a versão sequencial
    seq = PrimoSequencial(N)

    #Imprimindo os resultados e testando a corretude do código concorrente
    print(f"Resultado encontrado: {final}")
    print(f'O programa levou {(end - start)} segundos')
    if(seq != final):
       print("Problema no código, sequencial encontrou valor diferente")
    else:
       print("Tudo certo! Corretude garantida.")


