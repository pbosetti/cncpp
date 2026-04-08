#include "model.hpp"

#include <exception>

Model::Model(std::string name, size_t n_states, size_t n_inputs)
    : _name(name), _n_states(n_states), _n_inputs(n_inputs) {
  _states.resize(_n_states, 0.0);
}

void Model::set_x0(Vec initial_state) {
  if (_states.size() != initial_state.size()) {
    throw std::invalid_argument("Invalid initial state size");
  }

  _states = initial_state;
}

Vec Model::compute_x_dot(double dt, Vec inputs, Vec states) {
  if (dt <= 0) {
    throw std::invalid_argument("Invalid dt");
  }
  if (states.size() != _n_states) {
    throw std::invalid_argument("Invalid states size");
  }
  if (inputs.size() != _n_inputs) {
    throw std::invalid_argument("Invalid inputs size");
  }

  Vec x_dot = compute_x_dot_impl(dt, inputs, states);

  if (x_dot.size() != _n_states) {
    throw std::logic_error("Wrong number of states derivatives");
  }
  return x_dot;
}

void Model::step(double dt, Vec inputs) {
  // Compute the states derivatives from the current state
  Vec x_dot = compute_x_dot(dt, inputs, _states);

  // Euler integration
  for (size_t i = 0; i < _n_states; i++) {
    _states[i] = _states[i] + dt * x_dot[i];
  }
  // _t = _t + dt;
  _t += dt;

  // Log current states
  log();
}

bool Model::start_log(std::string path) {
  std::string file_name = path + "/" + _name + ".csv";
  _log_file.open(file_name);

  if (_log_file.is_open()) {
    // Write the header at the first line
    csv_header(_log_file);
    return true;
  } else {
    return false;
  }
}
void Model::stop_log() { _log_file.close(); }
void Model::log() {
  // If the file is open (then the logging is enabled).
  if (_log_file.is_open()) {
    csv_row(_log_file);
  }
}
void Model::csv_header(std::ostream &out) {
  // Write the header as t,x0,x1,x2
  out << "t";
  for (size_t i = 0; i < _n_states; i++) {
    out << "," << "x" << i;
  }
  out << "\n";
}
void Model::csv_row(std::ostream &out) {
  out << _t;
  for (size_t i = 0; i < _n_states; i++) {
    out << "," << _states[i];
  }
  out << "\n";
}
