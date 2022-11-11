import java.util.ArrayList;

//classe da estrutura de dados (recurso) compartilhado entre as threads
class SS {

    //recurso compartilhado
    private final ArrayList<Integer> vetor;
    private int x;

    //construtor
    public SS(ArrayList<Integer> V) {
        this.vetor = V;
        this.x = 0;
    }

    public int get(int i){
        return this.vetor.get(i);
    }

    public int tamanho(){
        return this.vetor.size();
    }

    public synchronized void par(){
        this.x++;
    }

    public int getPar(){
        return this.x;
    }

}

//classe que estende Thread e implementa a tarefa de cada thread do programa
class TT extends Thread {

    //identificador da thread
    private int id;

    //objeto compartilhado com outras threads
    SS s;

    //construtor
    public TT(int tid, SS s) {
        this.id = tid;
        this.s = s;
    }

    //metodo main da thread
    public void run(){
        for(int i = id; i < this.s.tamanho(); i += lab8Concorrente.N){
            if(((this.s.get(i))%2) == 0){
                s.par();
            }
        }
    }
}

public class lab8Concorrente {

    static final int N = 10;
    static final int TAM = 100000;

    public static void main (String[] args) {

        //--reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N];

        ArrayList<Integer> x = new ArrayList<Integer>();

        for(int i = 0; i < TAM; i++){
            x.add(i);
        }

        //cria uma instancia do recurso compartilhado entre as threads
        SS s = new SS(x);

        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
            threads[i] = new TT(i, s);
        }

        //inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        System.out.println("Quantidade de pares: "+ s.getPar());
    }
}
