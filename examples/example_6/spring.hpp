#pragma once

#include "model.hpp"

#include <iostream>

class Spring : public Model {
public:
  // Enum to hold the indeces of the states/inputs and the total number of
  // states/inputs
  enum States {
    STATE_X,
    STATE_V,
    STATES_NUMBER,
  };
  enum Inputs { INPUTS_NUMBER };

  Spring(std::string name) : Model(name, STATES_NUMBER, INPUTS_NUMBER) {}

protected:
  Vec compute_x_dot_impl(double dt, Vec inputs, Vec states) override {
    // Creating the derivatives vector with the same size of the states
    Vec x_dot(states.size());

    x_dot[STATE_X] = +states[STATE_V];
    x_dot[STATE_V] = -states[STATE_X] * _k / _m;

    return x_dot;
  }

  bool set_config(std::map<std::string, double> config) override {
    bool ret = true;
    // Check if the key exists, in that case update this model stiffness
    if(config.find("k") != config.end()) {
      _k = config["k"];
    } else {
      std::cout << "Key k was not found" << std::endl;
      ret = false;
    }
    // Same as above but with mass
    if(config.find("m") != config.end()) {
      _m = config["m"];
    } else {
      std::cout << "Key m was not found" << std::endl;
      ret = false;
    }
    
    // Returns true if both of the keys were found
    return ret;
  }

  std::optional<std::map<std::string, double>> get_config() override {
    // Construct a map with the current values.
    return std::map<std::string, double>{
      {"k", _k},
      {"m", _m},
    };
  }

private:
  double _k = 100.0; // Spring stiffness N/m
  double _m = 20.0;   // Mass Kg
};
