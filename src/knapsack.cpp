#include "knapsack.hpp"

#include <algorithm>
#include <cmath>

using namespace std;

BandwidthItem makeBandwidthItem(const Solicitud& solicitud, int originalPosition) {
    BandwidthItem item;
    item.customerID = solicitud.id;
    item.tenure = solicitud.tenure;
    item.weight = static_cast<int>(llround(solicitud.totalCharges));
    item.value = static_cast<int>(llround(solicitud.monthlyCharges * 10.0));
    item.originalPosition = originalPosition;
    item.ratio = (item.weight > 0) ? static_cast<double>(item.value) / item.weight : 0.0;
    return item;
}

vector<BandwidthItem> selectTopActiveItems(const vector<Solicitud>& sortedSolicitudes, int limit) {
    vector<BandwidthItem> items;
    items.reserve(limit);

    for (int i = 0; i < static_cast<int>(sortedSolicitudes.size()) && static_cast<int>(items.size()) < limit; ++i) {
        if (sortedSolicitudes[i].churn == "No") {
            items.push_back(makeBandwidthItem(sortedSolicitudes[i], i));
        }
    }

    return items;
}

vector<BandwidthItem> selectTopFeasibleActiveItems(const vector<Solicitud>& sortedSolicitudes, int limit, int capacity) {
    vector<BandwidthItem> items;
    items.reserve(limit);

    for (int i = 0; i < static_cast<int>(sortedSolicitudes.size()) && static_cast<int>(items.size()) < limit; ++i) {
        if (sortedSolicitudes[i].churn != "No") continue;

        BandwidthItem item = makeBandwidthItem(sortedSolicitudes[i], i);
        if (item.weight > 0 && item.weight <= capacity) {
            items.push_back(item);
        }
    }

    return items;
}

KnapsackResult solveKnapsackTabulation(const vector<BandwidthItem>& items, int capacity) {
    int n = static_cast<int>(items.size());
    KnapsackResult result;
    result.dp.assign(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        int wi = items[i - 1].weight;
        int vi = items[i - 1].value;
        for (int w = 0; w <= capacity; ++w) {
            result.dp[i][w] = result.dp[i - 1][w];
            if (wi <= w) {
                result.dp[i][w] = max(result.dp[i][w], result.dp[i - 1][w - wi] + vi);
            }
        }
    }

    result.optimalValue = result.dp[n][capacity];

    int w = capacity;
    for (int i = n; i >= 1; --i) {
        if (result.dp[i][w] != result.dp[i - 1][w]) {
            const BandwidthItem& item = items[i - 1];
            result.selected.push_back(item);
            result.totalWeight += item.weight;
            w -= item.weight;
        }
    }

    reverse(result.selected.begin(), result.selected.end());
    return result;
}

GreedyResult solveGreedyByRatio(const vector<BandwidthItem>& items, int capacity) {
    vector<BandwidthItem> sortedItems = items;
    sort(sortedItems.begin(), sortedItems.end(), [](const BandwidthItem& a, const BandwidthItem& b) {
        if (a.ratio != b.ratio) return a.ratio > b.ratio;
        return a.originalPosition < b.originalPosition;
    });

    GreedyResult result;
    int remaining = capacity;
    for (const BandwidthItem& item : sortedItems) {
        if (item.weight <= remaining) {
            result.selected.push_back(item);
            result.totalWeight += item.weight;
            result.totalValue += item.value;
            remaining -= item.weight;
        }
    }

    return result;
}

CounterexampleResult findGreedyCounterexample(const vector<BandwidthItem>& items, int capacity) {
    CounterexampleResult answer;
    int n = static_cast<int>(items.size());

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                vector<BandwidthItem> triple = {items[i], items[j], items[k]};
                GreedyResult greedy = solveGreedyByRatio(triple, capacity);
                KnapsackResult dp = solveKnapsackTabulation(triple, capacity);
                if (greedy.totalValue < dp.optimalValue) {
                    answer.found = true;
                    answer.items = triple;
                    answer.greedy = greedy;
                    answer.dynamicProgramming = dp;
                    return answer;
                }
            }
        }
    }

    return answer;
}
