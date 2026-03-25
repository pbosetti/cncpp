// Vector2D class definition
// Implement a simple 2D vector class with basic operations and CSV output
// functionality.
#pragma once

#include <ostream>
#include <string>

class Vector2D {
public:
  Vector2D();
  Vector2D(double x, double y);
  Vector2D(const std::string label, double x, double y);

  double x() const;
  double y() const;
  std::string label() const;
  double norm() const;
  double distance_to(const Vector2D &other) const;
  Vector2D operator+(const Vector2D &other) const;
  Vector2D operator-(const Vector2D &other) const;
  Vector2D operator*(double scalar) const;
  void set(double x, double y);
  std::string to_csv_row() const;

  // Static members
  // call it like this: Vector2D::to_csv_header()
  static std::string to_csv_header();

private:
  double _x;
  double _y;
  std::string _label;
  static size_t _vector_count;
};

std::ostream &operator<<(std::ostream &os, const Vector2D &v);
