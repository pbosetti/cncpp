#include "Vector2D.hpp"
#include <iostream>
#include <vector>

int main() {
  Vector2D v1(3.0, 4.0);
  Vector2D v2("custom_vec", 1.0, 2.0);

  std::cout << "v1: " << v1 << std::endl;
  std::cout << "v2: " << v2 << std::endl;

  std::cout << "Norm of v1: " << v1.norm() << std::endl;
  std::cout << "Distance from v1 to v2: " << v1.distance_to(v2) << std::endl;

  Vector2D v3 = v1 + v2;
  std::cout << "v1 + v2: " << v3 << std::endl;

  Vector2D v4 = v1 - v2;
  std::cout << "v1 - v2: " << v4 << std::endl;

  Vector2D v5 = v1 * 2.0;
  std::cout << "v1 * 2: " << v5 << std::endl;

  // Output CSV header and rows
  std::cout << Vector2D::to_csv_header() << std::endl;
  std::cout << v1.to_csv_row() << std::endl;
  std::cout << v2.to_csv_row() << std::endl;

  // Create a std::vector of Vector2d and print a CSV file
  std::vector<Vector2D> vv;
  for (int i = 0; i < 10; i++) {
    vv.push_back(Vector2D(i, i*i));
  }

  std::cout << std::endl << "CSV list of vectors:" << std::endl;
  std::cout << Vector2D::to_csv_header() << std::endl;
  for (auto &v : vv) {
    std::cout << v.to_csv_row() << std::endl;
    v.set(0, 0);
  }

  for (auto v = vv.begin(); v < vv.end(); v++) {
    std::cout << v->to_csv_row() << std::endl;
  }

  return 0;
}