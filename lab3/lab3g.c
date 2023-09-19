/* Disciplina: Programacao Concorrente */
/* Nome: Gustavo Vilares Mariz de Oliveira */
/* DRE: 121073784 */
/* Laboratório: 3 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int nthreads = 0; //variável global para o número de threads
int cont_bar = 0; //variável global para a contagem da barreira
pthread_mutex_t mutex;
pthread_cond_t cond_bar;

//Implementação da barreira
void barreira(int numt) {
   pthread_mutex_lock(&mutex);
   cont_bar++;
   if (cont_bar < numt) {
       pthread_cond_wait(&cond_bar, &mutex);
   } else {
       cont_bar=0;
       pthread_cond_broadcast(&cond_bar);
   }
   pthread_mutex_unlock(&mutex);
}


void *greetings(void *t) {


  int *my_id = (int*)t;
  printf("Olá da thread %d\n", my_id);

 //Espera todas as threads
  barreira(nthreads);
  printf("Que dia bonito thread %d\n", my_id);

  //Espera todas as threads
  barreira(nthreads);
  printf("Até breve da thread %d\n", my_id);
  
 free(my_id);
 pthread_exit(NULL);
}




int main(int argc, char *argv[]){

   nthreads = atoi(argv[1]); //define o número de threads na var global
   pthread_t threads[nthreads];

   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init (&cond_bar, NULL);

   //Cria as threads, passando a função e o índice da thread.
   for (int i = 0; i < nthreads; i++){
      pthread_create(&threads[i], NULL, greetings, (void *)(i+1));
   }


   //Espera todas as threads chegarem
   for (int i = 0; i < nthreads; i++) {
       pthread_join(threads[i], NULL);
   }

   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_bar);


   return 0;
}
