
import java.util.Arrays;
import java.util.function.*;
import java.util.List;
import java.util.ArrayList;

class fnargs {

  //private static final List<Integer> l = new ArrayList<Integer>();

  private static final List<Integer> l = new ArrayList<>(Arrays.asList(1, 2));


  public static void main(String args[]) {
      fnpass(fnargs::print_fn);
      fnpass_apply(fnargs::lower);
  }

  private static void print_fn(String s) {
      System.out.println(s);
  }

  private static String lower(String s) {
      return s.toLowerCase();
  }

  private static void fnpass(Consumer<String> fn) {
      fn.accept("This is crazy !!");
  }

  private static void fnpass_apply(Function<String, String> fn) {
      System.out.println(fn.apply("This is crazy !!"));
  }
}
