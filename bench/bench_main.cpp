#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "kruskal.hpp"
#include "union_find.hpp"

namespace {

const char* variantName(UFVariant variant) {
  switch (variant) {
    case UFVariant::Naive:
      return "Naive";
    case UFVariant::PathCompression:
      return "PathCompression";
    case UFVariant::Rank:
      return "Rank";
  }
  return "Unknown";
}

int64_t median(std::vector<int64_t> values) {
  if (values.empty()) {
    return 0;
  }
  const std::size_t mid = values.size() / 2;
  std::nth_element(values.begin(), values.begin() + static_cast<std::ptrdiff_t>(mid),
                   values.end());
  return values[mid];
}

int64_t timeUnionFind(int n, const std::vector<std::pair<int, int>>& ops,
                      UFVariant variant) {
  UnionFind uf(n, variant);
  const auto start = std::chrono::steady_clock::now();
  for (const auto& [a, b] : ops) {
    uf.unite(a, b);
    uf.find(a);
    uf.find(b);
  }
  const auto end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

int64_t timeKruskal(int n, const std::vector<Edge>& edges, UFVariant variant) {
  const auto start = std::chrono::steady_clock::now();
  (void)kruskalMstWeight(n, edges, variant);
  const auto end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

// Sort is identical for every Union-Find variant, so it is timed separately
// from the scan that actually calls unite.
struct KruskalParts {
  int64_t sortNs;
  int64_t ufNs;
};

KruskalParts timeKruskalParts(int n, const std::vector<Edge>& edges,
                              UFVariant variant) {
  std::vector<Edge> sorted = edges;
  const auto sortStart = std::chrono::steady_clock::now();
  std::sort(sorted.begin(), sorted.end(),
            [](const Edge& a, const Edge& b) { return a.w < b.w; });
  const auto sortEnd = std::chrono::steady_clock::now();
  (void)kruskalMstWeightSorted(n, sorted, variant);
  const auto ufEnd = std::chrono::steady_clock::now();
  return {
      std::chrono::duration_cast<std::chrono::nanoseconds>(sortEnd - sortStart)
          .count(),
      std::chrono::duration_cast<std::chrono::nanoseconds>(ufEnd - sortEnd)
          .count(),
  };
}

void printCsvHeader() {
  std::cout << "benchmark,variant,family,n,m,median_ns,runs,seed\n";
}

void printCsvRow(const std::string& benchmark, UFVariant variant,
                 const std::string& family, int n, int m, int64_t medianNs,
                 int runs, uint32_t seed) {
  std::cout << benchmark << ',' << variantName(variant) << ',' << family << ','
            << n << ',' << m << ',' << medianNs << ',' << runs << ',' << seed
            << '\n';
}

void benchUnionFind(int n, int m, int runs, uint32_t seed) {
  const auto ops = randomUniteSequence(n, m, seed);
  for (UFVariant variant :
       {UFVariant::Naive, UFVariant::PathCompression, UFVariant::Rank}) {
    std::vector<int64_t> samples;
    samples.reserve(static_cast<std::size_t>(runs));
    (void)timeUnionFind(n, ops, variant);
    for (int r = 0; r < runs; ++r) {
      samples.push_back(timeUnionFind(n, ops, variant));
    }
    printCsvRow("uf_unite", variant, "sequence", n, m, median(samples), runs, seed);
  }
}

void benchKruskalFamily(GraphFamily family, int n, int m, int runs,
                        uint32_t seed) {
  std::vector<Edge> edges;
  switch (family) {
    case GraphFamily::Random:
      edges = randomGraph(n, m, seed);
      break;
    case GraphFamily::Grid: {
      const int side = static_cast<int>(std::sqrt(static_cast<double>(n)));
      edges = gridGraph(side);
      n = side * side;
      m = static_cast<int>(edges.size());
      break;
    }
    case GraphFamily::Complete:
      edges = completeGraph(n);
      m = static_cast<int>(edges.size());
      break;
  }

  for (UFVariant variant :
       {UFVariant::Naive, UFVariant::PathCompression, UFVariant::Rank}) {
    std::vector<int64_t> totalSamples;
    std::vector<int64_t> sortSamples;
    std::vector<int64_t> ufSamples;
    totalSamples.reserve(static_cast<std::size_t>(runs));
    sortSamples.reserve(static_cast<std::size_t>(runs));
    ufSamples.reserve(static_cast<std::size_t>(runs));

    (void)timeKruskal(n, edges, variant);
    for (int r = 0; r < runs; ++r) {
      totalSamples.push_back(timeKruskal(n, edges, variant));
      const KruskalParts parts = timeKruskalParts(n, edges, variant);
      sortSamples.push_back(parts.sortNs);
      ufSamples.push_back(parts.ufNs);
    }
    const std::string familyName = graphFamilyName(family);
    printCsvRow("kruskal", variant, familyName, n, m, median(totalSamples), runs,
                seed);
    printCsvRow("kruskal_sort", variant, familyName, n, m, median(sortSamples),
                runs, seed);
    printCsvRow("kruskal_uf", variant, familyName, n, m, median(ufSamples), runs,
                seed);
  }
}

}  // namespace

int main() {
  const int runs = 10;
  const uint32_t seed = 42;

  printCsvHeader();

  // Union-Find: vary n with m = 4n unite+find batches
  for (int n : {500, 1'000, 2'000, 5'000, 10'000}) {
    benchUnionFind(n, 4 * n, runs, seed);
  }

  // Kruskal on sparse random graphs (m ≈ 2n)
  for (int n : {500, 1'000, 2'000, 5'000}) {
    benchKruskalFamily(GraphFamily::Random, n, 2 * n, runs, seed);
  }

  // Grid graphs — structured sparse
  for (int side : {22, 32, 45, 63, 89}) {
    benchKruskalFamily(GraphFamily::Grid, side * side, 0, runs, seed);
  }

  // Complete graphs — stress UF inside Kruskal (keep n modest)
  for (int n : {100, 150, 200, 250, 300}) {
    benchKruskalFamily(GraphFamily::Complete, n, 0, runs, seed);
  }

  return 0;
}
