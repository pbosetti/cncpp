#pragma once

#include "types.hpp"

#include <fstream>
#include <map>
#include <string>
#include <functional>
#include <optional>

class Model {
public:
  /**
   * @param name The name of the model.
   * @param n_states Number of states
   * @param n_inputs Number of inputs
   */
  Model(std::string name, size_t n_states, size_t n_inputs);

  /**
   * @brief Loads the configuration from: path/<model_name>.txt
   * @param path the folder where to find the <model_name>.txt
   */
  bool load_config(std::string path);
  /**
   * @brief Saves the configuration in an already existing file called:
   * path/<model_name>.txt
   * @param path the folder where to find the <model_name>.txt
   * @return true if it was possible to create and store the configuration
   */
  bool save_config(std::string path);

  /**
   * @brief Set the initial state of the model.
   * @param initial_state vector of the initial states
   */
  void set_x0(Vec initial_state);

  /**
   * @brief performs an integration step of the model.
   * @param dt integration step in seconds
   * @param inputs vector holding the input variables (forces/controls)
   */
  void step(double dt, Vec inputs);

  /**
   * @brief performs an integration step of the model.
   * @param dt integration step in seconds
   * @param inputs vector holding the input variables (forces/controls)
   * @param integrator function that performs the integration.
   */
  void step(double dt, Vec inputs,
            std::function<void(double dt, Vec states, Vec x_dot)> integrator) {
    Vec x_dot = compute_x_dot(dt, inputs, _states);

    integrator(dt, _states, x_dot);

    _t += dt;
    // Log current states
    log();
  }

  /**
   * @brief Enable and starts logging in CSV format.
   * @param path output directory
   * @returns true if the log file is created
   */
  bool start_log(std::string path);
  /**
   * @brief Stops logging in the CSV file
   */
  void stop_log();

  /**
   * @brief writes the CSV header of this model
   */
  void csv_header(std::ostream &out);
  /**
   * @brief writes in CSV format the model state (time and states).
   */
  void csv_row(std::ostream &out);

protected:
  std::string _name;

  /**
   * @brief Function that must be overridden from the inherited classes. This
   * implements the dynamics of the model
   * @param dt integration time step
   * @param inputs vector of inputs
   * @param states vector of states where to compute the states derivatives
   * @returns vector of states derivatives
   */
  virtual Vec compute_x_dot_impl(double dt, Vec inputs, Vec states) = 0;

  /**
   * @brief From the config, update the model parameters
   * @param config the config itself
   * @return if successful or not
   */
  virtual bool set_config(std::map<std::string, double> config) {
    return true;
  };

  /**
   * @brief Gets the default configuration if available
   * @return either nullopt (aka empty optional) or the config.
   */
  virtual std::optional<std::map<std::string, double>> get_config() {
    return std::nullopt;
  }

private:
  /**
   * @brief called from the integrator. Performs some checks and then calls
   * compute_x_dot_impl.
   * @param dt integration time step
   * @param inputs vector of inputs
   * @param states vector of states where to compute the states derivatives
   * @returns vector of states derivatives
   */
  Vec compute_x_dot(double dt, Vec inputs, Vec states);

  /**
   * @brief If enabled, logs to the CSV log file.
   */
  void log();

  size_t _n_states;
  size_t _n_inputs;
  std::ofstream _log_file;

  Vec _states;
  double _t;
};
