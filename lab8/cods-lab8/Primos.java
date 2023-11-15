/* Disciplina: Programacao Concorrente */
/* Nome: Gustavo Vilares Mariz de Oliveira */
/* DRE: 121073784 */
/* Laboratório: 8 */

import java.util.LinkedList;
import java.util.ArrayList;
import java.lang.Math;
import java.util.concurrent.atomic.*;

//-------------------------------------------------------------------------------
//Classe que define um pool de threads
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        }
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }

    public void shutdown() {
        synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }

    private class MyPoolThreads extends Thread {
        public void run() {
            Runnable r;
            while (true) {
                synchronized(queue) {
                    while (queue.isEmpty() && (!shutdown)) {
                        try { queue.wait(); }
                        catch (InterruptedException ignored){}
                    }
                    if (queue.isEmpty() && shutdown) return;
                    r = (Runnable) queue.removeFirst();
                }
                try { r.run(); }
                catch (RuntimeException e) {}
            }
        }
    }
}
//-------------------------------------------------------------------------------

//Cria uma classe para definir um contador local
class Contador{

    //recurso compartilhado
    private AtomicLong r;

    //construtor
    public Contador(long init) {
        this.r = new AtomicLong(init);
    }

    //operacao sobre o recurso compartilhado
    public long get() {
        return r.get();
    }

    //operacao sobre o recurso compartilhado
    public void inc(){
        this.r.getAndIncrement();
    }
}

//Classe para armazenar o método de criação dos contadores locais
class cLocais{
    public static ArrayList<Contador> geraLocais(int nthreads) {
        //Cria um array de contadores
        ArrayList<Contador> contadoresLocais = new ArrayList<Contador>();
        //Para cada thread adiciona um contador no array
        for (int i = 0; i < nthreads; i++) {
            Contador c = new Contador(0);
            contadoresLocais.add(c);
        }
        return contadoresLocais;
    }

}


//Implementa a interface Runnable para identificar primos
class IdentPrimos implements Runnable {
    int valor;
    Contador c;
    public IdentPrimos(int v, Contador c) {this.valor = v; this.c = c;}

    int ehPrimo(int n) {
        int i;
        if (n<=1) return 0;
        if (n==2) return 1;
        if (n%2==0) return 0;
        for (i=3; i<Math.sqrt(n)+1; i+=2){
            if(n%i==0) return 0;
        }
        return 1;
    }

    //--metodo executado pela thread
    public void run() {
        if(ehPrimo(valor) == 1){c.inc();}
    }
}

//Classe para definir os métodos para verificar o problema de forma sequencial
class Verifier{

    public static int ehPrimoV(int n) {
        int i;
        if (n<=1) return 0;
        if (n==2) return 1;
        if (n%2==0) return 0;
        for (i=3; i<Math.sqrt(n)+1; i+=2){
            if(n%i==0) return 0;
        }
        return 1;
    }
    public static long VerifPrimos(int n){
        int c = 0;
        for(int i = 1; i<= n; i++){
            if(ehPrimoV(i) == 1) c++;
        }
        return c;
    }
}

//Classe da aplicação (método main)
class Primos{

    //Definir o número de threads usadas aqui
    private static final int NTHREADS = 10;

    //Definir o N desejado para o teste em questão aqui
    private static final int NTOTAL = 10000;

    public static void main (String[] args) {

        //Cria o contador final
        long countGeral = 0;

        //Cria um array com os contadores locais a serem somados no fim da execução
        ArrayList<Contador> contadoresLocais = cLocais.geraLocais(NTHREADS);

        //cria o pool de threads
        FilaTarefas pool = new FilaTarefas(NTHREADS);

        //dispara a execução dos objetos runnable usando o pool de threads
        int j = 0;
        for (int i = 1; i <= NTOTAL; i++) {
            Runnable r = new IdentPrimos(i, contadoresLocais.get(j));
            pool.execute(r);
            //Avança o índice do array de contadores e retorna para o 0 quando chega ao fim
            j++;
            if(j == NTHREADS) j = 0;
        }

        //esperar pelo termino das threads
        pool.shutdown();

        //Soma os contadores locais
        for(int k = 0; k < NTHREADS; k++){
            countGeral += contadoresLocais.get(k).get();
        }

        //Print do resultado final
        System.out.println("Total de números primos encontrados com N = " + NTOTAL + " : " + countGeral);

        //Se quiser retirar o teste, apenas comentar esse trecho
        if(Verifier.VerifPrimos(NTOTAL) == countGeral){
        System.out.println("O método sequencial encontrou o mesmo resultado!");
        }
        else{System.out.println("Houve um erro!");}

        System.out.println("Terminou");
    }
}

