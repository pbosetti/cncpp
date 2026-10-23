

#include "machine.hpp"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace cncpp;
using namespace std;
using namespace Mads;
using namespace chrono_literals; // allows to do 100ms
using json = nlohmann::json;

static data_t read_number(json const &j, string const &key, data_t def);
static Point read_point(json const &j, string const &key, Point const &def);

// LIFECYCLE =================================================================

Machine::Machine(json const &j) {
  load(j);
}

Machine::Machine(std::string const &filename) {
  if (filename.substr(0, 6) == "tcp://") {
    connect("cnc", filename);
  } else {
    load(filename);
  }
}

Machine::~Machine() {
  if (_agent && _agent->is_connected()) {
    _agent->disconnect();
  }
}


std::string Machine::desc(bool colored) const {
  stringstream ss;
  ss << "A = " << _A << ", "
     << "tq = " << _tq << ", "
     << "tq_max = " << _tq_max << ", "
     << "max_error = " << _max_error << ", "
     << "fmax = " << _fmax << ", "
     << "zero = " << _zero.desc(colored) << ", "
     << "offset = " << _offset.desc(colored) << endl;
  return ss.str();
}

// OPERATIONS/OPERATORS ======================================================
void Machine::load(json const &j) {
  _data = j;
  _tq = read_number(j, "tq", _tq);
  _tq_max = read_number(j, "tq_max", _tq_max);
  _A = read_number(j, "A", _A);
  _fmax = read_number(j, "fmax", _fmax);
  _max_error = read_number(j, "max_error", _max_error);
  _zero = read_point(j, "zero", _zero);
  _offset = read_point(j, "offset", _offset);
}

void Machine::load(toml::table const &t) {
  // Convert the toml::table to JSON by formatting it as JSON text and parsing
  // it back, then use the JSON loader
  ostringstream ss;
  ss << toml::json_formatter{t} << endl;
  load(json::parse(ss.str()));
}

void Machine::load(std::string const &filename) {
  filesystem::path path{filename};
  if (!filesystem::exists(path)) throw runtime_error("File does not exist: " + filename);
  if (!filesystem::is_regular_file(path)) throw runtime_error("Not a regular file: " + filename);
  if (path.extension() == ".json") {
    ifstream f(filename);
    load(json::parse(f));
  } else if (path.extension() == ".toml" || path.extension() == ".ini") {
    toml::table t = toml::parse_file(filename);
    auto mt = t["cnc"];
    if (!mt) throw runtime_error("TOML file does not contain a 'cnc' table: " + filename);
    if (!mt.is_table()) throw runtime_error("Expected 'cnc' to be a table in TOML file");
    load(*mt.as_table());
  } else {
    throw runtime_error("Unsupported file type: " + filename);
  }
}

data_t Machine::quantize(data_t t, data_t &dq) const {
  data_t q;
  q = static_cast<size_t>((t / _tq) + 1) * _tq;
  dq = q - t;
  return q;
}

void Machine::connect(const std::string &name, const std::string &url) {
  _agent = make_unique<Agent>(name, url);
  _agent->set_settings_timeout(10000);
  _agent->init();
  try {
    json settings = _agent->get_settings();
    load(settings);
  } catch (const exception &e) {
    throw runtime_error(string("Error fetching agent settings: ") + e.what());
  }
  _agent->set_agent_id(name);
  _agent->set_receive_timeout(1000ms);
  _agent->set_high_watermark(1);
  _agent->connect();
  clear_command();
}

void Machine::sync() {
  if (!_agent) {
    return;
  }
  try {
    _agent->publish(_command);
    _agent->receive();
    _state = json::parse(get<1>(_agent->last_message()));
    try {
      _position.x(_state["output"]["position"].at(0).get<data_t>());
      _position.y(_state["output"]["position"].at(1).get<data_t>());
      _position.z(_state["output"]["position"].at(2).get<data_t>());
    } catch (const exception &e) {
      cerr << "Error parsing position data: " << e.what() << endl;
    }
    } catch (const exception &e ) {
    cerr << "Error during MADS sync: " << e.what() << endl;
  }
  try {
    _error = (_setpoint - _position).length();
  } catch (const exception &e) {
    _error = numeric_limits<data_t>::quiet_NaN();
  }
  clear_command();
}

void Machine::set_setpoint(const Point &p) {
  _setpoint = p;
  if (_agent) {
    _command["fmu_input"]["setpoint"] = {p.x(), p.y(), p.z()};
    sync();
  } else { // ideal machine: no dynamics, no error
    _position = p;
    _error = 0;
  }
}

void Machine::reset() {
  if (_agent) {
    _command["fmu_reset"] = true;
    sync();
  }
}

bool Machine::is_connected() const {
  return _agent && _agent->is_connected();
}

void Machine::send_metrics(const nlohmann::json &metrics) {
  if (_agent) {
    auto msg = nlohmann::json::object();
    msg["metrics"] = metrics;
    _agent->publish(msg, "fmu_machine_tool");
  }
}



// PRIVATE =====================================================================
void Machine::clear_command() {
  _command["fmu_input"] = json::object();
  _command["fmu_reset"] = false;
}

// UTILITY FUNCTIONS ===========================================================

// Reads an optional numeric value, returning def when the key is missing
static data_t read_number(json const &j, string const &key, data_t def) {
  if (!j.contains(key))
    return def;
  if (!j.at(key).is_number())
    throw runtime_error(key + " is not a number");
  return j.at(key).get<data_t>();
}

// Reads an optional array of three numbers, returning def when missing
static Point read_point(json const &j, string const &key, Point const &def) {
  if (!j.contains(key))
    return def;
  json const &v = j.at(key);
  if (!v.is_array() || v.size() != 3 ||
      !all_of(v.begin(), v.end(), [](json const &e) { return e.is_number(); }))
    throw runtime_error(key + " must be an array of three numbers");
  return Point(v[0].get<data_t>(), v[1].get<data_t>(), v[2].get<data_t>());
}

#ifdef CNCPP_TEST_MACHINE
#include <iostream>

int main(int argc, const char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <machine.<json|toml>>" << endl;
    return EXIT_FAILURE;
  }
  string filename{argv[1]};
  Machine m{filename};
  cout << "Parsed data structure: " << endl
       << m.data().dump(2) << endl;

  cout << "Machine parameters: " << endl << m << endl;


  return 0;
}



#endif // CNCPP_TEST_MACHINE