#include "mergesort.hpp"

using namespace std;

void merge(vector<Solicitud>& arr, int left, int mid, int right) {
    vector<Solicitud> l(arr.begin() + left, arr.begin() + mid + 1);
    vector<Solicitud> r(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0;
    int j = 0;
    int k = left;

    while (i < static_cast<int>(l.size()) && j < static_cast<int>(r.size())) {
        // >= mantiene la estabilidad: ante empate por tenure se conserva el orden original.
        if (l[i].tenure >= r[j].tenure) {
            arr[k++] = l[i++];
        } else {
            arr[k++] = r[j++];
        }
    }

    while (i < static_cast<int>(l.size())) arr[k++] = l[i++];
    while (j < static_cast<int>(r.size())) arr[k++] = r[j++];
}

void mergeSort(vector<Solicitud>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
