/*
  ____                                             _               
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___ 
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/                                            
Program class implementation
Author: Paolo Bosetti, 2024
*/
#include "program.hpp"
#include <fmt/core.h>
#include <fstream>
#include <rang.hpp>

using namespace cncpp;
using namespace std;
using namespace rang;

Program::Program(const string &filename, Machine *machine)
    : _machine(machine), _filename(filename) {
  load(_filename);
}

Program::~Program() {
  if (_debug)
    cerr << rang::style::italic
         << fmt::format("Destroying program with {:} blocks", size())
         << rang::style::reset << endl;
}

void Program::load_machine(string machine_file) {
  _machine->load(machine_file);
}

void Program::load(const std::string &filename) {
  _filename = filename;
  ifstream file(_filename);
  if (!file.is_open()) {
    throw runtime_error("Could not open file: " + _filename);
  }
  
  string line;
  while (getline(file, line)) {
    *this << line;
  }
  file.close();
}

Program &Program::operator<<(string line) {
  if (size() > 0) {
    emplace_back(line, back());
  } else {
    emplace_back(line);
  }
  back().parse(_machine);
  return *this;
}

std::string Program::desc(bool colored) const {
  std::ostringstream ss;
  for (auto &b : *this) {
    ss << b.desc()
       << fmt::format(", previous: {:0>3}", b.prev ? b.prev->n() : 0)
       << std::endl;
  }
  return ss.str();
}

/*
  _____         _
 |_   _|__  ___| |_
   | |/ _ \/ __| __|
   | |  __/\__ \ |_
   |_|\___||___/\__|

*/
#ifdef PROGRAM_MAIN

int main(int argc, const char *argv[]) {
  Machine machine;
  try {
    machine.load("machine.yml");
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what() << fg::reset
         << style::reset << endl;
    return 1;
  }
  Program program(&machine);
  try {
    program.load(argv[1]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what() << fg::reset
         << style::reset << endl;
    return 1;
  }

  try {
    program << "N10 G0 X0 Y0 Z0"
            << "N15 M30";
    program << "N20 G1 X10 Y10 Z10 F1000";
    program << "N30 G2 X20 Y20 Z20 I10 J10 K30 F500";
  } catch (CNCError &e) {
    cerr << fg::red << style::bold << "Error: " << e.what() << " in " << e.who()
         << fg::reset << style::reset << endl;
  }

  cout << program.desc(false) << endl;

  for (auto &b : program) {
    cout << b.n() << ": length " << b.length() << endl;
  }

  return 0;
}

#endif