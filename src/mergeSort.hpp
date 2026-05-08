#pragma once

#include "parser.hpp"
#include <vector>

void merge(std::vector<Solicitud>& arr, int left, int mid, int right);
void mergeSort(std::vector<Solicitud>& arr, int left, int right);
