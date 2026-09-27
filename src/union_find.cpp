#include "union_find.hpp"

#include <algorithm>
#include <numeric>

UnionFind::UnionFind(int n, UFVariant variant)
    : variant_(variant),
      parent_(n),
      rank_(n, 0),
      components_(n) {
  std::iota(parent_.begin(), parent_.end(), 0);
}

int UnionFind::findNaive(int x) {
  while (parent_[x] != x) {
    x = parent_[x];
  }
  return x;
}

int UnionFind::findCompress(int x) {
  if (parent_[x] != x) {
    parent_[x] = findCompress(parent_[x]);
  }
  return parent_[x];
}

int UnionFind::find(int x) {
  switch (variant_) {
    case UFVariant::Naive:
      return findNaive(x);
    case UFVariant::PathCompression:
    case UFVariant::Rank:
      return findCompress(x);
  }
  return findCompress(x);
}

bool UnionFind::unite(int a, int b) {
  a = find(a);
  b = find(b);
  if (a == b) {
    return false;
  }

  if (variant_ == UFVariant::Rank) {
    if (rank_[a] < rank_[b]) {
      std::swap(a, b);
    }
    parent_[b] = a;
    if (rank_[a] == rank_[b]) {
      ++rank_[a];
    }
  } else {
    parent_[b] = a;
  }

  --components_;
  return true;
}

int UnionFind::components() const { return components_; }
