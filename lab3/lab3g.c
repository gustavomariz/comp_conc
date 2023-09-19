/* Disciplina: Programacao Concorrente */
/* Nome: Gustavo Vilares Mariz de Oliveira */
/* DRE: 121073784 */
/* Laboratório: 3 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define NTHREADS 3


int cont_bar = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_bar;

//Implementação da barreira
void barreira(int nthreads) {
   pthread_mutex_lock(&mutex);
   cont_bar++;
   if (cont_bar < nthreads) {
       pthread_cond_wait(&cond_bar, &mutex);
   } else {
       cont_bar=0;
       pthread_cond_broadcast(&cond_bar);
   }
   pthread_mutex_unlock(&mutex);
}


void *greetings(void *t) {


  int *my_id = (int*)t, i;
  printf("Olá da thread %d\n", *my_id);

 //Espera todas as threads
  barreira(NTHREADS);
  printf("Que dia bonito thread %d\n", *my_id);

  //Espera todas as threads
  barreira(NTHREADS);
  printf("Até breve da thread %d\n", *my_id);
  
 free(my_id);
 pthread_exit(NULL);
}




int main(int argc, char *argv[]){

   //Gerando variáveis para passar o índice da thread
   int *t1, *t2, *t3;
   pthread_t threads[NTHREADS];
   t1=malloc(sizeof(int));
   t2=malloc(sizeof(int));
   t3=malloc(sizeof(int));
   *t1=1, *t2=2, *t3=3;


   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init (&cond_bar, NULL);

   //Cria as três threads, passando a função e o índice da thread.
   pthread_create(&threads[0], NULL, greetings, (void *)t1);
   pthread_create(&threads[2], NULL, greetings, (void *)t3);
   pthread_create(&threads[1], NULL, greetings, (void *)t2);


   //Espera todas as threads chegarem
   for (int i = 0; i < NTHREADS; i++) {
       pthread_join(threads[i], NULL);
   }
   printf ("Acabou!\n");

   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_bar);


   return 0;
}
