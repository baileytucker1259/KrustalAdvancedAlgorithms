#include "graph.hpp"

#include <algorithm>
#include <random>
#include <set>
#include <stdexcept>
#include <utility>

std::vector<Edge> randomGraph(int n, int m, uint32_t seed) {
  if (n < 1) {
    throw std::invalid_argument("randomGraph: n must be positive");
  }
  const int maxEdges = n * (n - 1) / 2;
  if (m < 0 || m > maxEdges) {
    throw std::invalid_argument("randomGraph: m out of range");
  }

  std::mt19937 rng(seed);
  std::uniform_int_distribution<int64_t> weightDist(1, 1'000'000);
  std::set<std::pair<int, int>> seen;
  std::vector<Edge> edges;
  edges.reserve(static_cast<std::size_t>(m));

  while (static_cast<int>(edges.size()) < m) {
    const int u = std::uniform_int_distribution<int>(0, n - 1)(rng);
    const int v = std::uniform_int_distribution<int>(0, n - 1)(rng);
    if (u == v) {
      continue;
    }
    const int a = std::min(u, v);
    const int b = std::max(u, v);
    if (seen.insert({a, b}).second) {
      edges.push_back({a, b, weightDist(rng)});
    }
  }

  return edges;
}

std::vector<Edge> gridGraph(int side, int64_t weight) {
  if (side < 1) {
    throw std::invalid_argument("gridGraph: side must be positive");
  }

  const int n = side * side;
  std::vector<Edge> edges;
  edges.reserve(static_cast<std::size_t>(2 * n));

  auto index = [side](int r, int c) { return r * side + c; };

  for (int r = 0; r < side; ++r) {
    for (int c = 0; c < side; ++c) {
      const int u = index(r, c);
      if (c + 1 < side) {
        edges.push_back({u, index(r, c + 1), weight});
      }
      if (r + 1 < side) {
        edges.push_back({u, index(r + 1, c), weight});
      }
    }
  }

  return edges;
}

std::vector<Edge> completeGraph(int n, int64_t weight) {
  if (n < 1) {
    throw std::invalid_argument("completeGraph: n must be positive");
  }

  std::vector<Edge> edges;
  edges.reserve(static_cast<std::size_t>(n) * (n - 1) / 2);
  for (int u = 0; u < n; ++u) {
    for (int v = u + 1; v < n; ++v) {
      edges.push_back({u, v, weight});
    }
  }
  return edges;
}

std::vector<std::pair<int, int>> randomUniteSequence(int n, int m, uint32_t seed) {
  if (n < 1 || m < 0) {
    throw std::invalid_argument("randomUniteSequence: invalid n or m");
  }

  std::mt19937 rng(seed);
  std::vector<std::pair<int, int>> ops;
  ops.reserve(static_cast<std::size_t>(m));

  for (int i = 0; i < m; ++i) {
    const int a = std::uniform_int_distribution<int>(0, n - 1)(rng);
    const int b = std::uniform_int_distribution<int>(0, n - 1)(rng);
    ops.emplace_back(a, b);
  }
  return ops;
}

const char* graphFamilyName(GraphFamily family) {
  switch (family) {
    case GraphFamily::Random:
      return "random";
    case GraphFamily::Grid:
      return "grid";
    case GraphFamily::Complete:
      return "complete";
  }
  return "unknown";
}
