/*
  ____                                             _
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/
Program class header
Author: Paolo Bosetti, 2024
*/
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "block.hpp"
#include "defines.hpp"
#include "machine.hpp"
#include <iostream>
#include <list>
#include <sstream>

namespace cncpp {

class Program : Object, public std::list<Block> {
public:
  Program(const std::string &filename, Machine *machine);
  Program(Machine *machine) : _machine(machine) {}
  Program() : _machine() {}
  ~Program();
  void load_machine(std::string machine_file);
  void load(const std::string &filename);
  std::string desc(bool colored) const override;

  Program &operator<<(string line);

  // Iterating blocks
  using iterator = std::list<Block>::iterator;
  using const_iterator = std::list<Block>::const_iterator;
  void rewind() { _current = begin(); _current--; _done = false;}
  Program::iterator load_next() { _current++; _done = _current == end(); return _current; }
  Program::iterator current() { return _current; }
  bool done() const { return _done; };

private:
  Machine *_machine;
  Program::iterator _current = begin();
  bool _done = false;
  std::string _filename;
};

} // namespace cncpp

#endif // PROGRAM_HPP