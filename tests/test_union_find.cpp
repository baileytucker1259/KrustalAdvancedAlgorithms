#include <cassert>
#include <iostream>
#include <vector>

#include "kruskal.hpp"
#include "union_find.hpp"

static void testUnionFindVariants() {
  for (UFVariant v : {UFVariant::Naive, UFVariant::PathCompression,
                      UFVariant::Rank}) {
    UnionFind uf(5, v);
    uf.unite(0, 1);
    uf.unite(2, 3);
    uf.unite(1, 2);
    assert(uf.find(0) == uf.find(3));
    assert(uf.components() == 2);
  }
  std::cout << "union_find variants OK\n";
}

static void testKruskalTriangle() {
  // 0-1-2 triangle, weights 1,2,3 -> MST weight 3
  std::vector<Edge> edges = {{0, 1, 1}, {1, 2, 2}, {0, 2, 3}};
  for (UFVariant v : {UFVariant::Naive, UFVariant::PathCompression,
                      UFVariant::Rank}) {
    assert(kruskalMstWeight(3, edges, v) == 3);
  }
  std::cout << "kruskal triangle OK\n";
}

static void testKruskalLectureGraph() {
  // Week 5 lecture graph, A..G = 0..6. MST weight 27.
  std::vector<Edge> edges = {
      {0, 1, 4},  {0, 2, 3},  {1, 2, 2}, {1, 3, 7}, {2, 3, 5}, {2, 4, 8},
      {3, 4, 1},  {3, 5, 9},  {4, 5, 6}, {4, 6, 11}, {5, 6, 10}};
  for (UFVariant v : {UFVariant::Naive, UFVariant::PathCompression,
                      UFVariant::Rank}) {
    assert(kruskalMstWeight(7, edges, v) == 27);
  }
  std::cout << "kruskal lecture graph OK\n";
}

int main() {
  testUnionFindVariants();
  testKruskalTriangle();
  testKruskalLectureGraph();
  std::cout << "all tests passed\n";
  return 0;
}
