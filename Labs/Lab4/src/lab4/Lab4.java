/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab4;

/**
 *
 * @author aliahmed
 */
import ecs100.*;
import java.awt.Color;
import java.util.Scanner;
import java.io.*;
import java.util.Vector;
import java.util.Set;

public class Lab4 {
	Vector<Node> nodes = new Vector<Node>();

	public Lab4() {
		UI.addButton("load map", this::load);
		UI.addButton("Draw Nodes", this::draw);

		UI.addButton("Start", this::start);
		UI.addButton("Simulate", this::Simulate);
		UI.addButton("Chose", this::Chose);

	}
	
	public void Chose(){
		UI.printf("\n\n====================================================================\n");
		long startTime = System.currentTimeMillis();
		Node n = getNode(UI.askString("Enter source"));
		Node destination = getNode(UI.askString("Enter destination"));
		
			n.initialise(nodes);	//initialise all the nodes to oo and set them to not visited 
			UI.printf("\nDijkstra’s algorithm: " + n.getName() + "\nIn the bracket the format shows as (distence,from node)\n         ");
			printallnode(n);
			int count = 0;
			while (true) {// to check is any node does not visited
				
				Node minNode = n.findMinCostNode(); // to find the min Node
				int costSoFar = n.getRoutingTable().get(minNode); // the cost for the current node (n) to the minimum Node
				for (String key : minNode.getNeighbours().keySet()) { //for all the key(NodeNames) form minimum node's neighbors
					if (costSoFar + minNode.getNeighbours().get(key) < n.getRoutingTable().get(getNode(key))) {// to check that the costSofar+the link to neighbors is smaller the cost from routing table
						n.updateRoutingTable(minNode, getNode(key), costSoFar + minNode.getNeighbours().get(key));//then update the new path(new shortest cost) for routing table current node n
					}
				}
				minNode.setVisited(true);// set the minimum node to Visited
				if(minNode.getName().equals(destination.getName())) {
					break;
				}
				UI.printf("step: %d "+n.RoutingTable()+"\n",count++);
				
			}
			UI.println("From: "+n.getName()+" To: "+destination.getName());
			UI.println(n.findPath(destination));
			UI.printf("\n\n====================================================================");

		
		long endTime = System.currentTimeMillis();
		long totalTime = endTime - startTime;
		UI.printf("\nThe time used: %dms\n", totalTime);
	}

	public void Simulate() {
		
		//do {
		String fromNode = UI.askString("Enter fromNode: ");
		String NodeDate = UI.askString("\nEnter NodeDate: ");
		int newCost = UI.askInt("\nEnter newCost: ");
		//}while(cheakvaild);
		for (Node n : this.nodes) {
			n.simulateNewLink(fromNode, NodeDate, newCost);//to simulate the new link
		}
		draw();
		start();
	}

	public void start() {
		UI.printf("\n\n====================================================================\n");
		long startTime = System.currentTimeMillis();
		for (Node n : this.nodes) { // for every node in nodes
			n.initialise(nodes);	//initialise all the nodes to oo and set them to not visited 
			UI.printf("\nDijkstra’s algorithm: " + n.getName() + "\nIn the bracket the format shows as (distence,from node)\n         ");
			printallnode(n);
			int count =0;
			while (inRounting()) {// to check is any node does not visited
				
				Node minNode = n.findMinCostNode(); // to find the min Node
				int costSoFar = n.getRoutingTable().get(minNode); // the cost for the current node (n) to the minimum Node
				for (String key : minNode.getNeighbours().keySet()) { //for all the key(NodeNames) form minimum node's neighbors
					if (costSoFar + minNode.getNeighbours().get(key) < n.getRoutingTable().get(getNode(key))) {// to check that the costSofar+the link to neighbors is smaller the cost from routing table
						n.updateRoutingTable(minNode, getNode(key), costSoFar + minNode.getNeighbours().get(key));//then update the new path(new shortest cost) for routing table current node n
					}
				}
				minNode.setVisited(true);// set the minimum node to Visited
				UI.printf("step: %d "+n.RoutingTable()+"\n",count++);
				
			}
			//UI.println();
			//UI.printf("\nThe shortest distance from " + n.getName() + " to: others show at last line of the table\n");
			UI.printf("\nRounding Table: "+n.getName());
			UI.printf("\nThe shortest path and distance from " + n.getName() + " to others are shows as below:\n");
			UI.printf("To Distance Path\n");
			UI.printf(n.findPath());
			UI.printf("\n\n====================================================================");

		}
		long endTime = System.currentTimeMillis();
		long totalTime = endTime - startTime;
		UI.printf("\nThe time used: %dms\n", totalTime);
	}

	private void printallnode(Node node) {
		for (Node n : node.getRoutingTable().keySet()) {
			UI.printf("%s     ", n.getName());
		}
		UI.println();	
	}

	private Node getNode(String nodeName) {

		for (int i = 0; i < this.nodes.size(); i++)
			if (this.nodes.get(i).getName().equals(nodeName)) {
				return this.nodes.get(i);
			}
		return null;
	}

	private boolean inRounting() {
		for (Node n : this.nodes) {
			if (!n.isVisited()) {
				return true;
			}
		}
		return false;
	}

	public void load() {
		try {
			Scanner scan = new Scanner(new File(UIFileChooser.open("Select Map File")));
			while (scan.hasNext()) {
				String n = scan.next();
				int x = scan.nextInt();
				int y = scan.nextInt();
				Node node = new Node(n, x, y);
				int count = scan.nextInt(); // the number of neighbouring nodes
				for (int i = 0; i < count; i++)
					node.addNeighbour(scan.next(), scan.nextInt());

				this.nodes.add(node);
				// node.initialise(nodes);
			}
			scan.close();
		} catch (IOException e) {
			UI.println("File Failure: " + e);
		}
	}

	public void draw() {
		UI.clearGraphics();
		for (Node n : this.nodes) {
			UI.setColor(Color.green);
			UI.fillOval(n.getxPos(), n.getyPos(), 40, 40);
			UI.setColor(Color.blue);
			UI.drawString(n.getName(), n.getxPos() + 5, n.getyPos() + 22);

			UI.setColor(Color.red);

			// loop on all neighbours
			Set<String> keys = n.getNeighbours().keySet();
			for (String s : keys) {
				// Search in the list of nodes for this node wth name "s"
				Node neighbour = null;
				for (int i = 0; i < this.nodes.size(); i++)
					if (this.nodes.get(i).getName().equals(s)) {
						neighbour = this.nodes.get(i);
						break;
					}

				if (neighbour != null) // there is a neighbour
				{
					UI.setColor(Color.RED);
					UI.drawLine(n.getxPos() + 20, n.getyPos() + 20, neighbour.getxPos() + 20, neighbour.getyPos() + 20);
					double x = (n.getxPos() + 20 + neighbour.getxPos() + 20) / 2;
					double y = (n.getyPos() + 20 + neighbour.getyPos() + 20) / 2;
					UI.setColor(Color.BLUE);
					UI.drawString(n.getNeighbours().get(neighbour.getName()).toString(), x, y);

				}

			}
		}
		//UI.clearGraphics();
	}

	/**
	 * @param args
	 *            the command line arguments
	 */
	public static void main(String[] args) {
		// TODO code application logic here
		new Lab4();
	}

}
