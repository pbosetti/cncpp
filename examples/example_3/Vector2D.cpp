#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class Vector2D {
public:
  static size_t instance_count;

  Vector2D(double x, double y) : x(x), y(y) {
    ++Vector2D::instance_count;
    _label = "v" + std::to_string(Vector2D::instance_count);
  }
  Vector2D(std::string const &label, double x, double y) : Vector2D(x, y) {
    _label = label;
  }
  Vector2D() : Vector2D(0.0, 0.0) {}

  void set_label(std::string const &label) { _label = label; }

  void set(double x, double y) {
    this->x = x;
    this->y = y;
  }

  void print(std::ostream &os = std::cout) const {
    os << _label << ": (" << x << ", " << y << ")\n";
  }

  static void to_csv_header(std::ostream &os = std::cout) {
    os << "label,x,y,norm\n";
  }

  void to_csv_row(std::ostream &os = std::cout) const {
    os << _label << "," << x << "," << y << "," << norm() << "\n";
  }

  void scale(double factor = 1.0) {
    x *= factor;
    y *= factor;
  }

  double norm() const { return std::hypot(x, y); }
  
  void add(const Vector2D &other) {
    x += other.x;
    y += other.y;
  }
  
  double x, y;

private:
  std::string _label{""};
};

size_t Vector2D::instance_count = 0;

int main(int argc, char *argv[]) {
  std::vector<Vector2D> vectors;
  vectors.emplace_back("v1", 1.0, 2.0);
  vectors.emplace_back(3.0, 4.0);
  vectors.emplace_back("v3", 5.0, 6.0);
  vectors.emplace_back("v4", 7.0, 8.0);

  std::cout << "Vector2D objects:\n";
  Vector2D::to_csv_header();
  for (auto v = vectors.begin(); v != vectors.end(); ++v) {
    v->to_csv_row();
  }

  std::cout << "\nScaled Vector2D objects (scaled by 2):\n";
  Vector2D::to_csv_header();
  for (auto &v : vectors) {
    v.scale(2.0);
    v.to_csv_row();
  }

  return 0;
}