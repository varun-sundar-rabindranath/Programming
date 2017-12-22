
import java.util.HashSet;
import java.util.TreeSet;
import java.util.Iterator;

class HS {

    public static void main(String args[]) {

        TreeSet<String> ts = new TreeSet<String>();

        ts.add("L");
        ts.add("K");
        ts.add("J");
        ts.add("I");
        ts.add("H");
        ts.add("G");
        ts.add("F");
        ts.add("E");
        ts.add("D");
        ts.add("C");
        ts.add("B");
        ts.add("A");

        //System.out.println(ts.add("varun"));
        //System.out.println(ts.add("sundar"));
        //System.out.println(ts.add("rules"));
        //System.out.println(ts.add(":P"));
        //System.out.println(ts.add("varun"));
        //System.out.println(ts.add("sundar"));
        //System.out.println(ts.add("rules"));
        //System.out.println(ts.add(":P"));

        Iterator<String> it = ts.iterator();

        while (it.hasNext()) {
            System.out.println(it.next());
        }

    }

}
