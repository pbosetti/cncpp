#include "program.hpp"
#include <fstream>
#include <fmt/core.h>

using namespace cncpp;
using namespace std;

Program::Program(const string &filename, Machine *machine) : _machine(machine), _filename(filename) {
  load(_filename);
}

Program::~Program() {
  if (_debug)
    cout << fmt::format("Destroying program with {:} blocks", size()) << endl;
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
  for(auto &b : *this) {
    ss << b.desc() << ", previous: " << (b.prev ? to_string(b.prev->n()) : "(null)") << std::endl;
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
    machine.load("machine.ini");
  } catch (exception &e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }
  Program program(&machine);
  try {
    program.load(argv[1]);
  } catch (exception &e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  try {
    program << "N10 G0 X0 Y0 Z0";
    program << "N20 G1 X10 Y10 Z10 F1000";
    program << "N30 G2 X20 Y20 Z20 I10 J10 K30 F500";
  } catch (CNCError &e) {
    std::cerr << "Error: " << e.what() << " in " << e.who() << std::endl;
  }

  cout << program.desc(false) << endl;

  for (auto &b : program) {
    cout << b.n() << ": length " << b.length() << endl;
  }

  return 0;
}


#endif