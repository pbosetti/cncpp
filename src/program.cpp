/*
  ____                                             _               
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___ 
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/                                            
*/

#include "program.hpp"
#include <rang.hpp>
#include <fmt/core.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cncpp;
using namespace rang;
using namespace fmt;

// LIFECYCLE -------------------------------------------------------------------
Program::Program(const string &f, Machine *m) : _filename(f), _machine(m) {
  load(_filename);
}

Program::~Program() {
  if (_debug)
    cerr << style::italic
         << format("Destroying program {:} with {:} blocks", _filename, size())
         << style::reset << endl;
}

std::string Program::desc(bool colored) const {
  ostringstream ss;
  for (auto &current_block : *this) {
    ss << current_block.desc();
    ss << format(", previous: {:0>3}", current_block.prev ? current_block.prev->n() : 0);
    ss << endl;
  }
  return ss.str();
}


// METHODS ---------------------------------------------------------------------
void Program::load(const string &f, bool append) {
  _filename = f;
  // open the file, load one line at a time, create a new Block with it, 
  // add the block to the list
  ifstream file(_filename);
  if (! file.is_open()) {
    throw runtime_error("Could not open file " + _filename);
  }
  if (!append) reset();
  string line;
  while (getline(file, line)) {
    *this << line;
  }
  file.close();
}


Program &Program::operator<<(string line) {
  if (size() > 0) {
    // emplace_back tries to create a new instance as Block(line)
    emplace_back(line, back());
  } else {
    emplace_back(line);
  }
  back().parse(_machine);
  return *this;
}




/*
  _____         _                     _       
 |_   _|__  ___| |_   _ __ ___   __ _(_)_ __  
   | |/ _ \/ __| __| | '_ ` _ \ / _` | | '_ \ 
   | |  __/\__ \ |_  | | | | | | (_| | | | | |
   |_|\___||___/\__| |_| |_| |_|\__,_|_|_| |_|
                                              
*/

#ifdef PROGRAM_MAIN


int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <file.gcode>" << endl;
    return 1;
  }
  Machine machine;
  try {
    machine.load("machine.yml");
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what()
         << style::reset << fg::reset << endl;
    return 1;
  }
  Program program(&machine);
  try {
    program.load(argv[1]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what()
         << style::reset << fg::reset << endl;
    return 2;
  }

  cout << program.desc() << endl;

  return 0;
}




#endif
