/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
Class implementation
*/

#include "machine.hpp"
#include <yaml-cpp/yaml.h>
#include <sstream>

namespace cncpp {

Machine::Machine(const string &s) : _settings_file(s) {
  load(s);
}

void Machine::load(const string &s) {
  _settings_file = s;
  auto data = YAML::LoadFile(s);
  auto machine = data["machine"];
  _A = machine["A"].as<data_t>();
  _tq = machine["tq"].as<data_t>();
  _fmax = machine["fmax"].as<data_t>();
  _max_error = machine["max_error"].as<data_t>();
  _zero = Point(
    machine["zero"][0].as<data_t>(),
    machine["zero"][1].as<data_t>(),
    machine["zero"][2].as<data_t>()
  );
  _offset = Point(
    machine["offset"][0].as<data_t>(),
    machine["offset"][1].as<data_t>(),
    machine["offset"][2].as<data_t>()
  );
}

string Machine::desc(bool colored) const {
  stringstream ss;
  ss << "A = " << _A << ", ";
  ss << "tq = " << _tq << ", ";
  ss << "max_error = " << _max_error << ", ";
  ss << "fmax = " << _fmax << endl;
  ss << "zero = " << _zero.desc(colored) << endl;
  ss << "offset = " << _offset.desc(colored) << endl;
  return ss.str();
}

data_t Machine::quantize(data_t t, data_t &dq) const {
  data_t q;
  q = static_cast<size_t>((t / _tq) + 1) * _tq;
  dq = q - t;
  return q;
}

}



/*
  _____         _   
 |_   _|__  ___| |_ 
   | |/ _ \/ __| __|
   | |  __/\__ \ |_ 
   |_|\___||___/\__|
                    
*/

#ifdef MACHINE_MAIN
#include <iostream>
using namespace std;

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <settings_file.yml>" << endl;
    return 1;
  }

  cncpp::Machine machine = cncpp::Machine(argv[1]);
  cout << machine.desc() << endl;
  
  cncpp::Machine default_machine = cncpp::Machine();
  cout << "Default machine:" << endl;
  cout << default_machine.desc() << endl;
  default_machine.load(argv[1]);
  cout << "Default machine after loading:" << endl;
  cout << default_machine.desc() << endl;



  return 0;

}


#endif