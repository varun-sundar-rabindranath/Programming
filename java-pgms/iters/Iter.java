
import java.util.*;

interface IterInterface extends Iterable<String> {
}

public class Iter implements IterInterface {


    String[] strs;
    int len;
    int last;

    // Constructor
    Iter(int len_) {
        this.len = len_;
        strs     = new String[len];
        last     = -1;
    }

    public void add(String s) {

        if (last == len - 1) {
            // dont add
            return;
        }

        strs[last + 1] = s;
        last           = last + 1;
    }

    public Iterator<String> iterator() {
        return new StrIter();
    }


    // Iterator inner class
    private class StrIter implements Iterator<String> {

        // Cursor. Present index
        int cursor;

        // constructor
        StrIter() {
            this.cursor = 0;
        }

        public boolean hasNext() {
            return cursor < last;
        }

        public String next() {
            String rs = Iter.this.strs[cursor];
            cursor++;
            return rs;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }
    }

    public static void main(String args[]) {

        Iter it = new Iter(10);

        it.add("0");
        it.add("1");
        it.add("2");
        it.add("3");
        it.add("4");
        it.add("5");
        it.add("6");
        it.add("7");
        it.add("8");
        it.add("9");
        it.add("10");
        it.add("11");
        it.add("12");
        it.add("13");
        it.add("14");

        for (String i : it) {
            System.out.println(i);
        }
    }

}
