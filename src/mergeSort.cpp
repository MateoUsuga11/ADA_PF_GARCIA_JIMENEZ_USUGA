#include "mergeSort.hpp"
using namespace std;

void merge(vector<Solicitud> &arr, int left, int mid, int right) {
    vector<Solicitud> l(arr.begin() + left, arr.begin() + mid + 1); // Vectores temporales
    vector<Solicitud> r(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while(i < l.size() && j < r.size()) { //comparar elementos y escribir el mayor en arr
        if(l[i].tenure >= r[j].tenure) {
            arr[k++] = l[i++];
        } else {
            arr[k++] = r[j++];
        }
    }
    while(i < l.size()){ // Copiar elementos restantes
        arr[k++] = l[i++];
    }
    while(j < r.size()) {
        arr[k++] = r[j++];
    }
}
void mergeSort(vector<Solicitud> &arr, int left, int right) {
    if (left >= right) return; //Caso base, un solo elemento ordenado

    int mid = left + (right - left) / 2;//Calcular el punto medio(aquí resto primero para evitar overflow)

    mergeSort(arr, left, mid); //Ordenar cada mitad
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right); //combinar mitades ordenadas
}