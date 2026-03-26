// Vector2D class definition
// Implement a simple 2D vector class with basic operations and CSV output
// functionality.
#pragma once

#include <ostream>
#include <string>

class Vector2D {
public:
  /**
   * @brief Default constructor. Initializes vector to (0, 0) with empty label.
   */
  Vector2D();

  /**
   * @brief Construct a vector with given x and y values.
   * @param x X coordinate
   * @param y Y coordinate
   */
  Vector2D(double x, double y);

  /**
   * @brief Construct a vector with a label and given x and y values.
   * @param label Label for the vector
   * @param x X coordinate
   * @param y Y coordinate
   */
  Vector2D(const std::string label, double x, double y);

  /**
   * @brief Get the X coordinate.
   * @return X value
   */
  double x() const;

  /**
   * @brief Get the Y coordinate.
   * @return Y value
   */
  double y() const;

  /**
   * @brief Get the label of the vector.
   * @return Label string
   */
  std::string label() const;

  /**
   * @brief Compute the Euclidean norm (length) of the vector.
   * @return Norm of the vector
   */
  double norm() const;

  /**
   * @brief Compute the Euclidean distance to another vector.
   * @param other The other vector
   * @return Distance between this and other
   */
  double distance_to(const Vector2D &other) const;

  /**
   * @brief Add another vector to this one.
   * @param other The vector to add
   * @return Sum of the two vectors
   */
  Vector2D operator+(const Vector2D &other) const;

  /**
   * @brief Subtract another vector from this one.
   * @param other The vector to subtract
   * @return Difference of the two vectors
   */
  Vector2D operator-(const Vector2D &other) const;

  /**
   * @brief Multiply this vector by a scalar.
   * @param scalar The scalar value
   * @return Scaled vector
   */
  Vector2D operator*(double scalar) const;

  /**
   * @brief Set the X and Y coordinates.
   * @param x New X value
   * @param y New Y value
   */
  void set(double x, double y);

  /**
   * @brief Convert the vector to a CSV row string.
   * @return CSV-formatted string
   */
  std::string to_csv_row() const;

  /**
   * @brief Get the CSV header for vector data.
   * @return CSV header string
   */
  static std::string to_csv_header();

  /**
   * @brief Output stream operator for Vector2D.
   * @param os Output stream
   * @param v Vector to output
   * @return Reference to output stream
   */
  friend 
  std::ostream &operator<<(std::ostream &os, const Vector2D &v);

private:
  double _x;
  double _y;
  std::string _label;
  static size_t _vector_count;
};

std::ostream &operator<<(std::ostream &os, const Vector2D &v);
