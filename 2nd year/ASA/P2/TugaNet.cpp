#include <stack>
#include <list>
#include <vector>
#include <iostream>

typedef std::vector<std::vector<int>> Graph;

/**
 * Visits the node u and all reachable neighbours while
 * sorting them by their topological order.
 */
void DFSTopoVisit(Graph& graph, int u, std::list<int>& topo,
				  std::vector<bool>& visited, std::vector<bool>& inTopo) {
	std::stack<int> traversal;
	traversal.push(u);
	while (!traversal.empty()) {
		int cur = traversal.top();
		if (visited[cur]) {
			traversal.pop();
			if (!inTopo[cur]) {
				topo.push_front(cur);
				inTopo[cur] = true;
			}
			continue;
		}
		visited[cur] = true;
		for (auto it = graph[cur].begin(); it != graph[cur].end(); ++it)
			if (!visited[*it])
				traversal.push(*it);
	}
}

/**
 * Given a graph, calculates one of it's possible topological
 * orderings.
 */
void DFSTopo(Graph& graph, std::list<int>& topo) {
	std::vector<bool> visited(graph.size(), false);
	std::vector<bool> inTopo(graph.size(), false); // avoids duplicates
	for (unsigned int i = 1; i < graph.size(); i++)
		if (!visited[i])
			DFSTopoVisit(graph, i, topo, visited, inTopo);
}

/**
 * Finds all nodes in the current SCC and sets the maximum jumps
 * for that SCC. Returns the maximum jumps of the current SCC.
 */
int DFSmaxJumpsVisit(Graph& transposed, int u, int sccIdx, std::vector<int>& scc,
					 std::vector<int>& distance, std::vector<bool>& visited) {
	std::stack<int> traversal; // nodes in traversal
	traversal.push(u);
	while (!traversal.empty()) {
		int cur = traversal.top();
		traversal.pop();
		if (visited[cur])
			continue;
		scc[cur] = sccIdx;
		visited[cur] = true;
		for (auto it = transposed[cur].begin(); it != transposed[cur].end(); ++it) {
			if (!visited[*it]) {
				traversal.push(*it);
	   			continue;
			}
			if (scc[*it] != scc[cur])
				distance[sccIdx] = std::max(distance[sccIdx], distance[scc[*it]] + 1);
			// if a node points outside, to another SCC, then we max
			// the currently obtained distance of the SCC with the
			// distance of that neighbour + 1
	   }
	}
	return distance[sccIdx];
}

/**
 * Calculates the longest path present on the transposed graph,
 * considering it only as a DAG (edges between nodes in the same
 * SCC do not count towards the longest path).
 */
int maxJumps(Graph& transposed, std::list<int>& topo) {
	std::vector<int> scc(transposed.size(), 0); // scc of each node
	std::vector<int> distance(transposed.size(), 0); // max jumps of each scc
	std::vector<bool> visited(transposed.size(), false); // status of each node
	int sccIdx = 1;
	int max = 0;
	distance[0] = -1;
	for (auto it = topo.begin(); it != topo.end(); ++it) {
		if (!visited[*it]) {
			max = std::max(max, DFSmaxJumpsVisit(transposed, *it, sccIdx,
												scc, distance, visited));
			sccIdx++;
		}
	}
	return max;
}

int main() {
	std::ios::sync_with_stdio(false);
	int size = -1, edges = -1;
	std::cin >> size >> edges;
	Graph graph(size + 1);
	Graph transposed(size + 1);
	for (int i = 0; i < edges; i++) {
		int u = -1, v = -1; // insert all edges in the both graphs
		std::cin >> u >> v;
		graph[u].push_back(v);
		transposed[v].push_back(u);
	}
	std::list<int> topo;
	DFSTopo(graph, topo); // calculate topological sort
	graph.clear(); // no need for the original graph anymore
	std::cout << maxJumps(transposed, topo) << '\n'; // calculate max jumps
}
