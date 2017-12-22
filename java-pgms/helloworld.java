public class helloworld {

  public static void main(String[] args) {

    System.out.println(args.length);

    int f = 0;

    for (int i = 0; i < args.length; i++) {
      System.out.println(args[i]);
    }

    System.out.println("hello world " + 3);

    assert (false) : "WOW";
  }

}
