import java.util.*;

class Override implements Comparable<Override> {


    // Dummy variables
    int dummy;

    // Default constructor
    Override(int dummy_) {
        this.dummy = dummy_;
    }

    public int dummyVal() {
        return dummy;
    }

    public int hashCode() {
        return Integer.hashCode(this.dummy);
    }

    public String toString() {
        return String.valueOf(this.dummy);
    }

    public boolean equals(Override otherO) {
        return (this.dummy == otherO.dummy);
    }

    public int compareTo(Override otherO) {
        return this.dummy - otherO.dummy;
    }

    public static void main(String[] args) {
        Override o = new Override(12345);
        System.out.println("toString - " + o);
        System.out.println("hashCode - " + o.hashCode());

        Override otherO = new Override(54321);
        System.out.println("equals same objects " + o.equals(o));
        System.out.println("equals diff objects " + o.equals(otherO));

        Override otherOWithSameDummy = new Override(12345);
        System.out.println("equals diff objects same dummy " + o.equals(otherOWithSameDummy));

        Override o1 = new Override(1);
        Override o2 = new Override(2);
        Override o3 = new Override(3);
        Override o4 = new Override(4);
        Override o5 = new Override(5);

        List<Override> ol = new ArrayList<Override>();
        ol.add(o1);
        ol.add(o2);
        ol.add(o3);
        ol.add(o4);
        ol.add(o5);

        Collections.sort(ol);

        for (Override or : ol) {
            System.out.println(or);
        }

    }
}
