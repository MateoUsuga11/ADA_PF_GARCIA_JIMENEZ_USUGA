# ADA_PF_GARCIA_JIMENEZ_USUGA

Solución de la práctica final de Análisis de Algoritmos, aplicando Divide y Vencerás, algoritmos codiciosos y programación dinámica sobre el dataset Telco Customer Churn.

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

La ejecución genera los archivos de salida dentro de `results/`.

## Archivos implementados para el Módulo B

- `src/graph.hpp` / `src/graph.cpp`: construcción determinística del grafo completo de 20 nodos y 190 aristas.
- `src/kruskal.hpp` / `src/kruskal.cpp`: algoritmo de Kruskal con Union-Find, unión por rango y compresión de caminos.
- `results/mst_red.txt`: reporte del grafo, MST, justificación con el lema del ciclo y verificación del subgrafo inducido por los primeros 5 nodos.
