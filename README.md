# Tarea_1._PSP

# Solución: Suma de Arreglos en Paralelo con OpenMP

Este proyecto suma dos arreglos A y B para producir R, validando contra una versión serial.
La suma paralela se realiza con OpenMP usando un `parallel for`.

## Requisitos
- Compilador C++ (GCC o Clang) con soporte OpenMP
- CMake (opcional, recomendado)

## Compilar y ejecutar (CMake)
```bash
cmake -S . -B build
cmake --build build
./build/suma_arreglos
