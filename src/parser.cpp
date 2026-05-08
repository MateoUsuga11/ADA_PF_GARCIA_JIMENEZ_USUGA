#include "parser.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

static vector<string> splitCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += ch;
        }
    }
    fields.push_back(field);
    return fields;
}

static bool isBlank(const string& value) {
    for (char ch : value) {
        if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') {
            return false;
        }
    }
    return true;
}

vector<Solicitud> ParsearCSV(const string& ruta, int& nulos) {
    ifstream file(ruta);
    if (!file.is_open()) {
        throw runtime_error("No se pudo abrir el archivo CSV: " + ruta);
    }

    string line;
    vector<Solicitud> solicitudes;
    nulos = 0;

    getline(file, line); // Cabecera
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> columns = splitCSVLine(line);
        if (columns.size() < 21) {
            throw runtime_error("Fila CSV invalida con menos de 21 columnas: " + line);
        }

        Solicitud s;
        s.id = columns[0];
        s.tenure = stoi(columns[5]);
        s.monthlyCharges = stod(columns[18]);
        s.churn = columns[20];

        if (isBlank(columns[19])) {
            s.totalCharges = 0.0;
            nulos++;
        } else {
            s.totalCharges = stod(columns[19]);
        }

        solicitudes.push_back(s);
    }

    return solicitudes;
}
