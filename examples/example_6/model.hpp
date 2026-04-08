#pragma once

#include "types.hpp"

#include <fstream>
#include <string>

class Model {
public:
  /**
   * @param name The name of the model.
   * @param n_states Number of states
   * @param n_inputs Number of inputs
   */
  Model(std::string name, size_t n_states, size_t n_inputs);

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
