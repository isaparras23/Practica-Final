#pragma once
#include <vector>
#include <algorithm>


// STDSORT
// Algoritmo estándar de C++ (Introsort).
// Combina QuickSort, HeapSort y MergeSort.

class StdSort {
public:
    // Ordena el vector usando std::sort
    static void sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        std::sort(arr.begin(), arr.end());
    }

    // Versión con pasos (para visualización)
    // Nota: std::sort no expone pasos internos,
    // así que aquí mostramos solo el estado inicial y final.
    static std::vector<std::vector<int>> sortWithSteps(std::vector<int>& arr) {
        std::vector<std::vector<int>> steps;
        if (arr.empty()) return steps;

        // Snapshot inicial
        steps.push_back(arr);

        // Ordenar con std::sort
        std::sort(arr.begin(), arr.end());

        // Snapshot final
        steps.push_back(arr);

        return steps;
    }
};
