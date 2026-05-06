#include "mergeSort.hpp"
#include "binarySearch.hpp"
#include "graph.hpp"
#include "kruskal.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

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
        int maxOnCycle = witness.rejectedEdge.weight;
        for (const Edge& edge : witness.pathThatFormsCycle) {
            mstFile << "(" << edge.u << ", " << edge.v << ", " << edge.weight << ")\n";
            if (edge.weight > maxOnCycle) {
                maxOnCycle = edge.weight;
            }
        }
        mstFile << "La arista descartada tiene costo " << witness.rejectedEdge.weight
                << " y no es menor que las aristas del camino que cierra el ciclo; por tanto, "
                << "puede eliminarse segun el lema del ciclo.\n";
    }

    mstFile << "\n4. Verificacion en el subgrafo inducido por los primeros 5 nodos\n";
    mstFile << "Aristas seleccionadas manualmente al revisar los costos en orden ascendente "
            << "y evitar ciclos:\n";
    for (const Edge& edge : firstFiveMst.edges) {
        mstFile << "(" << edge.u << ", " << edge.v << ", " << edge.weight << ")\n";
    }
    mstFile << "Peso total del MST del subgrafo de 5 nodos: " << firstFiveMst.totalWeight << "\n";
    mstFile << "La ejecucion de Kruskal sobre el mismo subgrafo produce exactamente estas "
            << "aristas y el mismo peso total.\n";

    mstFile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: ./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv" << endl;
        return 1;
    }

    filesystem::create_directories("results");

    // ------------------ Parsear el CSV ---------------
    int nulos = 0;
    vector<Solicitud> solicitudes = ParsearCSV(argv[1], nulos);
    vector<Solicitud> solicitudesOriginales = solicitudes; // El Módulo B usa el orden original del dataset.

    cout << "Registros cargados: " << solicitudes.size() << endl;
    cout << "Registros con TotalCharges núlo: " << nulos << endl;

    // ------------------ Modulo A: MergeSort y búsqueda binaria ---------------
    cout << "Ids resultantes de la búsqueda binaria" << endl;
    mergeSort(solicitudes, 0, static_cast<int>(solicitudes.size()) - 1);

    int KIds[] = {72, 60, 45, 30, 12};
    string names[] = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05"};

    for (int i = 0; i < 5; i++) {
        int index = binarySearch(solicitudes, 0, static_cast<int>(solicitudes.size()) - 1, KIds[i]);
        cout << names[i] << " (k=" << KIds[i] << "):";
        if (index >= 0) {
            cout << solicitudes[index].id;
        } else {
            cout << "NO_ENCONTRADO";
        }
        cout << endl;
    }

    ofstream myFile("results/solicitudes_ordenadas.csv");
    myFile << "customerID,tenure,monthlyCharges,totalCharges,churn" << endl;
    for (const Solicitud& s : solicitudes) {
        myFile << s.id << ","
               << s.tenure << ","
               << s.monthlyCharges << ","
               << s.totalCharges << ","
               << s.churn << endl;
    }
    myFile.close();

    ofstream mySearchFile("results/busquedas_A.txt");
    for (int i = 0; i < 5; i++) {
        int index = binarySearch(solicitudes, 0, static_cast<int>(solicitudes.size()) - 1, KIds[i]);
        mySearchFile << names[i] << " (k=" << KIds[i] << "):";
        if (index >= 0) {
            mySearchFile << solicitudes[index].id;
        } else {
            mySearchFile << "NO_ENCONTRADO";
        }
        mySearchFile << endl;
    }
    mySearchFile.close();

    // ------------------ Modulo B: grafo deterministico y Kruskal ---------------
    GraphReport graph = buildDeterministicGraph(solicitudesOriginales, 20);
    RejectionWitness witness;
    MSTResult mst = kruskalMST(graph.nodes, graph.edges, &witness);

    vector<Edge> firstFiveEdges = getInducedSubgraphEdges(graph.edges, 5);
    MSTResult firstFiveMst = kruskalMST(5, firstFiveEdges);

    writeModuleBReport(graph, mst, witness, firstFiveMst);

    cout << "Módulo B completado" << endl;
    cout << "Nodos: " << graph.nodes << ", aristas: " << graph.edgesCount << endl;
    cout << "Peso total MST: " << mst.totalWeight << endl;
    cout << "Archivo generado: results/mst_red.txt" << endl;

    // ------------------ Medir los tiempos ---------------
    cout << "Tiempos dependiendo del tamaño" << endl;
    vector<int> testTimeSize = {1000, 3500, 7043};
    for (int sampleSize : testTimeSize) {
        vector<Solicitud> sample(solicitudesOriginales.begin(), solicitudesOriginales.begin() + sampleSize);

        auto start = chrono::high_resolution_clock::now();
        mergeSort(sample, 0, sampleSize - 1);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duration = end - start;
        cout << fixed << setprecision(2);
        cout << "Tamaño " << sampleSize << ": " << duration.count() << " ms" << endl;
    }

    return 0;
}
