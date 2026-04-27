/*
  ____                                      
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___  
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \ 
 |  __/| | | (_) | (_| | | | (_| | | | | | |
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|
                  |___/                     
*/

#include "defines.hpp"
#include "program.hpp"
#include <fstream>

using namespace std;
using namespace cncpp;



Program::Program(const std::string &f, Machine *m) : _filename(f), _machine(m) {
  load(_filename);
}

Program::Program(Machine *m) : _machine(m) {}


Program::~Program() {}


std::string Program::desc(bool colored = true) const {

}

void Program::load(const string &filename, bool append) {
  _filename = filename;

  ifstream file(_filename);
  if (! file.is_open()) {
    throw runtime_error("Could not open " + _filename);
  }
  if (!append) reset();
  string line;
  while (getline(file, line)) {
    if (line[0] == '#') continue;
    *this << line;
  }
  file.close();
}