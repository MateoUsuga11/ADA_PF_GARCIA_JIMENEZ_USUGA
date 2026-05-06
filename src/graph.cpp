#include "graph.hpp"

#include <cmath>
#include <stdexcept>

using namespace std;

double roundToTwoDecimals(double value) {
    return round(value * 100.0) / 100.0;
}

GraphReport buildDeterministicGraph(const vector<Solicitud>& solicitudes, int nodes) {
    if (nodes <= 0) {
        throw invalid_argument("El numero de nodos debe ser positivo.");
    }
    if (solicitudes.empty()) {
        throw invalid_argument("No hay solicitudes para construir el grafo.");
    }

    vector<double> sums(nodes, 0.0);
    vector<int> counts(nodes, 0);

    for (size_t i = 0; i < solicitudes.size(); ++i) {
        int group = static_cast<int>(i % nodes);
        sums[group] += solicitudes[i].monthlyCharges;
        counts[group]++;
    }

    vector<double> averages(nodes, 0.0);
    for (int i = 0; i < nodes; ++i) {
        if (counts[i] > 0) {
            averages[i] = roundToTwoDecimals(sums[i] / counts[i]);
        }
    }

    vector<Edge> edges;
    edges.reserve(static_cast<size_t>(nodes * (nodes - 1) / 2));

    long long totalCost = 0;
    for (int u = 0; u < nodes; ++u) {
        for (int v = u + 1; v < nodes; ++v) {
            int cost = static_cast<int>(floor(averages[u] + averages[v]));
            edges.push_back({u, v, cost});
            totalCost += cost;
        }
    }

    GraphReport report;
    report.nodes = nodes;
    report.edgesCount = static_cast<int>(edges.size());
    report.averageEdgeCost = edges.empty() ? 0.0 : static_cast<double>(totalCost) / edges.size();
    report.groupMonthlyAverages = averages;
    report.edges = edges;

    return report;
}

vector<Edge> getInducedSubgraphEdges(const vector<Edge>& edges, int firstNodesCount) {
    vector<Edge> subgraph;
    for (const Edge& edge : edges) {
        if (edge.u < firstNodesCount && edge.v < firstNodesCount) {
            subgraph.push_back(edge);
        }
    }
    return subgraph;
}
