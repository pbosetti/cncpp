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
#include <nlohmann/json.hpp>
#include <toml++/toml.hpp>
#include <agent.hpp>

namespace cncpp {
class Machine : public Object {

public:
  // LIFECYCLE =================================================================

  /**
   * @brief Construct a new Machine object (empty)
   * 
   */
  Machine() = default;
  
  /**
   * @brief Construct a new Machine object from a JSON object
   * 
   * @param j nlohmann::json object
   */
  Machine(nlohmann::json const &j);

  /**
   * @brief Construct a new Machine object by reading a JSON or TOML file
   * 
   * @param filename Valid JSON or TOML file
   */
  Machine(std::string const &filename);

  /**
   * @brief Destroy the Machine object
   * 
   * If connected to the actual machine, it also closes the connection.
   * 
   */
  ~Machine();

  /**
   * @brief Provide a description of the machine object
   * 
   * @param colored output must be colored
   * @return std::string Machine description
   */
  std::string desc(bool colored = true) const override;

  // INTERFACE =================================================================

  /**
   * @brief loading parameters from a JSON object
   * 
   * @test for any member field, if the JSON contains a valid value, it is stored; if it is missing, the fedault is used; if it is an invalid type, an exception is thrown
   * 
   * @param json 
   */
  void load(nlohmann::json const &json);

  /**
   * @brief Loading parameters from a TOML table
   * 
   * @test the table (already the content of the [cnc] section) is converted to JSON and passed to load(json), so the same rules on valid, missing and invalid fields apply
   * 
   * @param toml 
   */
  void load(toml::table const &toml);

  /**
   * @brief Loading parameters by reading a JSON or TOML file
   * 
   * @test if the file does not exist, is not a regular file, has an extension other than .json or .toml, or cannot be parsed, an exception is thrown; a JSON file is parsed and passed to load(json); for a TOML file, if it lacks a [cnc] section or [cnc] is not a table, an exception is thrown, otherwise the [cnc] table is passed to load(toml)
   * @param filename 
   */
  void load(std::string const &filename);


  /**
   * @brief Quantizes a time value to the nearest time quantum.
   * @param t The time to quantize.
   * @param dq A reference to a variable that will hold the quantization error.
   * @return The quantized time.
   */
  data_t quantize(data_t t, data_t &dq) const;

  // Machine connection ========================================================

  /**
   * @brief Connects the machine to a MADS broker.
   * @param name The name of this Agent.
   * @param url The URL of the remote server in the form `"tcp<hostname or IP>:<port>`".
   */
  void connect(const std::string &name, const std::string &url = "tcp://localhost:9092");

  /**
   * @brief Synchronizes the machine state with the MADS broker.
   * 
   * This will send the current setpoint and receive the current state from 
   * the machine/machine simulator.
   */
  void sync();

  /**
   * @brief Sets the machine's setpoint to the specified point.
   * @param p The point to set as the new setpoint.
   */
  void set_setpoint(const Point &p);

  /**
   * @brief Resets the machine to its initial state.
   */
  void reset();

  /**
   * @brief Checks if the machine is connected to the MADS broker.
   * @return true if the machine is connected, false otherwise.
   */
  bool is_connected() const;

  /**
   * @brief Sends metrics to the MADS network.
   * 
   * Metrics are sent as JSON and typically used by the viewer/dashboard for
   * debugging/logging purposes. The metrics can include any relevant 
   * information about the machine's state, performance, or other data that may
   *  be useful for monitoring or analysis.
   * 
   * @param metrics The metrics to send.
   */
  void send_metrics(const nlohmann::json &metrics);



  // ACCESSORS =================================================================
  // TODO: check and remove setters that are not really needed

  /**
   * @brief Return the loaded data as a JSON object. This is useful for debugging and for saving the machine configuration to a file.
   * 
   * @return nlohmann::json const& 
   */
  nlohmann::json const &data() const { return _data; }

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
  Point const &zero(data_t x, data_t y, data_t z) {
    return (_zero = Point{x, y, z});
  }

private:
  nlohmann::json _data{};
  data_t _A = 5.0;                        // mm/s/s
  data_t _fmax = 10000.0;                 // mm/min
  data_t _max_error = 0.005;              // mm
  data_t _tq = 0.001;                     // s
  Point _zero{0.0, 0.0, 0.0};             // Initial position
  Point _offset{0.0, 0.0, 0.0};           // WP origin offset
  Point _travels{1000.0, 1000.0, 1000.0}; // TODO: actually support this!

  // Interfacing with machine/simulator:
  std::unique_ptr<Mads::Agent> _agent;
  nlohmann::json _command{};
  nlohmann::json _state{};

}; // class Machine

} // namespace cncpp