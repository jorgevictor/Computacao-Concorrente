class Buffer {

    static final int N = 10;
    private double[] buffer = new double[N];
    private int count = 0;

    Buffer() {
        for (int i = 0; i < N; i++) {
            buffer[i] = 0;
        }
    }

    private void imprimeBuffer() {
        for (int i = 0; i < N; i++) {
            System.out.print(buffer[i]);
        }
        System.out.println("");
    }

    public synchronized void insere(int id) {
        try {
            System.out.println("Produtor " + id + " quer inserir");
            while (count == N) {
                System.out.println("Produtor " + id + " bloqueado");
                this.wait();
            }
            for (int i = 0; i < N; i++) {
                if (buffer[i] == 0) {
                    buffer[i] = 1;
                    count++;
                    break;
                }
            }
            System.out.println("Produtor " + id + " inseriu");
            imprimeBuffer();
            notifyAll();
        } catch (InterruptedException e) {
        }
    }

    public synchronized void retira(int id) {
        try {
            System.out.println("Consumidor " + id + " quer retirar");
            while (count == 0) {
                System.out.println("Consumidor " + id + " bloqueado");
                this.wait();
            }
            for (int i = 0; i < N; i++) {
                if (buffer[i] == 1) {
                    count--;
                    buffer[i] = 0;
                    break;
                }
            }
            System.out.println("Consumidor " + id + " retirou");
            imprimeBuffer();
            notifyAll();
        } catch (InterruptedException e) {
            return;
        }
    }

}

//--------------------------------------------------------
class Consumidor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    Buffer buffer;//objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Consumidor (int id, int delayTime, Buffer m) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.buffer.retira(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
class Produtor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    Buffer buffer; //objeto monitor para coordenar a lógica de execução das threads

    // Construtor
    Produtor (int id, int delayTime, Buffer m) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.buffer.insere(this.id);
                sleep(this.delay); //atraso bobo...
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
// Classe principal
class ProdutorConsumidor {
    static final int Cons = 1;
    static final int Prod = 3;

    public static void main(String[] args) {
        Buffer buffer = new Buffer();
        Consumidor[] consumidor = new Consumidor[Cons];
        Produtor[] produtor = new Produtor[Prod];

        for (int i = 0; i < Cons; i++) {
            consumidor[i] = new Consumidor(i+1, (i+1)*500, buffer);
            consumidor[i].start();
        }
        for (int i = 0; i < Prod; i++) {
            produtor[i] = new Produtor(i+1, (i+1)*500, buffer);
            produtor[i].start();
        }
    }
}