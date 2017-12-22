
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

class ListTut {

  //private static final List<Integer> l = new ArrayList<Integer>();

  private static final List<Integer> l = new ArrayList<>(Arrays.asList(1, 2));

  ListTut() {
      System.out.println("Constructor");
  }

  public static void main(String args[]) {

    add_lists();

    for (int i = 0; i < 10; i++) {
      //l.add(i);
    }

    for (int e : l) {
      //  System.out.println(e);
    }
  }

  private static void add_lists() {

      List<Integer> l1 = new ArrayList<>(Arrays.asList(1, 2));
      List<Integer> l2 = new ArrayList<>(Arrays.asList(3, 4));
      List<Integer> l3 = new ArrayList<>(Arrays.asList(5, 6));

      l1.add(0, 0);
      l1.add(l1.size(), l1.size());

      for (int e : l1) {
          System.out.println(e);
      }

      //l1.addAll(l1);

      //for (int e : l1) {
      //    System.out.println(e);
      //}

      //System.out.println(l1.size());
      //System.out.println(l2.size());

      //Iterator<Integer> it1 = l1.iterator();
      //Iterator<Integer> it2 = l2.iterator();

      //while (it1.hasNext() && it2.hasNext()) {
      //    System.out.println(it1.next());
      //    System.out.println(it2.next());
      //}

      //for (int e1 : l1; int e2 : l2) {
      //    System.out.println(e1);
      //    System.out.println(e2);
      //}

      //String a_str = "A";
      //String b_str = "B";
      //System.out.println(a_str.compareTo(b_str));

  }

}
