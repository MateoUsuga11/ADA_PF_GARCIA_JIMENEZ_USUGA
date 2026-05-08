#pragma once

#include <string>
#include <vector>

struct Solicitud {
    std::string id;
    int tenure = 0;
    double monthlyCharges = 0.0;
    double totalCharges = 0.0;
    std::string churn;
};

std::vector<Solicitud> ParsearCSV(const std::string& ruta, int& nulos);
