/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              

 */
#pragma once
#include "defines.hpp"
#include "point.hpp"


namespace cncpp {
class Machine : public Object {

  public:
  // LIFECYCLE =================================================================
  Machine() = default;
  ~Machine() {};

  std::string desc(bool colored = true) const override;

  // INTERFACE =================================================================



  // ACCESSORS =================================================================
  // TODO: check and remove setters that are not really needed
  /**
   * @brief Gets the machine acceleration.
   * @return The acceleration in mm/s^2.
   */
  data_t A() const { return _A; }

  /**
   * @brief Sets the machine acceleration.
   * @param value The acceleration in mm/s^2.
   * @return The assigned acceleration.
   */
  data_t A(data_t value) { return (_A = value); }

  /**
   * @brief Gets the maximum feed rate.
   * @return The maximum feed rate in mm/min.
   */
  data_t fmax() const { return _fmax; }

  /**
   * @brief Sets the maximum feed rate.
   * @param value The maximum feed rate in mm/min.
   * @return The assigned maximum feed rate.
   */
  data_t fmax(data_t value) { return (_fmax = value); }

  /**
   * @brief Gets the maximum permitted error.
   * @return The maximum error in mm.
   */
  data_t max_error() const { return _max_error; }

  /**
   * @brief Sets the maximum permitted error.
   * @param value The maximum error in mm.
   * @return The assigned maximum error.
   */
  data_t max_error(data_t value) { return (_max_error = value); }

  /**
   * @brief Gets the workpiece origin offset.
   * @return A constant reference to the offset point.
   */
  Point const &offset() const { return _offset; }

  /**
   * @brief Sets the workpiece origin offset.
   * @param value The new offset point.
   * @return A constant reference to the assigned offset.
   */
  Point const &offset(Point const &value) { return (_offset = value); }

  /**
   * @brief Gets the time quantum.
   * @return The time quantum in seconds.
   */
  data_t tq() const { return _tq; }

  /**
   * @brief Sets the time quantum.
   * @param value The time quantum in seconds.
   * @return The assigned time quantum.
   */
  data_t tq(data_t value) { return (_tq = value); }

  /**
   * @brief Gets the machine's initial position.
   * @return A constant reference to the initial position.
   */
  Point const &zero() const { return _zero; }

  /**
   * @brief Sets the machine's initial position.
   * @param value The new initial position.
   * @return A constant reference to the assigned initial position.
   */
  Point const &zero(Point const &value) { return (_zero = value); }
  Point const &zero(data_t x, data_t y, data_t z) { return (_zero = Point{x, y, z}); }


  private:
  data_t _A = 5.0; // mm/s/s
  data_t _fmax = 10000.0; // mm/min
  data_t _max_error = 0.005; // mm
  data_t _tq = 0.001; // s
  Point _zero{0.0, 0.0, 0.0};   // Initial position
  Point _offset{0.0, 0.0, 0.0}; // WP origin offset
  Point _travels{1000.0, 1000.0, 1000.0} // TODO: actually support this!
};

} // namespace cncpp