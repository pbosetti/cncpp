

#include "machine.hpp"
#include <fstream>
#include <strstream>


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

void Machine::load(std::string &filename) {
  ifstream f(filename);
  json data = json::parse(f);
  load(data);
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
    cerr << "Usage: " << argv[0] << " <machine.json>" << endl;
    return EXIT_FAILURE;
  }
  string filename{argv[1]};
  Machine m{filename};
  cout << "Parsed JSON data structure: " << endl
       << m.data().dump(2) << endl;

  cout << "Machine parameters: " << endl << m << endl;


  return 0;
}



#endif // CNCPP_TEST_MACHINE