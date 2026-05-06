#include "mergeSort.hpp"
#include "binarySearch.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: ./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv" << endl;
        return 1;
    }
    // ------------------ Parsear el CSV ---------------
    int n = 0;
    vector<Solicitud> solicitudes = ParsearCSV(argv[1], n);
    cout << "Registros cargados: " << solicitudes.size() << endl;
    cout << "Registros con TotalCharges nulo: " << n << endl;

    // ------------------ MergeSort y binarySearch ---------------
    cout << "Ids resultantes de la busqueda binaria" << endl;
    mergeSort(solicitudes,0, solicitudes.size() -1);
    //for (Solicitud x : solicitudes) cout << x.tenure << endl; con este codigo confirmo que tenure está en orden descendente
    int KIds[] = {72, 60, 45, 30, 12};
    string names[] = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05",};

    for (int i = 0; i < 5; i++) {
        int index = binarySearch(solicitudes, 0, solicitudes.size() -1, KIds[i]);
        cout << names[i] << " (k=" << KIds[i] << "):" << solicitudes[index].id << endl;
    }

    // ------------------ Crear los archivos ---------------
    ofstream myFile("../../results/solicitudes_ordenadas.csv");
    myFile << "customerID,tenure,monthlyCharges,totalCharges,churn" << endl;
    for (Solicitud s : solicitudes) {
        myFile << s.id << ","
                << s.tenure << ","
                << s.monthlyCharges << ","
                << s.totalCharges << ","
                << s.churn << endl;
    }
    myFile.close();
    ofstream mySearchFile("../../results/Busquedas_A.txt");
    for (int i = 0; i < 5; i++) {
        int index = binarySearch(solicitudes, 0, solicitudes.size() -1, KIds[i]);
        mySearchFile << names[i] << " (k=" << KIds[i] << "):" << solicitudes[index].id << endl;
    }
    mySearchFile.close();

    // ------------------ Medir los tiempos ---------------
    cout << "Tiempos dependiendo del tamano" << endl;
    vector<int> testTimeSize = {1000, 3500, 7043};
    for (int i = 0; i < 3; i++) {
        vector<Solicitud> sample(solicitudes.begin(), solicitudes.begin() + testTimeSize[i]);

        auto start = chrono::high_resolution_clock::now();
        mergeSort(sample, 0, testTimeSize[i] - 1);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duration = end - start;
        cout << "Tamano " << testTimeSize[i] << ": " << duration.count() << " ms" << endl;
    }
    mySearchFile.close();
    int idx = binarySearch(solicitudes, 0, solicitudes.size() - 1, 45);
    cout << "Indice: " << idx << ", tenure: " << solicitudes[idx].tenure << endl;

    idx = binarySearch(solicitudes, 0, solicitudes.size() - 1, 30);
    cout << "Indice: " << idx << ", tenure: " << solicitudes[idx].tenure << endl;
    return 0;
}
