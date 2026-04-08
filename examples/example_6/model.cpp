#include "model.hpp"

#include <exception>

Model::Model(std::string name, size_t n_states, size_t n_inputs)
: _name(name), _n_states(n_states), _n_inputs(n_inputs)
{
    _states.resize(_n_states, 0.0);
}

void Model::set_x0(Vec initial_state) {
    if (_states.size() != initial_state.size()) {
        throw std::invalid_argument("Invalid size");
    }
    _states = initial_state;
}

Vec Model::compute_x_dot(double dt, Vec inputs, Vec states) {
    if (dt <= 0) { 
        throw std::invalid_argument("Invalid dt");
    }
    if (inputs.size() != _n_inputs) {
        throw std::invalid_argument("Invalid inputs size"); 
    }
    return compute_x_dot_impl(dt, inputs, states);
}

void Model::step(double dt, Vec inputs) {
    Vec x_dot = compute_x_dot(dt, inputs, _states);
    for (size_t i = 0; i < _n_states; i++) {
        _states[i] = _states[i] + dt * x_dot[i];
    }
    // _t = _t + dt;
    _t += dt;
    csv_row(_log_file);
}

bool Model::start_log(std::string path) {
    std::string file_name = path + "/" + _name + ".csv";
    _log_file.open(file_name);
    if (_log_file.is_open()) {
        csv_header(_log_file);
    }
    return _log_file.is_open();
}
void Model::stop_log() {
    _log_file.close();
}
void Model::csv_header(std::ostream &out) {
    out << "t";
    for(size_t i = 0; i < _n_states; i++) {
        out << "," << "x" << i;
    }
    out << "\n";
}
void Model::csv_row(std::ostream &out) {
    out << _t;
    for(size_t i = 0; i < _n_states; i++) {
        out << "," << _states[i];
    }
    out << "\n";
}
