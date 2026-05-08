# ADA_PF_GARCIA_JIMENEZ_USUGA

Solución de la práctica final integradora de **Análisis de Algoritmos**. El proyecto procesa el dataset Telco Customer Churn y aplica tres paradigmas:

- **Módulo A - Divide y Vencerás:** parser del CSV, MergeSort estable por `tenure` descendente, búsqueda binaria recursiva y medición empírica.
- **Módulo B - Codicioso:** construcción determinística del grafo completo de 20 nodos y MST con Kruskal + Union-Find.
- **Módulo C - Programación Dinámica:** Mochila 0-1 por tabulación, backtracking y contraejemplo del enfoque codicioso por razón valor/peso.

## Integrantes y módulos

- Módulo A: Mateo Usuga Vargas
- Módulo B: Juan José García Urrego
- Módulo C: Juan Esteban Jiménez Cardona

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
g++ -std=c++17 -O2 -o ada_pf src/main.cpp src/*.cpp
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

La ejecución crea o actualiza los archivos de salida dentro de `results/`:

- `results/solicitudes_ordenadas.csv`
- `results/busquedas_A.txt`
- `results/tiempos_A.txt` (archivo adicional para dejar trazabilidad de tiempos)
- `results/mst_red.txt`
- `results/asignacion_bw.txt`

> Nota técnica: el comando exigido por el enunciado incluye `src/main.cpp` y también `src/*.cpp`; en shells como Bash eso puede compilar `main.cpp` dos veces. Para que el comando literal funcione, `main` está marcado como símbolo débil cuando se compila con `g++`/`clang++`.

## Estructura

```text
ADA_PF_GARCIA_JIMENEZ_USUGA/
├── README.md
├── data/
│   └── WA_Fn-UseC_-Telco-Customer-Churn.csv
├── results/
│   ├── solicitudes_ordenadas.csv
│   ├── busquedas_A.txt
│   ├── tiempos_A.txt
│   ├── mst_red.txt
│   └── asignacion_bw.txt
├── src/
│   ├── parser.cpp / parser.hpp
│   ├── mergesort.cpp / mergesort.hpp
│   ├── binary_search.cpp / binary_search.hpp
│   ├── graph.cpp / graph.hpp
│   ├── kruskal.cpp / kruskal.hpp
│   ├── knapsack.cpp / knapsack.hpp
│   └── main.cpp
└── report/
    └── Informe.pdf
```

## Decisiones documentadas

1. **Mochila 0-1:** con la especificación literal, las 50 solicitudes activas de mayor `tenure` tienen `TotalCharges` redondeado mayor que `W = 500`; por tanto, la solución óptima literal es vacía y no existe contraejemplo codicioso dentro de ese conjunto. `asignacion_bw.txt` conserva ese resultado literal y añade una ejecución complementaria reproducible con las primeras 50 solicitudes activas cuyo peso sí cabe en la capacidad, solo para evidenciar el fallo codicioso exigido por la rúbrica.
