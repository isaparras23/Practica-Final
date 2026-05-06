#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <numeric>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include "DialSort.h"
#include "StdSort.h"


// BENCHMARK
// Ejecuta pruebas con DialSort y StdSort (std::sort).
// Mide: tiempo medio, desviación estándar, throughput y memoria.


struct BenchmarkResult {
    std::string algorithm;       // Nombre del algoritmo
    std::string distribution;    // Tipo de dataset (Uniform, Sorted, etc.)
    int n;                       // Tamaño del dataset
    int U;                       // Universo de valores
    double mean_ms;              // Tiempo medio en ms
    double std_ms;               // Desviación estándar en ms
    double throughput_mps;       // Millones de elementos por segundo
    double memory_mb;            // Memoria estimada en MB
};

class Benchmark {
public:
    static const int RUNS = 5; // Número de repeticiones por escenario

    // Generadores de datasets
    static std::vector<int> genUniform(int n, int U, unsigned seed = 42) {
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(0, U - 1);
        std::vector<int> arr(n);
        for (auto& x : arr) x = dist(rng);
        return arr;
    }

    static std::vector<int> genSorted(int n, int U) {
        std::vector<int> arr(n);
        for (int i = 0; i < n; i++)
            arr[i] = (long long)i * (U - 1) / (n - 1);
        return arr;
    }

    static std::vector<int> genReverse(int n, int U) {
        auto arr = genSorted(n, U);
        std::reverse(arr.begin(), arr.end());
        return arr;
    }

    static std::vector<int> genClustered(int n, int U, unsigned seed = 42) {
        std::mt19937 rng(seed);
        int clusters = std::max(1, U / 10);
        std::uniform_int_distribution<int> dist(0, clusters - 1);
        std::vector<int> arr(n);
        for (auto& x : arr) x = dist(rng) * 10;
        return arr;
    }


    // runSingle(): ejecuta una sola corrida y mide tiempo
    static double runSingle(const std::string& algo, std::vector<int> arr, int U) {
        auto start = std::chrono::high_resolution_clock::now();

        if (algo == "DialSort") {
            DialSort::sort(arr, U);
        } else if (algo == "StdSort") {
            StdSort::sort(arr);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        return elapsed.count();
    }


    // runBenchmark(): ejecuta varias corridas y calcula estadísticas
    static BenchmarkResult runBenchmark(const std::string& algo,
                                        const std::string& dist_name,
                                        const std::vector<int>& base_arr,
                                        int U) {
        int n = base_arr.size();
        std::vector<double> times;
        times.reserve(RUNS);

        // Repetir RUNS veces para calcular promedio y desviación
        for (int r = 0; r < RUNS; r++) {
            auto arr = base_arr;
            times.push_back(runSingle(algo, arr, U));
        }

        // Media
        double mean = std::accumulate(times.begin(), times.end(), 0.0) / RUNS;

        // Desviación estándar
        double sq_sum = 0;
        for (double t : times) sq_sum += (t - mean) * (t - mean);
        double std_dev = std::sqrt(sq_sum / RUNS);

        // Throughput (millones de elementos por segundo)
        double throughput = (n / mean) / 1000.0;

        // Memoria estimada
        double memory = 0;
        if (algo == "DialSort") {
            memory = (U * sizeof(std::list<int>) + n * sizeof(int)) / 1e6;
        } else {
            memory = (n * sizeof(int)) / 1e6;
        }

        return {algo, dist_name, n, U, mean, std_dev, throughput, memory};
    }


    // runFullSuite(): ejecuta todos los escenarios y guarda CSV
    static void runFullSuite(const std::string& csv_path) {
        std::vector<BenchmarkResult> results;

        // Tamaños de dataset
        std::vector<int> sizes = {10000, 50000, 100000};

        // Universos de prueba
        std::vector<std::pair<int,std::string>> universes = {
            {1000, "U=1K"},
            {10000, "U=10K"}
        };

        // Algoritmos a comparar
        std::vector<std::string> algos = {"DialSort", "StdSort"};

        int total = sizes.size() * universes.size() * 3 * algos.size();
        int done = 0;

        for (int n : sizes) {
            for (auto& [U, Uname] : universes) {
                auto distros = std::vector<std::pair<std::string, std::vector<int>>>{
                    {"Uniform",   genUniform(n, U)},
                    {"Sorted",    genSorted(n, U)},
                    {"Clustered", genClustered(n, U)},
                };

                for (auto& [dname, arr] : distros) {
                    for (auto& algo : algos) {
                        std::cout << "[" << ++done << "/" << total << "] "
                                  << algo << " | n=" << n
                                  << " | " << Uname
                                  << " | " << dname << "...\r" << std::flush;

                        auto res = runBenchmark(algo, dname + "_" + Uname, arr, U);
                        results.push_back(res);
                    }
                }
            }
        }
        std::cout << "\nBenchmark completo. Guardando CSV...\n";
        saveCSV(results, csv_path);
        printSummary(results);
    }

private:
    // Guardar resultados en CSV
    static void saveCSV(const std::vector<BenchmarkResult>& results,
                        const std::string& path) {
        std::ofstream f(path);
        f << "algorithm,distribution,n,U,mean_ms,std_ms,throughput_mps,memory_mb\n";
        for (auto& r : results) {
            f << r.algorithm << ","
              << r.distribution << ","
              << r.n << ","
              << r.U << ","
              << std::fixed << std::setprecision(4)
              << r.mean_ms << ","
              << r.std_ms << ","
              << r.throughput_mps << ","
              << r.memory_mb << "\n";
        }
        std::cout << "CSV guardado en: " << path << "\n";
    }

    // Imprimir resumen en consola
    static void printSummary(const std::vector<BenchmarkResult>& results) {
        std::cout << "\n"
                  << std::left
                  << std::setw(12) << "Algoritmo"
                  << std::setw(25) << "Distribución"
                  << std::setw(12) << "n"
                  << std::setw(10) << "Media(ms)"
                  << std::setw(10) << "StdDev"
                  << std::setw(14) << "Throughput"
                  << std::setw(12) << "Mem(MB)"
                  << "\n"
                  << std::string(90, '-') << "\n";

        for (auto& r : results) {
            std::cout << std::left
                      << std::setw(12) << r.algorithm
                      << std::setw(25) << r.distribution
                      << std::setw(12) << r.n
                      << std::setw(10) << std::fixed << std::setprecision(2) << r.mean_ms
                      << std::setw(10) << r.std_ms
                      << std::setw(14) << r.throughput_mps
                      << std::setw(12) << r.memory_mb
                      << "\n";
        }
    }
};
