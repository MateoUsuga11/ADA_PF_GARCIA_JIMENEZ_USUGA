#pragma once

#include "parser.hpp"
#include <string>
#include <vector>

struct BandwidthItem {
    std::string customerID;
    int tenure = 0;
    int weight = 0;
    int value = 0;
    int originalPosition = 0;
    double ratio = 0.0;
};

struct KnapsackResult {
    int optimalValue = 0;
    int totalWeight = 0;
    std::vector<BandwidthItem> selected;
    std::vector<std::vector<int>> dp;
};

struct GreedyResult {
    int totalValue = 0;
    int totalWeight = 0;
    std::vector<BandwidthItem> selected;
};

struct CounterexampleResult {
    bool found = false;
    std::vector<BandwidthItem> items;
    GreedyResult greedy;
    KnapsackResult dynamicProgramming;
};

BandwidthItem makeBandwidthItem(const Solicitud& solicitud, int originalPosition);
std::vector<BandwidthItem> selectTopActiveItems(const std::vector<Solicitud>& sortedSolicitudes, int limit);
std::vector<BandwidthItem> selectTopFeasibleActiveItems(const std::vector<Solicitud>& sortedSolicitudes, int limit, int capacity);
KnapsackResult solveKnapsackTabulation(const std::vector<BandwidthItem>& items, int capacity);
GreedyResult solveGreedyByRatio(const std::vector<BandwidthItem>& items, int capacity);
CounterexampleResult findGreedyCounterexample(const std::vector<BandwidthItem>& items, int capacity);
