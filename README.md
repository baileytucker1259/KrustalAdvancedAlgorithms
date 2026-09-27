# PA1: Union-Find and Kruskal (Track A)

Empirical comparison of three Union-Find variants, including their effect on Kruskal's algorithm.

The written report is `Kruskal Algorithm Report.docx`. The walkthrough is `Kruskal.mp4`. Measured timings are in `results/results.csv`, with plots in `results/`.

## Build and run

Tested with g++ 13.2.0 on Windows.

```powershell
g++ -std=c++23 -Wall -Wextra -O2 -I include src/union_find.cpp src/kruskal.cpp src/graph.cpp tests/test_union_find.cpp -o test_uf.exe
.\test_uf.exe

g++ -std=c++23 -Wall -Wextra -O2 -I include src/union_find.cpp src/kruskal.cpp src/graph.cpp bench/bench_main.cpp -o bench.exe
.\bench.exe > results\results.csv

python bench/plot_results.py results\results.csv
```

`bench.exe` prints CSV to stdout. Each row is the median of 10 timed runs after one warmup, seed 42.

Plotting needs `pandas` and `matplotlib`.

## Layout

```
include/                         union_find.hpp, kruskal.hpp, graph.hpp
src/                             implementations
tests/                           small correctness checks
bench/                           timing driver and plot script
results/                         results.csv and png plots
Kruskal Algorithm Report.docx    Track A write-up
Kruskal.mp4                      video walkthrough
```

## Variants

- `Naive`: no path compression, link the second root under the first
- `PathCompression`: compress on find, same link rule
- `Rank`: path compression and union by rank
