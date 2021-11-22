import java.util.concurrent.*;

public class TrafficController {
    private Semaphore sem = new Semaphore(1);


    public void enterLeft() {
        try{
            sem.acquire();
        } catch(Exception e){ System.err.println(e); }
    }
    public void enterRight() {
        try{
            sem.acquire();
        } catch(Exception e){ System.err.println(e); }
    }
    public void leaveLeft() {
        sem.release();
    }
    public void leaveRight() {
        sem.release();
    }

}