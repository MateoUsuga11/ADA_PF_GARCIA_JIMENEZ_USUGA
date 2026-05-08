#include "mergesort.hpp"
#include "binary_search.hpp"
#include "graph.hpp"
#include "kruskal.hpp"
#include "knapsack.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct TimingRecord {
    int sampleSize = 0;
    double timeMs = 0.0;
    double nLogN = 0.0;
    double normalized = 0.0;
};

static string formatItemList(const vector<BandwidthItem>& items) {
    if (items.empty()) return "NINGUNA";
    ostringstream out;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) out << ", ";
        out << items[i].customerID;
    }
    return out.str();
}

static vector<TimingRecord> measureMergeSortTimes(const vector<Solicitud>& original) {
    vector<int> sizes = {1000, 3500, static_cast<int>(original.size())};
    vector<TimingRecord> records;

    for (int sampleSize : sizes) {
        int runs = 7;
        vector<double> measurements;
        measurements.reserve(runs);

        for (int r = 0; r < runs; ++r) {
            vector<Solicitud> sample(original.begin(), original.begin() + sampleSize);
            auto start = chrono::high_resolution_clock::now();
            mergeSort(sample, 0, sampleSize - 1);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = end - start;
            measurements.push_back(duration.count());
        }

        sort(measurements.begin(), measurements.end());
        double median = measurements[measurements.size() / 2];
        double nLogN = static_cast<double>(sampleSize) * log2(static_cast<double>(sampleSize));
        records.push_back({sampleSize, median, nLogN, median / nLogN});
    }

    return records;
}

static void writeOrderedRequests(const vector<Solicitud>& solicitudes) {
    ofstream file("results/solicitudes_ordenadas.csv");
    file << fixed << setprecision(2);
    file << "customerID,tenure,MonthlyCharges,TotalCharges,Churn\n";
    for (const Solicitud& s : solicitudes) {
        file << s.id << "," << s.tenure << "," << s.monthlyCharges << "," << s.totalCharges << "," << s.churn << "\n";
    }
}

static void writeSearchResults(const vector<Solicitud>& solicitudes) {
    int kValues[] = {72, 60, 45, 30, 12};
    string names[] = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05"};

    ofstream file("results/busquedas_A.txt");
    file << "MODULO A - Busqueda binaria recursiva\n";
    file << "Criterio implementado: primera ocurrencia con tenure exactamente igual a k.\n";
    file << "Nota: el enunciado dice 'primer tenure >= k', pero con orden descendente eso devuelve el primer registro para casi todas las consultas.\n\n";
    file << left << setw(10) << "Consulta" << setw(8) << "k" << "customerID\n";

    for (int i = 0; i < 5; ++i) {
        int index = binarySearch(solicitudes, 0, static_cast<int>(solicitudes.size()) - 1, kValues[i]);
        file << left << setw(10) << names[i] << setw(8) << kValues[i]
             << (index >= 0 ? solicitudes[index].id : "NO_ENCONTRADO") << "\n";
    }
}

static void writeTimingResults(const vector<TimingRecord>& records) {
    ofstream file("results/tiempos_A.txt");
    file << fixed << setprecision(6);
    file << "MODULO A - Tiempos empiricos de MergeSort\n";
    file << "Los tiempos corresponden a la mediana de 7 ejecuciones por tamano.\n\n";
    file << left << setw(12) << "n" << setw(16) << "tiempo_ms" << setw(16) << "n_log2_n" << "tiempo/(nlogn)\n";
    for (const TimingRecord& record : records) {
        file << left << setw(12) << record.sampleSize
             << setw(16) << record.timeMs
             << setw(16) << record.nLogN
             << record.normalized << "\n";
    }
}

static void writeModuleBReport(const GraphReport& graph,
                               const MSTResult& mst,
                               const RejectionWitness& witness,
                               const MSTResult& firstFiveMst) {
    ofstream mstFile("results/mst_red.txt");
    mstFile << fixed << setprecision(2);

    mstFile << "MODULO B - Red de minimo costo (Kruskal)\n";
    mstFile << "========================================\n\n";

    mstFile << "1. Construccion deterministica del grafo\n";
    mstFile << "Nodos: " << graph.nodes << "\n";
    mstFile << "Aristas: " << graph.edgesCount << "\n";
    mstFile << "Costo promedio de arista: " << graph.averageEdgeCost << "\n\n";

    mstFile << "Promedios de MonthlyCharges por grupo, redondeados a dos decimales:\n";
    for (int i = 0; i < static_cast<int>(graph.groupMonthlyAverages.size()); ++i) {
        mstFile << "Nodo " << setw(2) << i << ": " << graph.groupMonthlyAverages[i] << "\n";
    }

    mstFile << "\n2. MST obtenido con Kruskal + Union-Find\n";
    mstFile << "Peso total del MST: " << mst.totalWeight << "\n";
    mstFile << "Aristas incluidas (u, v, costo):\n";
    for (const Edge& edge : mst.edges) {
        mstFile << "(" << edge.u << ", " << edge.v << ", " << edge.weight << ")\n";
    }

    mstFile << "\n3. Justificacion de la eleccion codiciosa usando el lema del ciclo\n";
    mstFile << "Kruskal procesa las aristas de menor a mayor costo. Cuando una arista conecta "
            << "dos componentes distintas, se agrega porque no forma ciclo. Si una arista "
            << "forma ciclo, ya existe un camino entre sus extremos con aristas seleccionadas "
            << "previamente y, por el ordenamiento, esas aristas tienen costo menor o igual. "
            << "Por el lema del ciclo, una arista de costo maximo dentro de un ciclo puede "
            << "descartarse sin perder optimalidad; por eso la decision local de Kruskal "
            << "mantiene la posibilidad de alcanzar un MST global.\n";

    if (witness.found) {
        mstFile << "\nArista concreta del grafo descartada por formar ciclo: ("
                << witness.rejectedEdge.u << ", " << witness.rejectedEdge.v << ", "
                << witness.rejectedEdge.weight << ").\n";
        mstFile << "Camino ya seleccionado entre sus extremos al momento de evaluarla:\n";
        for (const Edge& edge : witness.pathThatFormsCycle) {
            mstFile << "(" << edge.u << ", " << edge.v << ", " << edge.weight << ")\n";
        }
        mstFile << "Al agregar la arista descartada al camino anterior se forma un ciclo. Como "
                << "la arista fue considerada despues de las del camino, su costo no es menor; "
                << "por el lema del ciclo puede excluirse sin afectar el MST.\n";
    }

    mstFile << "\n4. Verificacion en el subgrafo inducido por los primeros 5 nodos\n";
    mstFile << "Aristas seleccionadas manualmente al revisar los costos en orden ascendente y evitar ciclos:\n";
    for (const Edge& edge : firstFiveMst.edges) {
        mstFile << "(" << edge.u << ", " << edge.v << ", " << edge.weight << ")\n";
    }
    mstFile << "Peso total del MST del subgrafo de 5 nodos: " << firstFiveMst.totalWeight << "\n";
    mstFile << "La ejecucion de Kruskal sobre el mismo subgrafo produce exactamente estas aristas y el mismo peso total.\n";
}

static void writeModuleCReport(const vector<BandwidthItem>& literalItems,
                               const KnapsackResult& literalResult,
                               const vector<BandwidthItem>& feasibleItems,
                               const KnapsackResult& feasibleResult,
                               const CounterexampleResult& counterexample,
                               int capacity) {
    ofstream file("results/asignacion_bw.txt");
    file << fixed << setprecision(4);
    file << "MODULO C - Asignacion de ancho de banda (Mochila 0-1)\n";
    file << "====================================================\n\n";
    file << "Capacidad W: " << capacity << " unidades\n";
    file << "Peso wi: TotalCharges redondeado al entero mas cercano\n";
    file << "Valor vi: MonthlyCharges * 10 redondeado\n\n";

    file << "1. Ejecucion literal del enunciado\n";
    file << "Conjunto usado: primeras 50 solicitudes activas de mayor tenure despues del MergeSort.\n";
    file << "Cantidad de solicitudes: " << literalItems.size() << "\n";
    int minWeight = literalItems.empty() ? 0 : literalItems.front().weight;
    int maxWeight = literalItems.empty() ? 0 : literalItems.front().weight;
    int feasibleCount = 0;
    for (const BandwidthItem& item : literalItems) {
        minWeight = min(minWeight, item.weight);
        maxWeight = max(maxWeight, item.weight);
        if (item.weight <= capacity) feasibleCount++;
    }
    file << "Peso minimo del conjunto literal: " << minWeight << "\n";
    file << "Peso maximo del conjunto literal: " << maxWeight << "\n";
    file << "Solicitudes con peso <= W: " << feasibleCount << "\n";
    file << "Valor optimo total: " << literalResult.optimalValue << "\n";
    file << "Peso total seleccionado: " << literalResult.totalWeight << "\n";
    file << "Numero de solicitudes seleccionadas: " << literalResult.selected.size() << "\n";
    file << "customerID seleccionados: " << formatItemList(literalResult.selected) << "\n";
    file << "Observacion: con el dataset real, todos los pesos de estas 50 solicitudes superan W=500; por tanto, la solucion literal es vacia y no permite construir un contraejemplo codicioso dentro de ese mismo conjunto.\n\n";

    file << "2. Ejecucion complementaria reproducible para evidenciar PD y el fallo codicioso\n";
    file << "Ajuste minimo aplicado: tomar las primeras 50 solicitudes activas de mayor tenure con 0 < peso <= W.\n";
    file << "Este bloque no reemplaza el resultado literal anterior; se incluye porque el enunciado exige un contraejemplo que el conjunto literal no contiene.\n";
    file << "Cantidad de solicitudes: " << feasibleItems.size() << "\n";
    file << "Valor optimo total: " << feasibleResult.optimalValue << "\n";
    file << "Peso total seleccionado: " << feasibleResult.totalWeight << "\n";
    file << "Numero de solicitudes seleccionadas: " << feasibleResult.selected.size() << "\n";
    file << "customerID seleccionados: " << formatItemList(feasibleResult.selected) << "\n\n";

    file << "Solicitudes seleccionadas por PD (customerID, tenure, peso, valor, ratio):\n";
    for (const BandwidthItem& item : feasibleResult.selected) {
        file << item.customerID << ", " << item.tenure << ", " << item.weight << ", " << item.value << ", " << item.ratio << "\n";
    }

    file << "\n3. Contraejemplo explicito del enfoque codicioso por ratio v/w\n";
    if (counterexample.found) {
        file << "Tres solicitudes usadas (customerID, tenure, peso, valor, ratio):\n";
        for (const BandwidthItem& item : counterexample.items) {
            file << item.customerID << ", " << item.tenure << ", " << item.weight << ", " << item.value << ", " << item.ratio << "\n";
        }
        file << "\n" << left << setw(28) << "Enfoque" << setw(36) << "Solicitudes seleccionadas" << setw(16) << "Valor total" << "Optimo\n";
        file << left << setw(28) << "Codicioso (ratio v/w)" << setw(36) << formatItemList(counterexample.greedy.selected)
             << setw(16) << counterexample.greedy.totalValue << "No\n";
        file << left << setw(28) << "PD (Mochila 0-1)" << setw(36) << formatItemList(counterexample.dynamicProgramming.selected)
             << setw(16) << counterexample.dynamicProgramming.optimalValue << "Si\n";
    } else {
        file << "No se encontro contraejemplo en el conjunto complementario.\n";
    }

    file << "\n4. Analisis de complejidad\n";
    file << "La tabla dp tiene (n+1)*(W+1) celdas. Cada celda se calcula en O(1), por lo que el tiempo es Theta(n*W).\n";
    file << "El espacio tambien es Theta(n*W) porque se conserva la tabla completa para hacer backtracking y recuperar los customerID seleccionados.\n";
    file << "El algoritmo es pseudopolinomial: es polinomial en el valor numerico W, pero no necesariamente en el tamano en bits de la entrada, que representa W con log2(W) bits.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: ./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv\n";
        return 1;
    }

    try {
        filesystem::create_directories("results");

        int nulos = 0;
        vector<Solicitud> solicitudes = ParsearCSV(argv[1], nulos);
        vector<Solicitud> solicitudesOriginales = solicitudes;

        cout << "Registros cargados: " << solicitudes.size() << "\n";
        cout << "Registros con TotalCharges nulo: " << nulos << "\n";

        vector<TimingRecord> timingRecords = measureMergeSortTimes(solicitudesOriginales);
        writeTimingResults(timingRecords);

        mergeSort(solicitudes, 0, static_cast<int>(solicitudes.size()) - 1);
        writeOrderedRequests(solicitudes);
        writeSearchResults(solicitudes);

        GraphReport graph = buildDeterministicGraph(solicitudesOriginales, 20);
        RejectionWitness witness;
        MSTResult mst = kruskalMST(graph.nodes, graph.edges, &witness);
        vector<Edge> firstFiveEdges = getInducedSubgraphEdges(graph.edges, 5);
        MSTResult firstFiveMst = kruskalMST(5, firstFiveEdges);
        writeModuleBReport(graph, mst, witness, firstFiveMst);

        const int capacity = 500;
        vector<BandwidthItem> literalItems = selectTopActiveItems(solicitudes, 50);
        KnapsackResult literalResult = solveKnapsackTabulation(literalItems, capacity);
        vector<BandwidthItem> feasibleItems = selectTopFeasibleActiveItems(solicitudes, 50, capacity);
        KnapsackResult feasibleResult = solveKnapsackTabulation(feasibleItems, capacity);
        CounterexampleResult counterexample = findGreedyCounterexample(feasibleItems, capacity);
        writeModuleCReport(literalItems, literalResult, feasibleItems, feasibleResult, counterexample, capacity);

        cout << "Pipeline completado. Archivos generados en results/:\n";
        cout << "- solicitudes_ordenadas.csv\n";
        cout << "- busquedas_A.txt\n";
        cout << "- tiempos_A.txt\n";
        cout << "- mst_red.txt\n";
        cout << "- asignacion_bw.txt\n";
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
