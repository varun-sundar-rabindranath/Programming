#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <climits>

using namespace std;


void bfs(char graph[500][500], int root, vector<int>gateway_nodes, vector<int> &shortest_paths, vector<int> &parent_nodes) {

    queue<int> bfs_queue;
    char visited[500];
    /* Make all visited 0 except root */
    for(int i = 0; i < 500; i++) {
	visited[i] = 0;
    }

    /* Make all shortest paths to root 0 */
    for(int i = 0; i < 500; i++) {
	shortest_paths[i] = 0;
    }

    /* Make all parent nodes 0 */
    for(int i = 0; i < 500; i++) {
	parent_nodes[i] = 0;
    }

    bfs_queue.push(root);
    visited[root] = 1;

    while(!bfs_queue.empty()) {
	int parent = bfs_queue.front();
	bfs_queue.pop();

	for(int i = 0; i < 500; i++) {
	    if(graph[parent][i] && !visited[i]) {
		parent_nodes[i] = parent;
		shortest_paths[i] = shortest_paths[parent] + 1;
		bfs_queue.push(i);
		visited[i] = 1;
	    }
	}
    }
}
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{

    vector<int> gateway_nodes;
    char graph[500][500];
    /* Init graph to all zeros */
    for(int i = 0; i < 500; i++) {
	for(int j = 0; j < 500; j++) {
	    graph[i][j] = 0;
	}
    }

    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();
    for (int i = 0; i < L; i++) {
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2; cin.ignore();
	graph[N1][N2] = 1;
	graph[N2][N1] = 1;
    }
    for (int i = 0; i < E; i++) {
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
	gateway_nodes.push_back(EI);
    }

    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();

	vector<int> gateway_distances;
	vector<int> gateway_parents;

	gateway_distances.resize(500);
	gateway_parents.resize(500);

	/* Get gateway distances */
	bfs(graph, SI, gateway_nodes, gateway_distances, gateway_parents);

	/* Fint shoortest of all distances */
	int min = INT_MAX;
	int nearest_gateway = gateway_nodes[0];
	for(int i = 0; i < gateway_nodes.size(); i++) {
	    int gateway = gateway_nodes[i];
	    if(gateway_distances[gateway] < min) {
		min = gateway_distances[gateway];
		nearest_gateway = gateway;
	    }
	} 

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

	/* Sever link between nearest gateway and its parent */
	cout << nearest_gateway<< " " << gateway_parents[nearest_gateway]<<endl;
        cout << "0 1" << endl; // Example: 0 1 are the indices of the nodes you wish to sever the link between
    }
}
