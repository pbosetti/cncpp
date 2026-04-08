#pragma once

#include "model.hpp"

class ForcedSpring : public Model {
public:
  // Enum to hold the indeces of the states/inputs and the total number of
  // states/inputs
  enum States {
    STATE_X,
    STATE_V,
    STATES_NUMBER,
  };
  enum Inputs {
    INPUT_F,
    INPUTS_NUMBER,
  };

  ForcedSpring(std::string name) : Model(name, STATES_NUMBER, INPUTS_NUMBER) {}

  Vec compute_x_dot_impl(double dt, Vec inputs, Vec states) {
    // Creating the derivatives vector with the same size of the states
    Vec x_dot(states.size());

    x_dot[STATE_X] = states[STATE_V];
    x_dot[STATE_V] = inputs[INPUT_F] / _m - states[STATE_X] * _k / _m;

    return x_dot;
  }

private:
  double _k = 100.0; // Spring stiffness N/m
  double _m = 3.0;   // Mass
};
