#include "Vector2D.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

namespace {

constexpr std::size_t kVectorCount = 100;
constexpr double kMinSumNorm = 1.0;
constexpr long long kMaxDurationMs = 5000;
constexpr double kEps = 1e-12;

bool approx_equal(double a, double b, double eps = kEps) {
  return std::fabs(a - b) <= eps;
}

} // namespace

int main() {
  using clock = std::chrono::steady_clock;
  const auto start = clock::now();

  std::vector<Vector2D> vectors;
  vectors.reserve(kVectorCount);

  for (std::size_t i = 0; i < kVectorCount; ++i) {
    const double x = static_cast<double>(i);
    const double y = static_cast<double>(i) * 0.5;
    vectors.emplace_back(x, y);
  }

  Vector2D accumulator(0.0, 0.0);
  double sum_norms = 0.0;
  double sum_distances = 0.0;

  for (const Vector2D &element : vectors) {
    accumulator = accumulator + element;
    sum_norms += element.norm();
  }

  for (std::size_t i = 1; i < vectors.size(); ++i) {
    sum_distances += vectors[i - 1].distance_to(vectors[i]);
  }

  const auto stop = clock::now();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  if (vectors.size() != kVectorCount) {
    std::cerr << "[FAIL] unexpected vector count: " << vectors.size() << '\n';
    return 1;
  }

  if (!(sum_norms > kMinSumNorm)) {
    std::cerr << "[FAIL] sum of norms should be positive, got: " << sum_norms << '\n';
    return 1;
  }

  if (!approx_equal(vectors.front().distance_to(vectors.front()), 0.0)) {
    std::cerr << "[FAIL] distance of vector from itself should be 0" << '\n';
    return 1;
  }

  if (!(sum_distances > 0.0)) {
    std::cerr << "[FAIL] accumulated neighbor distances should be positive" << '\n';
    return 1;
  }

  if (elapsed.count() > kMaxDurationMs) {
    std::cerr << "[FAIL] smoke test exceeded time limit: " << elapsed.count() << " ms" << '\n';
    return 1;
  }

  std::cout << "Smoke test passed in " << elapsed.count() << " ms"
            << " (accumulator=" << accumulator << ", sum_norms=" << sum_norms
            << ", sum_distances=" << sum_distances << ")" << std::endl;
  return 0;
}
