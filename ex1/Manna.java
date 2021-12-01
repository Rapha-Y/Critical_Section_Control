import java.util.Random;

public class Manna implements Runnable {

  public static final int NT = 3;
  public static final int N = 5;
  static volatile int request = 0;
  static volatile int respond = 0;
  static volatile int SOMA = 0;
  static volatile int j = 0;
  static volatile boolean temp = true;
  public int id;

  public Manna(int id) {
    this.id = id;
  }
  
  public void run() {
    int i, n, local;
    
    if (Thread.currentThread().getName().equals("Servidor")) {
      while (temp) {
        while (request == 0 && temp) {
        }
        respond = request;
        while (respond != 0 && temp) {}
        request = 0;
      }
      System.out.println("SOMA = " + SOMA);
    } else {
      for (i = 0; i < N; i++) {
        while (respond != id) {
          request = id;
        }
        System.out.println(Thread.currentThread().getName() + " entrou na SC");
        local = SOMA;
        Random rand = new Random();
        n = rand.nextInt(10) % 2;
        try {
          Thread.currentThread().sleep(1000*n);
        } catch (Exception ex) {
          System.out.println("Exception has " + "been caught" + ex);
        }
        SOMA = local + 1;
        if (i == (N - 1)) {
          j++;
          if (j == (NT - 1)) temp = false;
        }
        System.out.println(Thread.currentThread().getName() + " saiu da SC");
        respond = 0; 
      }
    }
  
  }

  public static void main(String[] args) {
    int i;
    String nome;
    Manna[] mannas = new Manna[NT];
    Thread[] threads = new Thread[NT];
    
    for (i = 0; i < NT; i++) {
      if (i == 0) nome = "Servidor";
      else nome = "Cliente " + ((char)(64+i));
      mannas[i] = new Manna(i);
      threads[i] = new Thread(mannas[i],nome);
      threads[i].start();
    }
    
    for(i = 0; i < NT; i++) {
      try{
        threads[i].join();
      } catch(Exception ex) {
        System.out.println("Exception has " + "been caught" + ex);
      }
    }
  
  }

}
