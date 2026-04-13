#include "model.hpp"

#include <exception>
#include <iostream>
#include <map>
#include <optional>

/**
 * @brief Removes the leading and trailing spaces
 * @param str: the string that should be processed
 * @return the trimmed string
 */
std::string trim(std::string str) {
  str.erase(0, str.find_first_not_of(" "));
  str.erase(str.find_last_not_of(" ") + 1);
  return str;
}

Model::Model(std::string name, size_t n_states, size_t n_inputs)
    : _name(name), _n_states(n_states), _n_inputs(n_inputs) {
  _states.resize(_n_states, 0.0);
}

bool Model::load_config(std::string path) {
  // <folder>/<model_name>.txt
  std::string file_name = path + "/" + _name + ".txt";
  std::ifstream config_file(file_name);
  
  if(!config_file.is_open()) {
    std::cout << "Cannot open file (" << file_name << "), is the folder existing?" << std::endl;
    throw std::runtime_error("Unable to open file: " + file_name);
    return false;
  }

  std::map<std::string, double> config;
  // std::unordered_map<std::string, double> config;

  // Will hold the line that should be processed
  std::string line;
  // Read file line by line up to the end.
  while(std::getline(config_file, line)) {
    // k = 100.0
    // mass = 20.0
    // Find the position in the string where we have the equal sign
    auto pos = line.find("=");
    
    // No position was found
    if (pos == std::string::npos) {
      throw std::runtime_error("Wrongly formatted line: " + line);
      return false;
    }

    // Since we found the position, we can pick the two parts of the string
    // the first one is from 0 to pos and will be the parameter name, the
    // second part will be from pos + 1 to the end and will be the parameter value.
    std::string key = line.substr(0, pos);
    std::string value_str = line.substr(pos + 1);

    // Trim the leading and trailing spaces
    key = trim(key);
    value_str = trim(value_str);

    // value_str has a string representation of the number,
    // here we are converting it to the number itself
    double value = std::stod(value_str);

    // double value = std::stod("0.5");
    // We could not have done:
    // double value = "0.5";
    // because "0.5" is a string

    // Inserts and sets the parameter and the parameter value
    // to the config
    config[key] = value;
  }
  // Close the file
  config_file.close();

  // Set the new configuration to the model
  if(!set_config(config)) {
    throw std::runtime_error("Error setting configs");
    return false;
  }

  return true;
}
bool Model::save_config(std::string path) {
  // Get the config from the model, remind that this is an std::optional
  // here I'm using auto to avoid writing the full type name.
  // The variable is called maybe_ since it is an optional,
  // we have to check for the config presence with the if.
  auto maybe_config = get_config();
  if(!maybe_config) {
    return false;
  }
  // Since the value exist, I can take it and assign to a new variable
  // to avoid using maybe_.
  auto config = maybe_config.value();

  // Constructing the file path and opening a file in output.
  std::string file_name = path + "/" + _name + ".txt";
  std::ofstream config_file(file_name);

  // Checking wether the file was opened
  if (!config_file.is_open()) {
    std::cout << "Unable to open the file (" << file_name << "), you should create it." << std::endl;
    throw std::runtime_error("Error opening file: " + file_name);
    return false;
  }
  
  // Here I want to save every item inside the config map.
  // I can loop on the map, the syntax is similar to the vector case.
  //   case of std::vector<double>
  //   for(const auto &element : vec) {}
  // In this loop of the map we are taking directly the
  // key and the value stored in every element.
  for (const auto &[key, value] : config) {
    config_file << key << " = " << value << "\n";
  }

  // Closing the file
  config_file.close();
  return true;
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
  // Write the header as t,x0,x1,x2 // in the case of 3 states
  out << "t";
  for (size_t i = 0; i < _n_states; i++) {
    out << "," << "x" << i;
  }
  out << "\n";
}
// ostream is both ofstream (aka file in output) or also std::cout
void Model::csv_row(std::ostream &out) {
  out << _t;
  for (size_t i = 0; i < _n_states; i++) {
    out << "," << _states[i];
  }
  out << "\n";
}
