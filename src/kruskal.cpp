#include "kruskal.hpp"

#include <algorithm>

#include "union_find.hpp"

int64_t kruskalMstWeightSorted(int n, const std::vector<Edge>& edges,
                               UFVariant ufVariant) {
  UnionFind uf(n, ufVariant);
  int64_t total = 0;
  int taken = 0;

  for (const Edge& e : edges) {
    if (uf.unite(e.u, e.v)) {
      total += e.w;
      if (++taken == n - 1) {
        break;
      }
    }
  }

  return total;
}

int64_t kruskalMstWeight(int n, std::vector<Edge> edges, UFVariant ufVariant) {
  std::sort(edges.begin(), edges.end(),
            [](const Edge& a, const Edge& b) { return a.w < b.w; });
  return kruskalMstWeightSorted(n, edges, ufVariant);
}
