#pragma once

#include <vector>
#include "parser.hpp"

struct Edge {
    int u;
    int v;
    int weight;
};

struct GraphReport {
    int nodes;
    int edgesCount;
    double averageEdgeCost;
    std::vector<double> groupMonthlyAverages;
    std::vector<Edge> edges;
};

double roundToTwoDecimals(double value);
GraphReport buildDeterministicGraph(const std::vector<Solicitud>& solicitudes, int nodes = 20);
std::vector<Edge> getInducedSubgraphEdges(const std::vector<Edge>& edges, int firstNodesCount);
