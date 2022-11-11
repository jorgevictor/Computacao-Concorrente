import java.util.Arrays;

public class lab8Sequencial {

    public static void main (String[] args) {
        int[] vetor = new int[100];
        int par = 0;
        int impar = 0;

        for(int i = 0; i < vetor.length; i++){
            vetor[i] = i;
        }

        System.out.println(Arrays.toString(vetor));

        for (int i = 0; i < vetor. length; i++) {
            if (vetor[i] % 2 == 0) {
                par++;
            } else {
                impar++;
            }
        }

        System.out.println("Quantidade de pares: "+ par);
        System.out.println("Quantidade de ímpares: "+ impar);
    }
}
