#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <omp.h>

int main() {
    int N = 100000;              // usamos 100000 para probar performance
    int requested_threads = 2;   // <-- HARD CODED: cambiar a 2, 4, 8 ,etc para distintas ejecuciones

    std::cout << "N (tamano del arreglo) = " << N << "\n";
    std::cout << "Hilos fijados: " << requested_threads << "\n";

    //Fija el número de hilos para OpenMP
    omp_set_num_threads(requested_threads);

    std::vector<int> A(N), B(N), R(N), R_serial(N);

    // Random: enteros 0-99
    std::mt19937 rng(12345); //semilla fija para reproducibilidad
    std::uniform_int_distribution<int> dist(0, 99);

    for (int i = 0; i < N; i++) {
        A[i] = dist(rng);
        B[i] = dist(rng);
    }

    // --- Serial ---
    double t0 = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        R_serial[i] = A[i] + B[i];
    }
    double t1 = omp_get_wtime();

    // --- Paralelo ---
    int threads_used = 0;
    double t2 = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        threads_used = omp_get_num_threads();

        #pragma omp for
        for (int i = 0; i < N; i++) {
            R[i] = A[i] + B[i];
        }
    }

    double t3 = omp_get_wtime();

    // Validación simple
    bool ok = true;
    for (int i = 0; i < N; i++) {
        if (R[i] != R_serial[i]) {
            ok = false;
            std::cout << "ERROR en i=" << i << " R=" << R[i]
                      << " serial=" << R_serial[i] << "\n";
            break;
        }
    }

    // Tiempos
    double serial_s   = (t1 - t0);
    double parallel_s = (t3 - t2);

    double serial_ms   = serial_s * 1000.0;
    double parallel_ms = parallel_s * 1000.0;

    //Throughput (elementos por segundo)
    double serial_eps   = (serial_s > 0)   ? (static_cast<double>(N) / serial_s)   : 0.0;
    double parallel_eps = (parallel_s > 0) ? (static_cast<double>(N) / parallel_s) : 0.0;

    std::cout << "\nThreads usados: " << threads_used << "\n";
    std::cout << "Tiempo serial   : " << serial_ms << " ms\n";
    std::cout << "Tiempo paralelo : " << parallel_ms << " ms\n";

    if (parallel_s > 0) {
        std::cout << "Speedup aprox   : " << (serial_s / parallel_s) << "x\n";
    }

    std::cout << "Throughput serial   : " << serial_eps << " elem/s\n";
    std::cout << "Throughput paralelo : " << parallel_eps << " elem/s\n";

    std::cout << "Validacion (R == R_serial): " << (ok ? "OK" : "FALLO") << "\n\n";

    //Imprimir los primeros 10 elementos (o menos si N<10)
    int show = std::min(N, 10);
    std::cout << "i |  A  +  B  =  R\n";
    std::cout << "--------------------\n";
    for (int i = 0; i < show; i++) {
        std::cout << std::setw(2) << i << " | "
                  << std::setw(2) << A[i] << " + "
                  << std::setw(2) << B[i] << " = "
                  << std::setw(3) << R[i] << "\n";
    }

    return 0;
}



