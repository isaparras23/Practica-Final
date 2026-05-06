#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "DialSort.h"
#include "StdSort.h"
#include "Benchmark.h"
#include "Visualization.h"


// MAIN - Menú principal
// Ejecuta: visualización, benchmark, o comparación rápida


// Muestra el menú en consola
void printMenu() {
    std::cout << "========================================\n\n";
    std::cout << "   DIALSORT vs STDSORT \n\n";
    std::cout << "========================================\n\n";
    std::cout << "  1. Visualizar DialSort (demo pequeño)\n";
    std::cout << "  2. Visualizar StdSort (demo pequeño)\n";
    std::cout << "  3. Comparación rápida (n personalizado)\n";
    std::cout << "  4. Benchmark completo → CSV\n";
    std::cout << "  5. Verificar correctitud\n";
    std::cout << "  0. Salir\n\n";
    std::cout << "========================================\n\n";
    std::cout << "Opcion: ";
}

// Verifica que ambos algoritmos producen el mismo resultado
bool verifyCorrecto(int n, int U) {
    auto arr = Benchmark::genUniform(n, U, 99);
    auto a1 = arr, a2 = arr;

    DialSort::sort(a1, U);
    StdSort::sort(a2);

    // Comparar con std::sort (referencia)
    auto ref = arr;
    std::sort(ref.begin(), ref.end());

    bool dial_ok = (a1 == ref);
    bool std_ok  = (a2 == ref);

    std::cout << "\n✓ DialSort correctitud: " << (dial_ok ? "\033[32mOK\033[0m" : "\033[31mFALL\033[0m") << "\n";
    std::cout << "✓ StdSort  correctitud: " << (std_ok  ? "\033[32mOK\033[0m" : "\033[31mFALL\033[0m") << "\n";
    return dial_ok && std_ok;
}

int main() {
    int opcion = -1;

    while (opcion != 0) {
        printMenu();
        std::cin >> opcion;

        switch (opcion) {

        case 1: {
            // Demo visual DialSort con array pequeño
            int U = 10;
            std::vector<int> demo = {7, 2, 5, 1, 8, 3, 9, 4, 6, 0};
            Visualization::showDialSort(demo, U);
            break;
        }

        case 2: {
            // Demo visual StdSort con array pequeño
            std::vector<int> demo = {170, 45, 75, 90, 802, 24, 2, 66};
            Visualization::showStdSort(demo);
            break;
        }

        case 3: {
            // Comparación rápida con parámetros del usuario
            int n, U;
            std::cout << "Tamaño n (ej: 100000): "; std::cin >> n;
            std::cout << "Universo U (ej: 10000): "; std::cin >> U;

            auto arr = Benchmark::genUniform(n, U);

            double dial_ms = -1, std_ms;

            // Ejecutar DialSort si U no es demasiado grande
            if ((long long)U * sizeof(std::list<int>) < 200 * 1024 * 1024LL) {
                auto a1 = arr;
                auto t1 = std::chrono::high_resolution_clock::now();
                DialSort::sort(a1, U);
                auto t2 = std::chrono::high_resolution_clock::now();
                dial_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();
            } else {
                std::cout << "\033[33m[!] U demasiado grande para DialSort, se omite\033[0m\n";
            }

            // Ejecutar StdSort
            auto a2 = arr;
            auto t3 = std::chrono::high_resolution_clock::now();
            StdSort::sort(a2);
            auto t4 = std::chrono::high_resolution_clock::now();
            std_ms = std::chrono::duration<double, std::milli>(t4 - t3).count();

            if (dial_ms > 0)
                Visualization::showComparison(dial_ms, std_ms, n, U);
            else
                std::cout << "StdSort: " << std_ms << " ms\n";
            break;
        }

        case 4: {
            // Benchmark completo → guarda CSV
            std::string path = "../results/benchmark_results.csv";
            std::cout << "Iniciando benchmark completo...\n";
            std::cout << "(Esto puede tardar)\n\n";
            Benchmark::runFullSuite(path);
            break;
        }

        case 5: {
            std::cout << "Verificando con n=100000, U=10000...\n";
            verifyCorrecto(100000, 10000);
            break;
        }

        case 0:
            std::cout << "¡Hasta luego!\n";
            break;

        default:
            std::cout << "Opción inválida.\n";
        }

        if (opcion != 0) {
            std::cout << "\nPresiona Enter para continuar...";
            std::cin.ignore();
            std::cin.get();
        }
    }

    return 0;
}

