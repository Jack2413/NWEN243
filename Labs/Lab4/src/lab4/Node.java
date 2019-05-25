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

import java.util.HashMap;
import java.util.Set;
import java.util.Vector;

public class Node {
	private String name; // the node name
	private int xPos, yPos; // the positions to draw the node
	private HashMap<String, Integer> neighbours; // the list of neighbours
	private HashMap<Node, Integer> RoutingTable;
	private boolean visited;
	private Node fromNode = null;

	// Think of what data structure to use to store the routing table
	// 2D array, vector, priorityQueue, etc.

	/**
	 * @param n:
	 *            the node name
	 * @param xp:
	 *            the x position
	 * @param yp:
	 *            the y position
	 */
	public Node(String n, int xp, int yp) {
		this.name = n;
		this.xPos = xp;
		this.yPos = yp;
		this.visited=false;
		this.neighbours = new HashMap<String, Integer>();
		this.RoutingTable = new HashMap<Node, Integer>();
	}

	/**
	 * Initialises the routing table
	 */
	public void initialise(Vector<Node> nodes) {
		for (Node node : nodes) {
			//System.out.println(node.name);
			if (node.name != this.name) {
				this.RoutingTable.put(node, Integer.MAX_VALUE);
			} else {
				this.RoutingTable.put(node, 0);
			}
			node.setVisited(false);
			node.fromNode=null;

		}

	}

	/**
	 * Updates the routing table
	 * I did change the parameter String to Node 
	 * 
	 */
	public void updateRoutingTable(Node fromNode, Node nodeData, int newCost) {
		RoutingTable.put(nodeData, newCost); // update the routing table hash map will auto override 
		nodeData.fromNode = fromNode; // set the parent 
	}

	public void simulateNewLink(String fromNode, String nodeData, int newCost) {
		if(this.name.equals(fromNode)) {// have to change in both way, from node to node data and node data to node
			neighbours.put(nodeData, newCost); 
		}else if(this.name.equals(nodeData)) {
			neighbours.put(fromNode, newCost);
		}
	}

	/**
	 * @param nodeName:the
	 *            neighbour node name
	 * @param cost:
	 *            the cost of the link
	 */
	public void addNeighbour(String nodeName, int cost) {
		this.neighbours.put(nodeName, cost);
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @param name
	 *            the name to set
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * @return the xPos
	 */
	public int getxPos() {
		return xPos;
	}

	/**
	 * @param xPos
	 *            the xPos to set
	 */
	public void setxPos(int xPos) {
		this.xPos = xPos;
	}

	/**
	 * @return the yPos
	 */
	public int getyPos() {
		return yPos;
	}

	/**
	 * @param yPos
	 *            the yPos to set
	 */
	public void setyPos(int yPos) {
		this.yPos = yPos;
	}

	/**
	 * @return the neighbours
	 */
	public HashMap<String, Integer> getNeighbours() {
		return neighbours;
	}

	public HashMap<Node, Integer> getRoutingTable() {
		return RoutingTable;
	}

	/**
	 * @param neighbours
	 *            the neighbours to set
	 */
	public void setNeighbours(HashMap<String, Integer> neighbours) {
		this.neighbours = neighbours;
	}

	public boolean isVisited() {
		return visited;
	}

	public void setVisited(boolean visited) {
		this.visited = visited;
	}

	public Node findMinCostNode() {
		Set<Node> keys = getRoutingTable().keySet();
		int min = Integer.MAX_VALUE;
		Node minNode = null;
		for (Node node : keys) {
			if (min > RoutingTable.get(node)&&!node.visited) {
				min = RoutingTable.get(node);
				minNode = node;
			}
		}
		return minNode;
	}

	public String RoutingTable() {
		String table = "";
		Set<Node> keys = getRoutingTable().keySet();
		for (Node key : keys) {
			//table+=(key.name+": ");
			if(!key.visited) {
			table+=(getRoutingTable().get(key)==Integer.MAX_VALUE?"(oo,":"("+getRoutingTable().get(key)+",");
			table+=(key.fromNode!=null?key.fromNode.name+") ":" )");
			}else {
				table+="      ";
			}
			//table+=key.findPath()+"\n";
		}
		return table;

	}

	public String findPath() {
		Set<Node> keys = getRoutingTable().keySet();
		String allpath ="";
		for (Node key : keys) {
		allpath+=key.name+": "+ getRoutingTable().get(key)+"        ";
		String path =key.name;
		Node current=key;
		while(current.fromNode!=null) {
			path+=("-"+current.fromNode.name);
			current=current.fromNode;
		}
		allpath+=reverse(path)+"\n";
		}
		return allpath;
	}

	private String reverse(String path) {
		String str="";
		for(int i =path.length()-1;i>=0;i--) {
			str+=path.charAt(i);
		}
		return str;
	}

	public String findPath(Node destination) {
		int destence = getRoutingTable().get(destination);
		String path =destination.name;
		Node current=destination;
		while(current.fromNode!=null) {
			path+=("-"+current.fromNode.name);
			current=current.fromNode;
		}
		String str = "The shortest destence is: "+destence + " path: " +reverse(path)+"\n";
		return str;
	}
}
