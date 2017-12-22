

import java.io.File;

class FileTut {

    public static void main(String[] args) {

        if (args.length == 0) {
            // Program expects atleast one argument
            System.out.println("Program expects atleast one argument");
            return;
        }

        // Get path to the input directory
        String dp = args[0];

        File dir = new File(dp);

        File[] files = dir.listFiles();

        for (File f : files) {
            System.out.println(f);
        }
    }
}
