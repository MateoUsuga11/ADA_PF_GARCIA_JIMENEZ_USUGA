#include "binary_search.hpp"

using namespace std;

int binarySearch(const vector<Solicitud>& arr, int left, int right, int k) {
    if (left > right) return -1;

    int mid = left + (right - left) / 2;

    if (arr[mid].tenure == k) {
        int earlier = binarySearch(arr, left, mid - 1, k);
        return (earlier == -1) ? mid : earlier;
    }

    // El arreglo esta en orden descendente: si mid es menor que k, se busca a la izquierda.
    if (arr[mid].tenure < k) {
        return binarySearch(arr, left, mid - 1, k);
    }

    return binarySearch(arr, mid + 1, right, k);
}
