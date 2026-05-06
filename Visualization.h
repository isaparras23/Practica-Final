#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <chrono>
#include "DialSort.h"
#include "StdSort.h"


// VISUALIZATION (modo consola / ASCII)
// Muestra paso a paso el comportamiento interno de cada algoritmo


class Visualization {
public:
    static const int DELAY_MS = 300; // pausa entre pasos


    // showDialSort()
    //   Muestra cómo se llenan los buckets elemento por elemento

    static void showDialSort(std::vector<int> arr, int U) {
        clearScreen();
        printHeader("DIALSORT — Visualización paso a paso");

        std::cout << "Arreglo original: ";
        printArray(arr);
        std::cout << "Universo U = " << U << "\n\n";
        sleep_ms(1000);

        // Obtener snapshots
        auto steps = DialSort::sortWithSteps(arr, U);

        // Mostrar estado de buckets tras cada inserción
        for (int step = 0; step < (int)steps.size(); step++) {
            clearScreen();
            printHeader("DIALSORT — Paso " + std::to_string(step + 1) +
                        "/" + std::to_string(steps.size()));

            std::cout << "Insertando elemento: "
                      << BOLD << arr[step] << RESET
                      << " → bucket[" << arr[step] << "]\n\n";

            printBuckets(steps[step], U);
            sleep_ms(DELAY_MS);
        }

        // Resultado final
        clearScreen();
        printHeader("DIALSORT — Resultado final");
        std::cout << "Arreglo ordenado: ";
        printArray(arr);
        std::cout << "\nComplejidad: O(n + U)\n";
    }


    // showStdSort()
    //   Muestra el arreglo antes y después de ordenar con std::sort

    static void showStdSort(std::vector<int> arr) {
        clearScreen();
        printHeader("STDSORT (std::sort) — Visualización");

        std::cout << "Arreglo original: ";
        printArray(arr);
        std::cout << "\n";
        sleep_ms(1000);

        auto steps = StdSort::sortWithSteps(arr);

        for (int step = 0; step < (int)steps.size(); step++) {
            clearScreen();
            printHeader("STDSORT — Estado " + std::to_string(step + 1) +
                        "/" + std::to_string(steps.size()));

            std::cout << "Arreglo:\n";
            printArrayDetailed(steps[step], 20);
            sleep_ms(DELAY_MS * 2);
        }

        clearScreen();
        printHeader("STDSORT — Resultado final");
        std::cout << "Arreglo ordenado: ";
        printArray(arr);
        std::cout << "\nComplejidad: O(n log n)\n";
    }


    // showComparison()
    //   Muestra una barra de progreso comparando tiempos
    static void showComparison(double dial_ms, double std_ms,
                               int n, int U) {
        printHeader("COMPARACIÓN DE RESULTADOS");

        std::cout << "n = " << n << " | U = " << U << "\n\n";

        double max_ms = std::max(dial_ms, std_ms);

        printBar("DialSort ", dial_ms, max_ms, 40);
        printBar("StdSort  ", std_ms, max_ms, 40);

        std::cout << "\n";
        if (dial_ms < std_ms)
            std::cout << GREEN << "✓ DialSort fue más rápido ("
                      << std::fixed << std::setprecision(2)
                      << (std_ms / dial_ms) << "x)\n" << RESET;
        else
            std::cout << GREEN << "✓ StdSort fue más rápido ("
                      << std::fixed << std::setprecision(2)
                      << (dial_ms / std_ms) << "x)\n" << RESET;
    }

private:
    // Códigos de color ANSI
    static constexpr const char* RESET  = "\033[0m";
    static constexpr const char* BOLD   = "\033[1m";
    static constexpr const char* GREEN  = "\033[32m";
    static constexpr const char* CYAN   = "\033[36m";
    static constexpr const char* YELLOW = "\033[33m";

    static void clearScreen() {
        std::cout << "\033[2J\033[H";
    }

    static void printHeader(const std::string& title) {
        std::string line(60, '=');
        std::cout << CYAN << line << "\n"
                  << "  " << title << "\n"
                  << line << "\n" << RESET;
    }

    static void printArray(const std::vector<int>& arr, int max = 20) {
        std::cout << "[ ";
        int show = std::min((int)arr.size(), max);
        for (int i = 0; i < show; i++)
            std::cout << arr[i] << (i < show - 1 ? ", " : "");
        if ((int)arr.size() > max)
            std::cout << " ... (" << arr.size() << " elementos)";
        std::cout << " ]\n";
    }

    static void printArrayDetailed(const std::vector<int>& arr, int per_row) {
        for (int i = 0; i < (int)arr.size(); i++) {
            std::cout << std::setw(8) << arr[i];
            if ((i + 1) % per_row == 0) std::cout << "\n";
        }
        std::cout << "\n";
    }

    static void printBuckets(const std::vector<std::list<int>>& buckets, int U) {
        int show = std::min(U, 15);  // mostrar máximo 15 buckets
        for (int i = 0; i < show; i++) {
            std::cout << YELLOW << "bucket[" << std::setw(2) << i << "]"
                      << RESET << ": ";
            if (buckets[i].empty()) {
                std::cout << "∅";
            } else {
                for (int v : buckets[i])
                    std::cout << "[" << v << "] ";
            }
            std::cout << "\n";
        }
        if (U > 15)
            std::cout << "  ... (" << U - 15 << " buckets más)\n";
    }

    static void printBar(const std::string& label, double val,
                         double max_val, int width) {
        int filled = (int)(val / max_val * width);
        std::cout << std::left << std::setw(12) << label << " │";
        std::cout << GREEN;
        for (int i = 0; i < filled; i++) std::cout << "█";
        std::cout << RESET;
        for (int i = filled; i < width; i++) std::cout << "░";
        std::cout << "│ " << std::fixed << std::setprecision(2)
                  << val << " ms\n";
    }

    static void sleep_ms(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
};

