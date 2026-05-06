#pragma once
#include <vector>
#include <list>
#include <stdexcept>


// DIALSORT
// Algoritmo tipo Bucket Sort que usa U cubetas.
// Cada valor se coloca en la cubeta con su mismo índice.


class DialSort {
public:
    // Ordena el vector usando DialSort
    static void sort(std::vector<int>& arr, int U) {
        if (arr.empty() || U <= 0) return;

        // Crear U cubetas (listas vacías)
        std::vector<std::list<int>> buckets(U);

        // Distribuir cada elemento en su cubeta
        for (int x : arr) {
            if (x < 0 || x >= U) {
                throw std::out_of_range("Valor fuera del universo [0, U-1]: " + std::to_string(x));
            }
            buckets[x].push_back(x);
        }

        // Reconstruir el arreglo recorriendo las cubetas
        int idx = 0;
        for (int i = 0; i < U; i++) {
            for (int val : buckets[i]) {
                arr[idx++] = val;
            }
        }
    }

    // Versión con pasos (para visualización en clase)
    static std::vector<std::vector<std::list<int>>> sortWithSteps(std::vector<int>& arr, int U) {
        std::vector<std::vector<std::list<int>>> steps;
        std::vector<std::list<int>> buckets(U);

        for (int x : arr) {
            buckets[x].push_back(x);
            steps.push_back(buckets); // snapshot
        }

        int idx = 0;
        for (int i = 0; i < U; i++)
            for (int val : buckets[i])
                arr[idx++] = val;

        return steps;
    }
};

