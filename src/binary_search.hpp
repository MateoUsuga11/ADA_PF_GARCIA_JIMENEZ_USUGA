#pragma once

#include "parser.hpp"
#include <vector>

// Retorna la primera posicion cuyo tenure es exactamente k en un arreglo ordenado
// descendentemente por tenure. Se implementa de forma recursiva.
int binarySearch(const std::vector<Solicitud>& arr, int left, int right, int k);
