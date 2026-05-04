#include "Vector2D.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>
#include <iostream>

// Initialize static member
size_t Vector2D::_vector_count = 0;

Vector2D::Vector2D() : Vector2D(0.0, 0.0) {}

Vector2D::Vector2D(double x, double y) : 
  Vector2D("vec_" + std::to_string(++_vector_count), x, y) {}

Vector2D::Vector2D(const std::string label, double x, double y)
    : _x(x), _y(y), _label(label) {}

double Vector2D::x() const { return _x; }

double Vector2D::y() const { return _y; }

std::string Vector2D::label() const { return _label; }

double Vector2D::norm() const { return std::hypot(_x, _y); }

double Vector2D::distance_to(const Vector2D &other) const {
  return std::hypot(_x - other._x, _y - other._y);
}

Vector2D Vector2D::operator+(const Vector2D &other) const {
  return Vector2D(_x + other._x, _y + other._y);
}

Vector2D Vector2D::operator-(const Vector2D &other) const {
  return Vector2D(_x - other._x, _y - other._y);
}

Vector2D Vector2D::operator*(double scalar) const {
  return Vector2D(_x * scalar, _y * scalar);
}

void Vector2D::set(double x, double y) {
  _x = x;
  _y = y;
}

std::string Vector2D::to_csv_row() const {
  std::ostringstream oss;
  oss << _label << ',' << std::setprecision(10) << _x << ',' << _y;
  return oss.str();
}

std::string Vector2D::to_csv_header() { return "label,x,y"; }

std::ostream &operator<<(std::ostream &os, const Vector2D &v) {
  os << "[" << v._label << "] (" << v._x << ", " << v._y << ")";
  return os;
}
