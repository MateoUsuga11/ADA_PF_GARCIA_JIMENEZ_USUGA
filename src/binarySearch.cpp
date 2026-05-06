#include <iostream>
#include <vector>

#include "parser.hpp"
using namespace std;

int binarySearch(vector<Solicitud> &arr, int left, int right, int k) {
    if (left > right) return -1; //Caso base si no se encuentra el elemento

    int mid = left + (right - left) / 2; //Calcular el punto medio(como en mergesort primero resto)

    if (arr[mid].tenure == k) return mid; //Si mid cumple, lo encontré

    if (arr[mid].tenure > k)
        return binarySearch(arr, mid + 1, right, k); // k está más a la derecha

    return binarySearch(arr, left, mid - 1, k); // k está más a la izquierda

}
