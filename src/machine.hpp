#pragma once

#include "defines.hpp"
#include "point.hpp"
#include <nlohmann/json.hpp>

namespace cncpp {

/**
 * @brief Represents the configuration and state of a CNC machine.
 *
 * A machine stores physical limits, quantization settings, coordinate offsets,
 * and the current commanded and actual positions. Configuration values can be
 * loaded from a JSON object or from a JSON file.
 */
class Machine : public Object {
  public:
  // LIFECYCLE =================================================================
  /**
   * @brief Constructs a machine with default configuration values.
   */
  Machine() = default;

  /**
   * @brief Constructs a machine and loads configuration from JSON data.
   * @param j JSON object containing machine configuration values.
   */
  Machine(nlohmann::json &j);

  /**
   * @brief Constructs a machine and loads configuration from a JSON file.
   * @param filename Path to the JSON configuration file.
   */
  Machine(std::string &filename);

  /**
   * @brief Destroys the machine instance.
   */
  ~Machine() = default; 

  /**
   * @brief Returns a human-readable description of the machine configuration.
   * @param colored Enables colored formatting in nested descriptions.
   * @return Text description of the configured machine parameters.
   */
  std::string desc(bool colored = true) const override;
  
  // OPERATIONS/OPERATORS ======================================================
  /**
   * @brief Loads machine configuration values from JSON data.
   * @param j JSON object containing optional machine parameters.
   *
   * Recognized keys are `tq`, `A`, `fmax`, `max_error`, `zero`, and `offset`.
   * Invalid value types raise std::runtime_error.
   */
  void load(nlohmann::json &j);

  /**
   * @brief Loads machine configuration values from a JSON file.
   * @param filename Path to the JSON configuration file.
   */
  void load(std::string &filename);

  /**
   * @brief Quantizes a time value to the next machine sampling instant.
   * @param t Time value to quantize.
   * @param dq Receives the positive quantization delta.
   * @return Quantized time value.
   */
  data_t quantize(data_t t, data_t &dq) const;


  // ACCESSORS =================================================================
  /**
   * @brief Gets the maximum acceleration.
   * @return Maximum acceleration value.
   */
  data_t A() const { return _A; }

  /**
   * @brief Gets the machine sampling period.
   * @return Quantization period.
   */
  data_t tq() const { return _tq; }

  /**
   * @brief Gets the maximum feed rate.
   * @return Maximum feed rate.
   */
  data_t fmax() const { return _fmax; }

  /**
   * @brief Gets the current position error.
   * @return Current position error.
   */
  data_t error() const { return _error; }

  /**
   * @brief Gets the maximum allowed position error.
   * @return Maximum allowed error.
   */
  data_t max_error() const { return _max_error; }

  /**
   * @brief Gets the machine zero point.
   * @return Machine zero point.
   */
  Point zero() const { return _zero; }

  /**
   * @brief Gets the coordinate offset.
   * @return Coordinate offset point.
   */
  Point offset() const { return _offset; }

  /**
   * @brief Gets the current setpoint.
   * @return Commanded setpoint position.
   */
  Point setpoint() const { return _setpoint; }

  /**
   * @brief Sets the current setpoint.
   * @param p New setpoint position.
   * @return Updated setpoint position.
   */
  Point setpoint(Point p) { return _setpoint = p; }

  /**
   * @brief Sets the current setpoint from coordinate values.
   * @param x New x coordinate.
   * @param y New y coordinate.
   * @param z New z coordinate.
   * @return Updated setpoint position.
   */
  Point setpoint(data_t x, data_t y, data_t z) { return _setpoint = Point(x, y, z); } 

  /**
   * @brief Gets the current actual position.
   * @return Current machine position.
   */
  Point position() const { return _position; }

  /**
   * @brief Sets the current actual position.
   * @param p New machine position.
   * @return Updated machine position.
   */
  Point position(Point p) { return _position = p; }

  /**
   * @brief Gets the last loaded configuration data.
   * @return JSON data used to configure the machine.
   */
  nlohmann::json data() const { return _data; }

  private:
  nlohmann::json _data{};
  // properties
  Point _zero{0, 0, 0};
  Point _offset{0, 0, 0};
  data_t _tq = 0.001;        // 1 ms
  data_t _A = 5.0;           // m^s/s
  data_t _fmax = 10000.0;    // mm/min
  data_t _max_error = 0.005; // mm
  // State parameters
  Point _setpoint, _position;
  data_t _error = 0.0;       // mm
};

} // namespace cncpp
