/*
  ____                                             _               
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___ 
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/                                            

Class representing a G-code program, as a list of Block instances
*/

#pragma once

#include "defines.hpp"
#include "machine.hpp"
#include "block.hpp"
#include <list>

namespace cncpp {
using block_iterator = std::list<Block>::iterator;

class Program : public Object, public std::list<Block> {
  public:

  // LIFECYCLE =================================================================
  Program(const std::string &filename, Machine *machine);
  Program(Machine *machine);
  ~Program();
  std::string desc(bool colored = true) const override;

  // OPERATORS/OPERATIONS ======================================================
  void load(const std::string &filename, bool append = false);
  Program &operator<<(const std::string &line);
  iterator load_next();
  void rewind();
  void reset();

  // ACCESSORS =================================================================
  block_iterator current() { return _current; }
  bool done() { return _done; };

  private:
  Machine *_machine = nullptr;
  std::string _filename = "";
  iterator _current = this->end();
  bool _done = false;
};



} // namespace cncpp