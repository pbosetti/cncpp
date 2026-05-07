

#include "machine.hpp"
#include <fstream>
#include <strstream>
#include <filesystem>


using namespace cncpp;
using namespace std;
using json = nlohmann::json;

// LIFECYCLE =================================================================

Machine::Machine(json &j) {
  load(j);
}

Machine::Machine(std::string &filename) {
  load(filename);
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
void Machine::load(nlohmann::json &j) {
  _data = j;
  if (_data.contains("tq")) {
    if (!_data["tq"].is_number()) throw runtime_error("tq is not a number");
    _tq = _data["tq"];
  }
  if (_data.contains("tq_max")) {
    if (!_data["tq_max"].is_number()) throw runtime_error("tq_max is not a number");
    _tq_max = _data["tq_max"];
  }
  if (_data.contains("A")) {
    if (!_data["A"].is_number()) throw runtime_error("A is not a number");
    _A = _data["A"];
  }
  if (_data.contains("fmax")) {
    if (!_data["fmax"].is_number()) throw runtime_error("fmax is not a number");
    _fmax = _data["fmax"];
  }
  if (_data.contains("max_error")) {
    if (!_data["max_error"].is_number()) throw runtime_error("max_error is not a number");
    _max_error = _data["max_error"];
  }
  if (_data.contains("zero")) {
    if (!_data["zero"].is_array() || _data["zero"].size() != 3)
      throw runtime_error("zero must be an array of three numbers");
    for (auto &v : _data["zero"]) {
      if (!v.is_number()) throw runtime_error("zero has a non-numerical value");
    }
    _zero.x(_data["zero"][0]);
    _zero.y(_data["zero"][1]);
    _zero.z(_data["zero"][2]);
  }
  if (_data.contains("offset")) {
    if (!(_data["offset"].is_array() && _data["offset"].size() == 3))
      throw runtime_error("offset must be an array of three numbers");
    for (auto &v : _data["offset"]) {
      if (!v.is_number()) throw runtime_error("offset has a non-numerical value");
    }
    _offset.x(_data["offset"][0]);
    _offset.y(_data["offset"][1]);
    _offset.z(_data["offset"][2]);
  }
}

void Machine::load(toml::table &t) {
  if (t.contains("tq")) {
    if (!t["tq"].is_floating_point()) throw runtime_error("tq is not a number");
    _tq = t["tq"].value_or(_tq);
  }
  if (t.contains("tq_max")) {
    if (!t["tq_max"].is_floating_point()) throw runtime_error("tq_max is not a number");
    _tq_max = t["tq_max"].value_or(_tq_max);
  }
  if (t.contains("A")) {
    if (!t["A"].is_floating_point()) throw runtime_error("A is not a number");
    _A = t["A"].value_or(_A);
  }
  if (t.contains("fmax")) {
    if (!t["fmax"].is_floating_point()) throw runtime_error("fmax is not a number");
    _fmax = t["fmax"].value_or(_fmax);
  }
  if (t.contains("max_error")) {
    if (!t["max_error"].is_floating_point()) throw runtime_error("max_error is not a number");
    _max_error = t["max_error"].value_or(_max_error);
  }
  if (t.contains("zero")) {
    auto zero = t["zero"];
    if (!zero.is_array() || zero.as_array()->size() != 3)
      throw runtime_error("zero must be an array of three numbers");
    for (auto &v : *zero.as_array()) {
      if (!v.is_floating_point()) throw runtime_error("zero has a non-numerical value");
    }
    _zero.x(zero[0].value_or(_zero.x()));
    _zero.y(zero[1].value_or(_zero.y()));
    _zero.z(zero[2].value_or(_zero.z()));
  }
  if (t.contains("offset")) {
    auto offset = t["offset"];
    if (!offset.is_array() || offset.as_array()->size() != 3)
      throw runtime_error("offset must be an array of three numbers");
    for (auto &v : *offset.as_array()) {
      if (!v.is_floating_point()) throw runtime_error("offset has a non-numerical value");
    }
    _offset.x(offset[0].value_or(_offset.x()));
    _offset.y(offset[1].value_or(_offset.y()));
    _offset.z(offset[2].value_or(_offset.z()));
  }

  // To store the original TOML structure in _data field, we need to convert 
  // the toml::table to JSON. We can do this by first formatting the table as 
  // JSON text and then parsing it back into a JSON object.
  ostringstream ss;
  ss << toml::json_formatter{t} << endl;
  _data = json::parse(ss.str());
}

void Machine::load(std::string &filename) {
  filesystem::path path{filename};
  if (!filesystem::exists(path)) throw runtime_error("File does not exist: " + filename);
  if (!filesystem::is_regular_file(path)) throw runtime_error("Not a regular file: " + filename);
  if (path.extension() == ".json") {
    ifstream f(filename);
    json data = json::parse(f);
    load(data);
  } else if (path.extension() == ".toml") {
    toml::table t = toml::parse_file(filename);
    if (t.contains("machine")) {
      auto mt = t["machine"];
      if (!mt.is_table()) throw runtime_error("Expected 'machine' to be a table in TOML file");
      load(*mt.as_table());
    } else {
      throw runtime_error("TOML file does not contain a 'machine' table: " + filename);
    }
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