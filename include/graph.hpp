#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <cstdint>
#include <vector>

#include "kruskal.hpp"

enum class GraphFamily { Random, Grid, Complete };

// Erdős–Rényi-style random graph with exactly m undirected edges (u < v).
std::vector<Edge> randomGraph(int n, int m, uint32_t seed);

// side × side grid; n = side * side, m ≈ 2n.
std::vector<Edge> gridGraph(int side, int64_t weight = 1);

// Complete graph K_n; m = n(n-1)/2.
std::vector<Edge> completeGraph(int n, int64_t weight = 1);

// m random unite operations on n elements (for Union-Find benchmarks).
std::vector<std::pair<int, int>> randomUniteSequence(int n, int m, uint32_t seed);

const char* graphFamilyName(GraphFamily family);

#endif  // GRAPH_HPP_
