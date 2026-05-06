#include <fstream>
#include <vector>
#include "parser.hpp"
#include <string>
using namespace std;

vector<string> SplitCSV(string line) { //función para separar los valores por comas
    vector<string> columns;
    string field = "";

    for (char ch : line){
        if (ch == ',') {
            columns.push_back(field);
            field = ""; //guardar campo y reiniciar para el siguiente
        } else {
            field += ch;
        }
    }
    columns.push_back(field); //agregar el ultimo campo que no tiene coma
    return columns;
}

vector<Solicitud> ParsearCSV(const string& ruta, int& nulos) {
    ifstream file(ruta);
    string line;
    vector<Solicitud> solicitudes;
    getline(file, line); // Descartar cabecera
    while (getline(file, line)) {
        vector<string> columns = SplitCSV(line);

        Solicitud s; //tomar las columnas que necesito del csv
        s.id = columns[0];
        s.tenure = stoi(columns[5]);
        s.monthlyCharges = stof(columns[18]);
        s.churn = columns[20];

        //poner 0.0 en los totalcharges que están vacios
        if (columns[19].empty() || columns[19] == " ") {
            s.totalCharges = 0.0;
            nulos++;
        } else {
            s.totalCharges = stof(columns[19]);
        }
        solicitudes.push_back(s);

    }
    return solicitudes;
}



