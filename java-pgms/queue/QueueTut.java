
import java.util.Queue;
import java.util.LinkedList;

class QueueTut {

  public static void main(String args[]) {

    Queue<Integer> q = new LinkedList<Integer>();

    for (int i = 0; i < 10; i++) {
      q.add(i);
    }

    while (!q.isEmpty()) {
      //System.out.println("Queue element popped - ", int(q.remove()));
      System.out.println(q.remove());
    }

  }

}
