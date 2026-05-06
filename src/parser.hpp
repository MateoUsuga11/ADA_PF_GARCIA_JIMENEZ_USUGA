#pragma once
#include <string>
#include <vector>
using namespace std;

struct Solicitud {
    string id;
    int tenure;
    float monthlyCharges;
    float totalCharges;
    string churn;
};

vector<Solicitud> ParsearCSV(const string& ruta, int& nulos);