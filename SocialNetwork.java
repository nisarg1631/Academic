import java.awt.geom.Point2D;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Vector;

public class SocialNetwork {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        HashMap<Integer, Node> nodes = new HashMap<>();

        int op = 0;
        while(op!=9) {
            System.out.print("\nSELECT OPTION FROM LIST BELOW:\n\n" +
                    "1> Print all nodes.\n" +
                    "2> Create a new node.\n" +
                    "3> Delete a node.\n" +
                    "4> Search for a node using name, type or birthday.\n" +
                    "5> Print all linked nodes to a given node.\n" +
                    "6> Create and post content by a user.\n" +
                    "7> Search for content posted by a node.\n" +
                    "8> Display all content posted by nodes linked to a given node.\n" +
                    "9> Exit\n\n" +
                    "OPTION NUMBER: "); op = input.nextInt();
                    switch(op) {
                        case 1:
                            printNodes(nodes);
                            break;
                        case 2:
                            addNode(input, nodes);
                            break;
                        case 3:
                            deleteNode(input, nodes);
                            break;
                        case 4:
                            searchNode(input, nodes);
                            break;
                        case 5:
                            printLinks(input, nodes);
                            break;
                        case 6:
                            addContent(input, nodes);
                            break;
                        case 7:
                            displayContent(input, nodes);
                            break;
                        case 8:
                            displayContentOfLinks(input, nodes);
                            break;
                        case 9:
                            System.out.println("\n\tBye!");
                            break;
                        default:
                            System.out.println("\n\tEnter a valid option number.");
                    }
        }
    }

    static void addNode(Scanner input, HashMap<Integer, Node> nodes) {
        int id;
        System.out.print("\nEnter ID: "); id = input.nextInt();
        if (nodes.containsKey(id)) {
            System.out.println("\n\tCOULDN'T ADD NODE. ID already exists. Please try again with another ID.");
            return;
        }
        int type;
        System.out.print("Enter type of node [ 1:individual, 2:Group, 3:Business, 4:Organization ]: "); type = input.nextInt();
        switch (type) {
            case 1:
                nodes.put(id, new Individual(input, id));
                break;
            case 2:
                nodes.put(id, new Group(input, id));
                break;
            case 3:
                nodes.put(id, new Business(input, id));
                break;
            case 4:
                nodes.put(id, new Organization(input, id));
                break;
            default:
                System.out.println("\n\tCOULDN'T ADD NODE. Invalid node type. Should be integer in range [1, 4]");
                return;
        }
        linkNodes(input, id, nodes);
        System.out.println("\n\tNODE SUCCESSFULLY ADDED.");
    }

    static void printNodes(HashMap<Integer, Node> nodes){
        System.out.println();
        for(Node n : nodes.values()){
            n.printNode();
            System.out.println();
        }
    }

    static void linkNodes(Scanner input, int id, HashMap<Integer, Node> nodes){
        System.out.println("FACILITY TO LINK NODES");
        int linkTo;
        while(true){
            System.out.print("Enter ID of node to link to [ -1 to finish linking ]: "); linkTo = input.nextInt();
            if(linkTo==-1)
                break;
            if(nodes.containsKey(linkTo)){
                nodes.get(id).linkNode(input, nodes.get(linkTo));
            }
            else{
                System.out.println("\tCOULDN'T LINK TO NODE. No node with ID " + linkTo + " exists.");
            }
        }
    }

    static void deleteNode(Scanner input, HashMap<Integer, Node> nodes){
        int id;
        System.out.print("\nEnter ID: "); id = input.nextInt();
        if(!nodes.containsKey(id)) {
            System.out.println("\n\tCOULDN'T DELETE NODE. No node with ID " + id + " exists.");
            return;
        }
        Node toDelete = nodes.get(id);
        for(Link l : toDelete.links.values().toArray(new Link[0]))
            l.linkTo.destroyLink(toDelete);
        nodes.remove(id);
        System.out.println("\n\tNODE SUCCESSFULLY DELETED.");
    }

    static void searchNode(Scanner input, HashMap<Integer, Node> nodes){
        int op;
        System.out.print("\nSEARCH USING:\n" +
                "1> Name.\n" +
                "2> Type.\n" +
                "3> Birthday (for individuals).\n" +
                "OPTION NUMBER [1, 3]: "); op = input.nextInt();
                System.out.println();

                if(op==1){
                    String name;
                    input.nextLine();
                    System.out.print("Enter name to search (prefix search allowed): "); name=input.nextLine();
                    name = name.toLowerCase();
                    System.out.println("\n\tSEARCH RESULTS\n");
                    for(Node n : nodes.values()){
                        if(n.name.toLowerCase().startsWith(name)) {
                            n.printNode();
                            System.out.println();
                        }
                    }
                }
                else if(op==2){
                    int subop;
                    System.out.print("SELECT TYPE:\n" +
                            "1> Individual.\n" +
                            "2> Group.\n" +
                            "3> Business.\n" +
                            "4> Organization.\n" +
                            "OPTION NUMBER [1, 4]: "); subop = input.nextInt();
                            System.out.println();

                            String type;
                            if(subop==1)
                                type="individual";
                            else if(subop==2)
                                type="group";
                            else if(subop==3)
                                type="business";
                            else
                                type="organization";

                            System.out.println("\tSEARCH RESULTS\n");
                            for(Node n : nodes.values()){
                                if(n.getType().equals(type)) {
                                    n.printNode();
                                    System.out.println();
                                }
                            }
                }
                else{
                    String birthday;
                    input.nextLine();
                    System.out.print("Enter birthday (DD/MM/YYYY) (prefix search allowed): "); birthday=input.nextLine();
                    birthday = birthday.toLowerCase();
                    System.out.println("\n\tSEARCH RESULTS\n");
                    for(Node n : nodes.values()){
                        if(n.getType().equals("individual") && n.getBirthday().toLowerCase().startsWith(birthday)){
                            n.printNode();
                            System.out.println();
                        }
                    }
                }
    }

    static int inputID(Scanner input, HashMap<Integer, Node> nodes){
        int id;
        System.out.print("\nEnter ID: "); id = input.nextInt();
        if(!nodes.containsKey(id))
            System.out.println("\n\tNo node with ID " + id + " exists.");
        return id;
    }

    static void printLinks(Scanner input, HashMap<Integer, Node> nodes){
        int id = inputID(input, nodes);
        if(nodes.get(id)!=null) {
            System.out.println();
            nodes.get(id).printLinks();
        }
    }

    static void addContent(Scanner input, HashMap<Integer, Node> nodes){
        int id = inputID(input, nodes);
        if(nodes.get(id)!=null) {
            int op;
            System.out.print("\nSELECT OPTION:\n" +
                    "1> Create new content.\n" +
                    "2> Repost existing content.\n" +
                    "OPTION NUMBER [1, 2]: "); op = input.nextInt();
            if(op==1)
                nodes.get(id).createContent(input);
            else
                nodes.get(id).repostContent(input);
        }
    }

    static void displayContent(Scanner input, HashMap<Integer, Node> nodes){
        int id = inputID(input, nodes);
        if(nodes.get(id)!=null) {
            System.out.println();
            nodes.get(id).printPosts();
        }
    }

    static void displayContentOfLinks(Scanner input, HashMap<Integer, Node> nodes){
        int id = inputID(input, nodes);
        if(nodes.get(id)!=null) {
            System.out.println();
            nodes.get(id).printLinks();
            System.out.println();
            for(Link l : nodes.get(id).links.values())
                System.out.println("\tPOSTS OF ID:" + l.linkTo.id + "\t" + l.linkTo.posts);
        }
    }
}

class Link{
    Node linkTo;
    String relationship;

    public Link(Node linkTo, String relationship){
        this.linkTo=linkTo;
        this.relationship=relationship;
    }
}

class Node{
    int id;
    String name, date;
    HashMap<Integer, Link> links = new HashMap<>();
    // 'content' stores set of uploaded content, using hashmap for fast lookups
    HashMap<String, StringBuilder> content = new HashMap<>();
    // 'posts' stores references to the elements in 'content', this way same content can be reposted without repetition
    Vector<StringBuilder> posts = new Vector<>();

    public Node(Scanner input, int id){
        this.id=id;
        input.nextLine();
        System.out.print("Enter name: "); this.name=input.nextLine();
        System.out.print("Enter date of creation (DD/MM/YYYY): "); this.date = input.nextLine();
    }

    public void printNode(){
        System.out.println("\tID: " + this.id);
        System.out.println("\tNAME: " + this.name);
        System.out.println("\tTYPE: " + this.getType());
        System.out.println("\tDATE OF CREATION: " + this.date);
        this.printLinks();
        this.printPosts();
    }

    public void printLinks(){
        System.out.print("\tLINKS (ID linked to:Relationship)");
        System.out.print("\t[ ");
        int i=0;
        for(Link l : links.values()) {
            System.out.printf("%d(%s):%s ",l.linkTo.id,l.linkTo.getType(),l.relationship);
            i++;
            if(i<links.size())
                System.out.print("; ");
        }
        System.out.println("]");
    }

    public void linkNode(Scanner input, Node linkTo){}

    public void createLink(Node linkTo, String relationship){
        this.links.put(linkTo.id, new Link(linkTo, relationship));
        linkTo.links.put(this.id, new Link(this, relationship));
        System.out.printf("\tSUCCESSFULLY LINKED %s (ID:%d) to %s (ID:%d) as %s.\n", this.getType(), this.id, linkTo.getType(), linkTo.id, relationship);
    }

    public void destroyLink(Node linkTo){
        this.links.remove(linkTo.id);
        linkTo.links.remove(this.id);
    }

    public void createContent(Scanner input){
        input.nextLine();
        String newcontent;
        System.out.print("\nEnter content: "); newcontent = input.nextLine();
        System.out.println();
        if(content.containsKey(newcontent)){
            posts.add(content.get(newcontent));
            System.out.println("\tContent already present. Reposted it.");
        }
        else{
            content.put(newcontent, new StringBuilder(newcontent));
            posts.add(content.get(newcontent));
            System.out.println("\tNew content created and posted.");
        }
    }

    public void repostContent(Scanner input){
        System.out.println();
        if(content.isEmpty()){
            System.out.println("\tNo content to repost. Please create some content first.");
            return;
        }
        System.out.println("Select content to repost:");
        int op;
        StringBuilder[] posts = this.content.values().toArray(new StringBuilder[0]);
        for(int i=0; i<posts.length; i++)
            System.out.printf("%d> \"%s\"\n",i+1,posts[i]);
        System.out.printf("OPTION NUMBER [1, %d]: ",posts.length); op = input.nextInt();
        System.out.println();
        if(op<1 || op>posts.length){
            System.out.println("\tInvalid option selected. Please try again.");
            return;
        }
        this.posts.add(posts[op-1]);
        System.out.println("\tContent successfully posted.");
    }

    public void printPosts(){
        System.out.println("\tPOSTED CONTENT: " + this.posts);
    }

    public String getType(){
        return "none";
    }

    public String getBirthday(){
        return "none";
    }
}

class Individual extends Node{
    String birthday;

    public Individual(Scanner input, int id) {
        super(input, id);
        System.out.print("Enter birthdate (DD/MM/YYYY): "); this.birthday = input.nextLine();
    }

    public String getType(){
        return "individual";
    }

    public String getBirthday(){
        return birthday;
    }

    public void printNode(){
        super.printNode();
        System.out.println("\tBIRTHDATE: " + this.birthday);
    }

    public void linkNode(Scanner input, Node linkTo){
        if(linkTo.getType().equals("group") || linkTo.getType().equals("organization")){
            this.createLink(linkTo, "member");
        }
        else if(linkTo.getType().equals("business")){
            input.nextLine();
            System.out.printf("ID:%d is a business. To link as customer enter 'c'. To link as owner enter 'o'. Enter option: ", linkTo.id);
            String relationship;
            if(input.next().charAt(0)=='c')
                relationship = "customer";
            else
                relationship = "owner";
            this.createLink(linkTo, relationship);
        }
        else{
            System.out.println("\tCOULDN'T LINK TO NODE. Link not allowed between nodes of type " + this.getType() + " and " + linkTo.getType() + ".");
        }
    }
}

class Group extends Node{
    public Group(Scanner input, int id) {
        super(input, id);
    }

    public String getType(){
        return "group";
    }

    public void printNode(){
        super.printNode();
    }

    public void linkNode(Scanner input, Node linkTo){
        if(linkTo.getType().equals("individual") || linkTo.getType().equals("business")){
            linkTo.createLink(this, "member");
        }
        else{
            System.out.println("\tCOULDN'T LINK TO NODE. Link not allowed between nodes of type " + this.getType() + " and " + linkTo.getType() + ".");
        }
    }
}

class Business extends Node{
    Point2D.Double loc;

    public Business(Scanner input, int id) {
        super(input, id);
        double x, y;
        System.out.print("Enter location x-coordinate: "); x = input.nextDouble();
        System.out.print("Enter location y-coordinate: "); y = input.nextDouble();
        loc = new Point2D.Double(x, y);
    }

    public String getType(){
        return "business";
    }

    public void printNode(){
        super.printNode();
        System.out.println("\tLOCATION: " + this.loc);
    }

    public void linkNode(Scanner input, Node linkTo){
        if(linkTo.getType().equals("group")){
            this.createLink(linkTo, "member");
        }
        else if(linkTo.getType().equals("individual")){
            input.nextLine();
            System.out.printf("ID:%d is an individual. To link as customer enter 'c'. To link as owner enter 'o'. Enter option: ", linkTo.id);
            String relationship;
            if(input.next().charAt(0)=='c')
                relationship = "customer";
            else
                relationship = "owner";
            linkTo.createLink(this, relationship);
        }
        else{
            System.out.println("\tCOULDN'T LINK TO NODE. Link not allowed between nodes of type " + this.getType() + " and " + linkTo.getType() + ".");
        }
    }
}

class Organization extends Node{
    Point2D.Double loc;

    public Organization(Scanner input, int id) {
        super(input, id);
        double x, y;
        System.out.print("Enter location x-coordinate: "); x = input.nextDouble();
        System.out.print("Enter location y-coordinate: "); y = input.nextDouble();
        loc = new Point2D.Double(x, y);
    }

    public String getType(){
        return "organization";
    }

    public void printNode(){
        super.printNode();
        System.out.println("\tLOCATION: " + this.loc);
    }

    public void linkNode(Scanner input, Node linkTo){
        if(linkTo.getType().equals("individual")){
            linkTo.createLink(this, "member");
        }
        else{
            System.out.println("\tCOULDN'T LINK TO NODE. Link not allowed between nodes of type " + this.getType() + " and " + linkTo.getType() + ".");
        }
    }
}
