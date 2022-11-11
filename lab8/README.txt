1.3) Sim, isso ocorre, pois, dado que, as threads recebem o start(), todas elas estão
aptas a executar, sem muito controle de quem vai executar primeiro.

1.5) A thread main não termina mais antes do término das demais threads.

3.1)    //operacao de escrita sobre o recurso compartilhado
      public void inc() {
         this.r++;
      }
      //operacao de leitura sobre o recurso compartilhado
      public int get() {
         return this.r;
      }

200000.

3.2) Não, pois a variável r perde incrementos em função da condição de corrida no programa.

4.3) Sim, pois, com essa sincronização, o problema de condição de corrida é corrigido.
