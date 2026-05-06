#pragma once

#include <vector>
#include "graph.hpp"

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    explicit UnionFind(int n);
    int find(int x);
    bool unite(int a, int b);
};

struct RejectionWitness {
    bool found = false;
    Edge rejectedEdge{0, 0, 0};
    std::vector<Edge> pathThatFormsCycle;
};

struct MSTResult {
    int totalWeight = 0;
    std::vector<Edge> edges;
};

MSTResult kruskalMST(int nodes, const std::vector<Edge>& edges, RejectionWitness* witness = nullptr);
