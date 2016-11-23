#include <iostream>
#include <vector>
using namespace std;

int solve_crab_feet(int graph[100][100], int N, int T) {

    int vertex_degree[100] = {0};
    /* Calculate all vertices's degree to determine crab heads */
    for(int i = 0; i < N; i++) {
	for(int j = 0; j < N; j++) {
	    vertex_degree[i] += graph[i][j];
	}
    }

    vector<int> crab_heads;
    for(int i = 0; i < N; i++) {
	if(vertex_degree[i] >= T) {
	    crab_heads.push_back(i);
	}
    }
    
    while(!crab_heads.empty()) {
	/* Iterate through all crab heads; 
	 * If a crab head does not impact other crab head's potential
	 */
    }
}

int main() {

    int C;
    cin>>C;

    while(C--) {
	int N, M, T;
	int graph[100][100] = {0};
	cin>>N>>T>>M;
	for(int ip_iter = 0; ip_iter < M; ip_iter++) {
	    int u, v;
	    cin>>u>>v;
	    graph[u-1][v-1] = 1;
	    graph[v-1][u-1] = 1;
	}
	int result  = solve_crab_feet(graph, N, T);
	cout<<result;
    }
    return 0;
}
