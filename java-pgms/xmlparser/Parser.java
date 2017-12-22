
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Element;
import org.xml.sax.InputSource;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

import java.io.FileReader;
import java.io.File;
import java.io.BufferedReader;
import java.io.StringReader;

class Parser {

    public static void main(String args[]) {

        try {


            FileReader fr = new FileReader("./cacm.query.txt");

            BufferedReader br = new BufferedReader(fr);

            String contents = "";

            String line = null;

            while ((line = br.readLine()) != null) {

                contents += line + "\n";
            }

            contents = "<ROOT>" + contents + "</ROOT>";

            fr.close();

            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();

            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();

            InputSource is = new InputSource(new StringReader(contents));

            Document doc = dBuilder.parse(is);

            NodeList nlist = doc.getElementsByTagName("DOC");

            for (int i = 0; i < nlist.getLength(); i++) {

                Node n = nlist.item(i);

                printDocNode(n);
            }

        } catch (Exception e) {

            e.printStackTrace();
        }

    }

    private static void printNode(Node n) {

        System.out.println("Node " + n.getNodeValue());
    }

    private static void printDocNode(Node dn) {

        NodeList children = dn.getChildNodes();

        assert (children.getLength() == 3);

        printDocnumNode(children.item(1));

        System.out.println(children.item(0).getNodeValue() + children.item(2).getNodeValue());
    }

    private static void printDocnumNode(Node docno) {

        NodeList children = docno.getChildNodes();

        assert (children.getLength() == 1);

        System.out.println(Integer.parseInt(children.item(0).getNodeValue().trim()));
    }
}
