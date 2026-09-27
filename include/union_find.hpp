#ifndef UNION_FIND_HPP_
#define UNION_FIND_HPP_

#include <cstddef>
#include <vector>

enum class UFVariant { Naive, PathCompression, Rank };

// Disjoint-set union. Variant selects optimisation level for benchmarking.
class UnionFind {
 public:
  explicit UnionFind(int n, UFVariant variant = UFVariant::Rank);

  int find(int x);
  bool unite(int a, int b);
  int components() const;

 private:
  UFVariant variant_;
  std::vector<int> parent_;
  std::vector<int> rank_;
  int components_;

  int findNaive(int x);
  int findCompress(int x);
};

#endif  // UNION_FIND_HPP_
