
import java.util.List;
import java.util.Arrays;
import java.util.LinkedList;

import java.util.stream.*;

import static java.util.stream.Collectors.toList;

class HOFTut {

  public static void main(String args[]) {

    List<String> cities = Arrays.asList("London", "HongKong", "Paris", "NewYork");

    cities = cities.stream().map(String::toUpperCase).collect(toList());

    for (String city : cities) {
        System.out.println(city);
    }

  }

}
