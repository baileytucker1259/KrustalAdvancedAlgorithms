#ifndef KRUSKAL_HPP_
#define KRUSKAL_HPP_

#include <cstdint>
#include <utility>
#include <vector>

struct Edge {
  int u;
  int v;
  int64_t w;
};

#include "union_find.hpp"

// Minimum spanning tree weight. Edges must already be sorted by weight.
int64_t kruskalMstWeightSorted(int n, const std::vector<Edge>& edges,
                               UFVariant ufVariant = UFVariant::Rank);

// Sorts a copy of the edge list, then runs Kruskal.
int64_t kruskalMstWeight(int n, std::vector<Edge> edges,
                         UFVariant ufVariant = UFVariant::Rank);

#endif  // KRUSKAL_HPP_
