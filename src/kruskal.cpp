#include "kruskal.hpp"

#include <algorithm>

using namespace std;

UnionFind::UnionFind(int n) {
    parent.resize(n);
    rank.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

int UnionFind::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]); // Compresion de caminos
    }
    return parent[x];
}

bool UnionFind::unite(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);

    if (rootA == rootB) {
        return false;
    }

    // Union por rango
    if (rank[rootA] < rank[rootB]) {
        parent[rootA] = rootB;
    } else if (rank[rootA] > rank[rootB]) {
        parent[rootB] = rootA;
    } else {
        parent[rootB] = rootA;
        rank[rootA]++;
    }

    return true;
}

static bool edgeLess(const Edge& a, const Edge& b) {
    if (a.weight != b.weight) return a.weight < b.weight;
    if (a.u != b.u) return a.u < b.u;
    return a.v < b.v;
}

static bool findPathDfs(int current,
                        int target,
                        const vector<vector<Edge>>& adjacency,
                        vector<int>& visited,
                        vector<Edge>& path) {
    if (current == target) {
        return true;
    }

    visited[current] = 1;
    for (const Edge& edge : adjacency[current]) {
        int next = (edge.u == current) ? edge.v : edge.u;
        if (!visited[next]) {
            path.push_back(edge);
            if (findPathDfs(next, target, adjacency, visited, path)) {
                return true;
            }
            path.pop_back();
        }
    }

    return false;
}

MSTResult kruskalMST(int nodes, const vector<Edge>& edges, RejectionWitness* witness) {
    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), edgeLess);

    UnionFind uf(nodes);
    MSTResult result;
    vector<vector<Edge>> selectedAdjacency(nodes);

    for (const Edge& edge : sortedEdges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v);
            result.edges.push_back(edge);
            result.totalWeight += edge.weight;
            selectedAdjacency[edge.u].push_back(edge);
            selectedAdjacency[edge.v].push_back(edge);

            if (static_cast<int>(result.edges.size()) == nodes - 1 &&
                (witness == nullptr || witness->found)) {
                break;
            }
        } else if (witness != nullptr && !witness->found) {
            vector<int> visited(nodes, 0);
            vector<Edge> path;
            findPathDfs(edge.u, edge.v, selectedAdjacency, visited, path);
            witness->found = true;
            witness->rejectedEdge = edge;
            witness->pathThatFormsCycle = path;
        }
    }

    return result;
}
