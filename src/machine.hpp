#pragma once

#include "defines.hpp"
#include "point.hpp"
#include <nlohmann/json.hpp>
#include <toml++/toml.hpp>
#include <agent.hpp>

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
  ~Machine(); 

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
   * @brief Loads machine configuration values from TOML data.
   * @param t TOML table containing optional machine parameters.
   *
   * Recognized keys are `tq`, `A`, `fmax`, `max_error`, `zero`, and `offset`.
   * Invalid value types raise std::runtime_error.
   */
  void load(toml::table &t);

  /**
   * @brief Loads machine configuration values from a JSON file.
   * 
   * Note that if using the TOML format, the machine configuration must be nested under a `machine` table.
   * 
   * @param filename Path to the JSON/TOML configuration file.
   */
  void load(std::string &filename);

  /**
   * @brief Quantizes a time value to the next machine sampling instant.
   * @param t Time value to quantize.
   * @param dq Receives the positive quantization delta.
   * @return Quantized time value.
   */
  data_t quantize(data_t t, data_t &dq) const;

  // MADS related ==============================================================
  /**
   * @brief Connect to the MADS broker and initialize the agent for machine 
   * control.
   * 
   * The agent will get from the broker the machine settings in the section 
   * having its `name`
   * 
   * @param name Name of the agent, used for registration with the MADS broker
   * @param url Broker address
   */
  void connect(const std::string &name, const std::string &url = "tcp://localhost:9092");

  /**
   * @brief Send and receive data
   * 
   */
  void sync();

  /**
   * @brief Send metrics to the MADS broker
   * 
   * Metrics can be any informative JSON object made by scalars (numbers or 
   * strings), that can be viewed by the mt_viewer plugin.
   * 
   * @param metrics JSON object containing the metrics to send (a dictnary
   * of keys and scalar values, numbers or strings)
   */
  void send_metrics(const nlohmann::json &metrics);

  /**
   * @brief Set the setpoint object
   * 
   * This call implicitly calls `sync()` to send the new setpoint to the agent 
   * and update the machine state.
   * 
   * @param p The setpoint coordinates
   */
  void set_setpoint(const Point &p);

  /**
   * @brief Reset the machine
   * 
   * This call implicitly calls `sync()` to send the new setpoint to the agent 
   * and update the machine state.
   */
  void reset();

  /**
   * @brief Check if the machine is connected to a MADS agent.
   * 
   * @return true 
   * @return false 
   */
  bool is_connected() const;


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
   * @brief Gets the max machine sampling period.
   * @return Quantization period.
   */
  data_t tq_max() const { return _tq_max; }

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
   * @brief Gets the last loaded configuration data.
   * @return JSON data used to configure the machine.
   */
  nlohmann::json data() const { return _data; }

  /**
   * @brief The MADS Agent associated with this machine, if any.
   * 
   * @return Reference to the Agent instance, or nullptr if no agent is associated.
   */
  Mads::Agent* agent() const { return _agent.get(); }

  /**
   * @brief Gets the current state of the machine.
   * @return JSON data representing the machine state.
   */
  nlohmann::json state() const { return _state; }

  private:
  void clear_command();
  nlohmann::json _data{};
  // properties
  Point _zero{0, 0, 0};
  Point _offset{0, 0, 0};
  data_t _tq = 0.001;        // 1 ms
  data_t _tq_max = 0.010;    // 10 ms
  data_t _A = 5.0;           // m^s/s
  data_t _fmax = 10000.0;    // mm/min
  data_t _max_error = 0.005; // mm
  // State parameters
  Point _setpoint, _position;
  data_t _error = numeric_limits<data_t>::quiet_NaN();       // mm
  // MADS agent
  std::unique_ptr<Mads::Agent> _agent;
  nlohmann::json _command{};
  nlohmann::json _state{};

};

} // namespace cncpp
