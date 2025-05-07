/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
*/

#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "defines.hpp"
#include "block.hpp"
#include "machine.hpp"
#include <list>


namespace cncpp {

class Program : Object, public std::list<Block> {

public:
  // LIFECYCLE
  Program(const std::string &filename, Machine *machine);
  Program(Machine *machine) : _machine(machine) {}
  ~Program();
  std::string desc(bool colored = true) const override;

  // METHODS
  void load(const std::string &filename, bool append = false);
  Program &operator<<(std::string line);

  using iterator = std::list<Block>::iterator;

  iterator load_next() { _current++; _done = _current == end(); return _current; }
  void rewind() { _current = begin(); _done = false; }
  void reset() { clear(); rewind(); }

  // ACCESSORS
  bool done() const { return _done; }


private:
  Machine *_machine = nullptr;
  std::string _filename;
  iterator _current = begin();
  bool _done = false;
};


}



#endif // PROGRAM_HPP