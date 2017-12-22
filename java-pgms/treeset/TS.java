
import java.util.TreeSet;
import java.util.Iterator;
import java.util.*;

class Element implements Comparable<Element> {

    String e = "";

    Element(String e_) {
        e = e_;
    }

    public String get() {
        return e;
    }

    public int compareTo(Element element) {
        return this.e.compareTo(element.e);
    }
}

class TS {

    public static void main(String args[]) {

        TreeSet<Element> ts = new TreeSet<Element>();

        Iterator<Element> it;

        it = ts.iterator();

        while (it.hasNext()) {
            System.out.println(it.next().get());
        }


        ts.add(new Element("L"));
        ts.add(new Element("K"));
        ts.add(new Element("J"));
        ts.add(new Element("I"));
        ts.add(new Element("H"));
        ts.add(new Element("G"));
        ts.add(new Element("F"));
        ts.add(new Element("E"));
        ts.add(new Element("D"));
        ts.add(new Element("C"));
        ts.add(new Element("B"));
        ts.add(new Element("A"));

        it = ts.iterator();

        while (it.hasNext()) {
            System.out.println(it.next().get());
        }

    }
}
