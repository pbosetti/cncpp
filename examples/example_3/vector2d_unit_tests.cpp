#include "Vector2D.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr double kEps = 1e-12;

bool approx_equal(double a, double b, double eps = kEps) {
  return std::fabs(a - b) <= eps;
}

bool starts_with(const std::string &value, const std::string &prefix) {
  return value.rfind(prefix, 0) == 0;
}

std::vector<std::string> split_by_comma(const std::string &value) {
  std::vector<std::string> parts;
  std::stringstream ss(value);
  std::string item;
  while (std::getline(ss, item, ',')) {
    parts.push_back(item);
  }
  return parts;
}

bool require(bool condition, const std::string &message) {
  if (!condition) {
    std::cerr << "[FAIL] " << message << '\n';
    return false;
  }
  return true;
}

} // namespace

int main() {
  int failures = 0;

  {
    const Vector2D v;
    failures += !require(approx_equal(v.x(), 0.0), "default constructor x == 0");
    failures += !require(approx_equal(v.y(), 0.0), "default constructor y == 0");
    failures += !require(starts_with(v.label(), "vec_"), "default constructor auto-label starts with vec_");
  }

  {
    const Vector2D v(3.0, 4.0);
    failures += !require(approx_equal(v.x(), 3.0), "(x,y) constructor stores x");
    failures += !require(approx_equal(v.y(), 4.0), "(x,y) constructor stores y");
    failures += !require(starts_with(v.label(), "vec_"), "(x,y) constructor auto-label starts with vec_");
    failures += !require(approx_equal(v.norm(), 5.0), "norm() computes Euclidean norm");
  }

  {
    const Vector2D v("custom", 1.5, -2.5);
    failures += !require(v.label() == "custom", "(label,x,y) constructor keeps custom label");
    failures += !require(approx_equal(v.x(), 1.5), "(label,x,y) constructor stores x");
    failures += !require(approx_equal(v.y(), -2.5), "(label,x,y) constructor stores y");
  }

  {
    Vector2D v;
    v.set(-3.25, 8.5);
    failures += !require(approx_equal(v.x(), -3.25), "set() updates x");
    failures += !require(approx_equal(v.y(), 8.5), "set() updates y");
  }

  {
    const Vector2D a(1.0, 2.0);
    const Vector2D b(3.0, 4.0);

    const Vector2D sum = a + b;
    failures += !require(approx_equal(sum.x(), 4.0), "operator+ x");
    failures += !require(approx_equal(sum.y(), 6.0), "operator+ y");

    const Vector2D diff = a - b;
    failures += !require(approx_equal(diff.x(), -2.0), "operator- x");
    failures += !require(approx_equal(diff.y(), -2.0), "operator- y");

    const Vector2D scaled = a * 2.5;
    failures += !require(approx_equal(scaled.x(), 2.5), "operator* x");
    failures += !require(approx_equal(scaled.y(), 5.0), "operator* y");
  }

  {
    const Vector2D a(0.0, 0.0);
    const Vector2D b(3.0, 4.0);
    failures += !require(approx_equal(a.distance_to(b), 5.0), "distance_to() computes Euclidean distance");
    failures += !require(approx_equal(a.distance_to(b), b.distance_to(a)), "distance_to() is symmetric");
  }

  {
    const Vector2D v("row_label", 1.25, -2.5);
    failures += !require(Vector2D::to_csv_header() == "label,x,y", "to_csv_header() format");

    const std::vector<std::string> parts = split_by_comma(v.to_csv_row());
    failures += !require(parts.size() == 3, "to_csv_row() has 3 fields");
    if (parts.size() == 3) {
      failures += !require(parts[0] == "row_label", "to_csv_row() stores label");
      failures += !require(approx_equal(std::stod(parts[1]), 1.25), "to_csv_row() stores x");
      failures += !require(approx_equal(std::stod(parts[2]), -2.5), "to_csv_row() stores y");
    }
  }

  {
    const Vector2D v("print_me", 1.0, -2.0);
    std::ostringstream os;
    os << v;
    failures += !require(os.str() == "[print_me] (1, -2)", "operator<< output format");
  }

  if (failures == 0) {
    std::cout << "All Vector2D unit tests passed" << std::endl;
    return 0;
  }

  std::cerr << failures << " test(s) failed" << std::endl;
  return 1;
}
