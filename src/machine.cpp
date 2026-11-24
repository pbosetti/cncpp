#include "machine.hpp"
#include <sstream>
#include <filesystem>

using namespace std;
using namespace cncpp;
using namespace toml;
using json = nlohmann::json;

// Forward declaration of private utility functions:
static data_t read_number(json const &j, const string &key, data_t def);
static Point read_point(json const &j, const string &key, Point const &def);


Machine::Machine(json const &j) {
  load(j);
}

Machine::Machine(string const &filename) {
  load(filename);
}

Machine::~Machine() {
  // TODO: disconnect from machine
}

string Machine::desc(bool colored) const {
  ostringstream ss;
  ss << "A = " << _A << ", "
     << "tq = " << _tq << ", "
     << "max_error = " << _max_error << ", "
     << "f_max = " << _fmax << ", "
     << "zero = " << _zero.desc(colored) << ", "
     << "offset = " << _offset.desc(colored) << endl;
  return ss.str();
}

void Machine::load(json const &j) {
  _data = j;
  // JSON fields may not exist or may be of a wrong type, so we use default 
  // values if they are missing or invalid, by calling the read_number and 
  // read_point utility functions
  _tq = read_number(j, "tq", _tq);
  _A = read_number(j, "A", _A);
  _fmax = read_number(j, "f_max", _fmax);
  _max_error = read_number(j, "max_error", _max_error);
  _zero = read_point(j, "zero", _zero);
  _offset = read_point(j, "offset", _offset);
}

void Machine::load(toml::table const &t) {
  ostringstream ss;
  // See https://marzer.github.io/tomlplusplus/classtoml_1_1json__formatter.html
  // for how to convert a toml::table to a JSON object 
  ss << toml::json_formatter{t} << endl;
  load(json::parse(ss.str()));
}


void Machine::load(string const &filename) {
  filesystem::path path{filename};
  if (!filesystem::exists(path)) throw("File does not exist:" + filename);
  if (!filesystem::is_regular_file(path)) throw("File is not valid: " + filename);

  if (path.extension() == ".json") {
    ifstream f(filename);
    load(json::parse(f));
  } else if (path.extension() == ".toml") {
    toml::table t = toml::parse_file(filename);
    auto mt = t["cnc"];
    if (!mt) throw("TOML file must contain a [cnc] section");
    if (!mt.is_table()) throw("TOML [cnc] section must be a table");
    load(*mt.as_table());
  } else {
    throw("Invalid file type (.json or .toml only)");
  }
}

data_t Machine::quantize(data_t t, data_t &dq) const {
  data_t q;
  q = static_cast<int>((t / _tq) + 1) * _tq;
  dq = q - t;
  return q;
}


// Private functions and methods ===============================================

static data_t read_number(json const &j, const string &key, data_t def) {
  if (!j.contains(key))
    return def;
  if (!j[key].is_number())
    throw runtime_error(key + " is not a number");
  return j[key].get<data_t>();
}

static Point read_point(json const &j, const string &key, Point const &def) {
  if (!j.contains(key))
    return def;
  json const &v = j[key];
  if (!v.is_array() || 
      v.size() != 3 ||
      // use std::all_of to check if all elements are numbers
      !all_of(v.begin(), v.end(), [](json const &e) { return e.is_number(); }))
    throw runtime_error(key + " is not an array with three elements");
  return Point(v[0].get<data_t>(), v[1].get<data_t>(), v[2].get<data_t>());
}

// MADS-related ================================================================

void Machine::connect(const string &name, const string &url) {
  _agent = make_unique<Mads::Agent>(name, url);
  _agent->set_settings_timeout(3000);
  _agent->init();
  try {
    json settings = _agent->get_settings();
    load(settings);
  } catch (const exception &e) {
    throw runtime_error(string("Error fetching settings from broker: ") + e.what());
  }
  _agent->set_receive_timeout(1000);
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
     _position.x(_state["output"]["position"][0]);
      _position.y(_state["output"]["position"][1]);
      _position.z(_state["output"]["position"][2]);
    } catch (const exception &e) {
      cerr << "Error parsing postion data: " + e.what(); << endl;
    }
  } catch (const exception &e) {
    cerr << "Error synchronizing: " + e.what() << endl();
  }
  try {
    _error = (_setpoint - _position).length();
  } catch (const exception &e) {
    _error = numeric_limits<data_t>::quiet_NaN();
  }
  clear_command();
}


void Machine::set_setpoint(const Point &p) {

}


void Machine::reset() {

}


bool Machine::is_connected() const {
  return false; // TODO: implement this method to check if the agent is connected 
}


void Machine::send_metrics(const nlohmann::json &metrics) {

}


void Machine::clear_command() {
  _command["input"] = json::object();
  _command["reset"] = false;
}



#ifdef CNCPP_TEST_MACHINE

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <machine.<json|toml>" << endl;
    return EXIT_FAILURE;
  }
  string filename{argv[1]};
  unique_ptr<Machine> m;
  try {
    m = make_unique<Machine>(filename);
  } catch (exception &e) {
    cerr << "Error loading machine from " << filename << ": " << e.what() << endl;
    return EXIT_FAILURE;
  }
  cout << "Parsed machine:" << endl;
  cout << m->data().dump(2) << endl;
  cout << "Machine description:" << endl;
  cout << *m << endl;

  return EXIT_SUCCESS;
}





#endif // CNCPP_TEST_MACHINE