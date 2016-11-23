#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cassert>
using namespace std;

int N, I;
vector<vector<int> > graph;

int main() {

    cin>>N>>I;

    graph.resize(N);

    /* Get input in form of a adjacency list */
    for(int i = 0; i < I; i++) {
	int u, v;
	cin>>u>>v;
	graph[u].push_back(v);
	graph[v].push_back(u);		
    }

    char *bfsMarker = (char *)calloc(sizeof(char), N);
    queue<int> bfsQueue;

    vector<int> country_astro_count;
    int countries = 0;

    for(int i = 0; i < N; i++) {
	if(!bfsMarker[i]) {

	    countries++;
	    int astro_count = 0;

	    bfsQueue.push(i);
	    bfsMarker[i] = 1;

	    while(!bfsQueue.empty()) {
		/* Get parent node */
		int parent = bfsQueue.front();
		bfsQueue.pop();
		
		astro_count++;

		/* Traverse adjacency list to find children */
		for(int child_iterator = 0; child_iterator < graph[parent].size(); child_iterator++) {
		    int child = graph[parent][child_iterator];
		    if(!bfsMarker[child]) {

			bfsQueue.push(child);
			bfsMarker[child] = 1;
#if 0
			/* Delete the (child, parent) edge in graph */
			for(int iter = 0; iter < graph[child].size(); iter++) {
			    if(graph[child][iter] == parent) {
				graph[child][iter] = -1;
				break;
			    }
			}
#endif
		    }
		}
	    }
	    
	    country_astro_count.push_back(astro_count);

	}
    }
    
    long long int result = 0;
    
#if 0
    for(int i = 0; i < country_astro_count.size() - 1; i++) {
        for(int j = i+1; j < country_astro_count.size(); j++) {
            result += country_astro_count[i]*country_astro_count[j];
        }
    }
#endif

    int total_yet = 0;

    for(int i = 0; i < country_astro_count.size() - 1; i++) {
	total_yet += country_astro_count[i];
	result += country_astro_count[i] * (N - total_yet);
    } 
    cout<<result;
    
    return 0;
}
