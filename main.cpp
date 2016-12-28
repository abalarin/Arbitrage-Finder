#include <iostream>
#include <tuple>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h> 
#include "Currency.h"

using namespace std;


void findLargestCycle();
void initGraphs(vector<Currency>&, vector<Currency>&);
void Bellman_Ford(vector<Currency>, int, vector<Currency>);
void printGraph(vector<Currency>);

int Num_Currencies;
vector<vector<Currency>> allGraphs; //List of all graphs with a cycle containing an Arbitrage
vector<Currency> Currency_Table;

int main() {
	//Empty Directed Graph used for storing a graph with at least 1 cycle
		// Cycle in Graph == A path of an Arbitrage
	vector<Currency> diGraph;

	//Init both complete graph and empty diGraph from an input file
	initGraphs(Currency_Table, diGraph);
	
	Num_Currencies = Currency_Table.size();// Init Number of currencies to however many where given in input file

	// Run Algorithim once for each Vertex in Graph
	for (int i = 0; i < Num_Currencies; i++)
		Bellman_Ford(Currency_Table, i, diGraph);
	
	//Prints out all Cycles in Adj List that may contain Arbitrage
		//Also prints out every Adj List (May want to comment out line 101)
	findLargestCycle();
}

void DFS(int s, vector<bool> &visited, vector<Currency> AdjList, vector<int>& cycle) {
	//Init list for path
	vector<int> PATH;

	// Create a stack for DFS
	stack<int> Stack;

	// Push the current source node.
	Stack.push(s);
	while (!Stack.empty()){
		// Pop a vertex from stack and print it
		s = Stack.top();
		Stack.pop();

		// Stack may contain same vertex twice, so check if visited
		if (!visited[s]) {
			PATH.push_back(s + 1);
			visited[s] = true;
		}

		// Get all adjacent vertices of the popped vertex s
			// If a adjacent has not been visited, then push to stack.
		vector<Currency::Edge> edges = AdjList[s].getEdges();
		for (unsigned int i = 0; i < edges.size(); i++) 
			if (!visited[edges[i].I - 1]) 
				Stack.push(edges[i].I - 1);
	}

	for (unsigned int i = 0; i < PATH.size(); i++) {
		if (AdjList[PATH[i] - 1].getEdges().size() != 0)
			cycle.push_back(PATH[i]);
	}
	
}

vector<int> getCycle(vector<Currency> AdjList) {
	//Init a cycle to be filled with an Arbitrage Path
	vector<int> Cycle;

	// Initially mark all verices as not visited
	vector<bool> visited(AdjList.size(), false);

	for (unsigned int i = 0; i < AdjList.size(); i++)
		if (!visited[i])
			DFS(i, visited, AdjList, Cycle);// Fills Cycle

	return Cycle;
}

//Run some search that returns a sum of cycle > 1
// Cycle = path were vertex[i].R * vertex[i+1].R * vertext[i].R > 1
pair<string, double> getAllCycles(vector<Currency> graph) {
	
	//Destroy Node0 -not needed anymore-
	graph.erase(graph.begin());
	
	//Print Adj List
	printGraph(graph); ///~~~~~~~~~~~~~Comment out if running for more than 10 currencies~~~~~~~~~~~~~
	
	
	//Find a cycle in Directed Graph 'graph'
	vector<int> cycle = getCycle(graph);
	
	//init cyc & profit
	string graphCycle;
	double profit = 1.0;
	
	//If cycle contains more than one Edge -continue
	if (cycle.size() > 1) {
		// Add start of path to end of path to make a proper cycle
		cycle.push_back(cycle[0]);

		// Iterate through entire cycle except for new last connection
		for (unsigned int i = 0; i < cycle.size()-1; i++) {
			// Append current Vertex name to string for printing
			graphCycle += Currency_Table[cycle[i]].getCountryName()+ "->";

			// Search Currency_Table until finds child Verticy
			vector<Currency::Edge> edges = Currency_Table[cycle[i]].getEdges();
			for (unsigned int j = 0; j < edges.size(); j++)
				if (edges[j].I == cycle[i + 1]) {
					profit *= edges[j].R; // update profit with Conv Rate of edge
					break; // Break out of this loop to save some time
				}
		}
		graphCycle += Currency_Table[cycle[0]].getCountryName();
	}

	return	{ graphCycle , profit};
}

void findLargestCycle() {
	vector<pair<string, double>> possibleAribtrages;
	pair<string, double> largestArbitrage("",0);

	for (unsigned int k = 0; k < allGraphs.size(); k++)
	{
		// Get a di-graph and find its cycle
		vector<Currency>directedGraph = allGraphs[k];
		pair<string, double> newCycle = getAllCycles(directedGraph);

		// Push Cycle to list of all Cycles
		possibleAribtrages.push_back(newCycle);

		// Update best Arbitrage if exsist
		if (newCycle.second > largestArbitrage.second)
			largestArbitrage = newCycle;
		
	}

	//Simple Print for all possible Arbitrage Cycles & Largest
	cout << endl << "Possible Aribtrages: " << endl;
	for (unsigned int i = 0; i < possibleAribtrages.size()-1; i++){
		cout << possibleAribtrages[i].first << " = " << possibleAribtrages[i].second << endl;
	}
	cout << endl << "Largest Arbitrage: " << endl << largestArbitrage.first << " = " << largestArbitrage.second << endl;
}

void initGraphs(vector<Currency>& graph, vector<Currency>& diGraph) {
	string line;
	ifstream file("testData25.txt");

	// Check if File opens
	if (file.is_open()) {
		// Create empty Currency object & Vertex Name
		Currency newCurrency;
		string country;

		//Parse through file and insert into graph accordingly
		//For initlizing intiger postion of vertex with respect to Adjacency List
		int i = 1, j=1; //We skip 0 because Node0 is reserved for that
		while (getline(file, line)) {

			//If current line contains country Vertex Name -> this Country
			if (line.find("Country") == 0) {
				i = 1;
				country = line.substr(8, 10); // Finds Country Code
				newCurrency.setCountryName(country);
				newCurrency.setNodeWeight(0.0);
				newCurrency.setPosition(j);
				//All Verticies will have a connection to Vertex '0'
				newCurrency.insertEdge("0", 0, 0.0, 0.0, 0.0);
				i++; j++;//For initlizing intiger postion of vertex with respect to Adjacency List

			}
			//If line contains colon insert edge with properties into parent Vertex
			else if (line.find(':') == 3) {
				string convRate = line.substr(4);
				string code = line.substr(0, 3);
				double rate = atof(convRate.c_str());

				//For initlizing intiger postion of vertex with respect to Adjacency List
				int index = i;
				for (unsigned int k = 0; k < diGraph.size(); k++)
					if (diGraph[k].getCountryName() == code)
						index = i-1;

				//Insert a new edge into current Currency Object
				newCurrency.insertEdge(code, index, 0.0, rate, -log10(rate));
				i++;
			}
			// Else - file contains no more edges for parent Vertex
				// Insert current Currency object into Graph 
					//& empty Currency object into the Directed Graph
			else {
				graph.push_back(newCurrency);

				//Push an edge into diGraph with no connecting (an empty graph)
				diGraph.push_back({newCurrency.getCountryName(), j-1, 0.0, 0.0, 0.0});

				// Delete & Recreate 'Currency' object and reset 'country'
				country = "";
				(&newCurrency)->~Currency();
				new (&newCurrency) Currency();
			}

		}
	}
	else {//File couldnt open
		cout << "Error: File could not open." << endl;
		return;
	}
	file.close();

	// Necesary to have a node0 with connection to all other nodes 
		// But all edge weghts := 0
	Currency Node0("0", 0, 0.0, 0.0, 0.0);
	for (unsigned int i = 0; i < graph.size(); i++)
		Node0.insertEdge(graph[i].getCountryName(), graph[i].getPosition(), 0.0, 0.0, 0.0);
	graph.insert(graph.begin(), Node0);
	diGraph.insert(diGraph.begin(), Node0);

	/*//----------Print statement for all Edge properties for each Vertex----------
	for (unsigned int i = 0; i < graph.size(); i++) {
		vector<Currency::Edge> Edges = graph[i].getEdges();
		//Node0.insertEdge(graph[i].getCountryName(), 0.0, 0.0, 0.0);
		cout << "Node'"<< graph[i].getPosition() <<"': " << graph[i].getCountryName() << endl;
		for (unsigned int j = 0; j < Edges.size(); j++) {
			cout << "Edge To: " << Edges[j].I << Edges[j].C <<
				", Edge-Weight:" << Edges[j].W <<
				", C-Rate:" << Edges[j].R <<
				", Node Weight: " << Edges[j].V << endl;
		}
		cout << endl;
	}*///---------------------------------------------------------------------------

}

void insertToDiGraph(string countryCode, Currency::Edge edge, vector<Currency>& diGraph) {
	for (int i = 0; i < Num_Currencies; i++){
		if (diGraph[i].getCountryName() == countryCode) {
			diGraph[i].insertEdge(edge);
		}		
	}
}

void Bellman_Ford(vector<Currency> graph, int source, vector<Currency> diGraph) {
	
	// Step1:Init an empty Array containing minimum distances of all Vertexes
	vector<double> distance;
	for (int i = 0; i < Num_Currencies; i++)
		distance.push_back(INFINITY);
	distance[source] = 0;

	// Step2: Relax all edges in Undirected Graph
	for (int i = 0; i < Num_Currencies; i++)
	{
		int k = 0;
		for (int j = 0; j < Num_Currencies; j++)
		{
			// There are 1 less edges in Adjacency List than in 'distance' Array
				// As long as position on Adj List is !same as 'dist' Array -continue
			if (j != i) {				
				string Node = graph[i].getCountryName();	// Current Node Name
				string edgeNode = graph[i].getEdges()[k].C; // End of Node's edge(Sibling Vertex)
				double r = graph[i].getEdges()[k].R;		// Exchange Rate from Node->edgeNode
				double v = graph[i].getEdges()[k].V;		// End of edge Node's Weight
				double weight = graph[i].getEdges()[k].W;	// Edge weight

				// Checker to see if graph contains a possible negative weight cycle
				if (distance[i] + weight < distance[j] && j != 0) {

					// If possible cycle exsist insert current edge into a digraph
					insertToDiGraph(Node, {edgeNode, j, v, r, weight}, diGraph);

					// Update distance array with smallest path height
					distance[j] = distance[i] + weight;
					graph[i].setNodeWeight(distance[j]);
				}
				k++;
			}
			
		}

	}
	// Add current Adjacency List containg an Arbitrage to list of all Graphs
	allGraphs.push_back(diGraph);
}

//--------Print out an Adjacency List------------
void printGraph(vector<Currency> graph) {
	cout << "Graph: " << endl;
	for (unsigned int i = 0; i < graph.size(); i++) {
		cout << "  " << graph[i].getCountryName() << "->";
		vector<Currency::Edge> edges = graph[i].getEdges();
		for (unsigned int j = 0; j < edges.size(); j++)
			cout << edges[j].C << " ";
		cout << endl;
	}
	cout << endl;
}