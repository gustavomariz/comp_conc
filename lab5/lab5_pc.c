/* Disciplina: Programação Concorrente */
/* Nome: Gustavo Vilares Mariz de Oliveira */
/* Laboratório 5*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM_TEXTO 100 //tamanho máximo da linha no texto!
#define TAM_BUFFER 10 //numero de elementos no buffer

//variaveis globais
int slot_vazio = 0; //indica que os slots estão vazios quando = 0
int fim_arquivo = 0; //indicador de fim do arquivo
int nthreads = 0;//número de threads
sem_t slotCheio, slotVazio;  //semaforos para sincronizacao por condicao
sem_t mutexGeral; //semaforo geral de sincronizacao por exclusao mutua

char** Buffer; //buffer compartilhado

//inserir linha do texto no buffer
void Insere(char *linha) {

    static int in = 0;

    sem_wait(&slotVazio); //espera slot ficar vazio para inserir
    sem_wait(&mutexGeral); //exclusão mútua entre os produtores

    strcpy(Buffer[in], linha); //copia o texto da linha obtida para o buffer

    in = (in + 1) % TAM_BUFFER;
    printf("\nin: %d\n", in);

    printf("Produtor inseriu: %s\n", linha);
    slot_vazio++;

    sem_post(&mutexGeral); //fim da exclusão mútua
    sem_post(&slotCheio); //sinaliza slot preenchido
}

//retirar linha do buffer e printar
void Retira(int id) {
    char* linha;
    static int out = 0;

    sem_wait(&slotCheio); //aguarda slot cheio para retirar
    sem_wait(&mutexGeral); //exclusão mútua entre consumidores

    linha = Buffer[out];

    strcpy(linha, Buffer[out]); //copia o texto do buffer para a linha obtida
    //Buffer[out] = 0; //limpa o slot do buffer

    out = (out + 1) % TAM_BUFFER;
    printf("\nout: %d\n", out);
    printf("Consumidor[%d] retirou: %s\n", id, linha);
    slot_vazio--;

    sem_post(&mutexGeral);//fim da exclusão mútua
    sem_post(&slotVazio); //libera um slot do buffer
}

//função para as threads consumidoras
void *consumidor(void * arg) {
    int id = *(int *)(arg);
    free(arg);

    while(1) {
        Retira(id); //retira o proximo item
        sleep(1);
        if(fim_arquivo == 1 && slot_vazio == 0){ //para a execução se o arquivo chegou ao fim
            exit(1);
        }
    }

    pthread_exit(NULL);
}

//a main já é nossa thread produtora

//função auxiliar para leitura do arquivo e realizar a função do produtor
void le_arquivo(char* nome){
    FILE *file;

    //abre arquivo para leitura
    file = fopen(nome, "r");

    //armazena conteúdo da linha
    char linha[TAM_TEXTO];

    //garante existência do arquivo
    if(file != NULL) {
        //le as linhas e imprime
        while(fgets(linha, TAM_TEXTO, file)) {
            //printf("%s", myString);
            Insere(linha); //atividade do produtor
            if (feof(file)){
                fim_arquivo = 1; //identifica que o arquivo chegou ao fim
            }
        }

    //se o arquivo não abrir
    } else {
        printf("Não foi possível abrir o arquivo.");
    }

    //fecha o arquivo
    fclose(file);
}

int main(int argc, char *argv[]) {

    //confere se todos os argumentos foram passados e os converte para o formato desejado
    if(argc < 2) {
            printf("Falta de argumentos");
            return 1;
        }
    nthreads = atoi(argv[1]);
    char *arquivo = argv[2];

    // Aloca memória para o Buffer
    char **aux = NULL;
    aux = malloc(TAM_BUFFER * sizeof(char*));
    for(int i = 0; i < TAM_BUFFER; i++) {
        aux[i] = malloc((TAM_TEXTO) * sizeof(char));
    }
    Buffer = aux;

    //inicia as threads e seus ids
    pthread_t tid[nthreads];
    int *id;

    //inicializa os semáforos
    sem_init(&mutexGeral, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, TAM_BUFFER);

    //inicializa as threads consumidoras
    for(int i = 0; i < nthreads; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&tid[i], NULL, consumidor, (void *) (id));        
    }

    //função consumidora da main
    le_arquivo(arquivo);

    pthread_exit(NULL);

}
