/*
  ____                                             _
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/
*/
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "block.hpp"
#include "cncpp.hpp"
#include "machine.hpp"
#include <iostream>
#include <list>
#include <sstream>

namespace cncpp {

class Program : Object, public std::list<Block> {
public:
  Program(const std::string &filename, Machine *machine);
  Program(Machine *machine) : _machine(machine) {}
  ~Program();
  void load(const std::string &filename);

  std::string desc(bool colored) const override;

  Program &operator<<(string line);

  using iterator = std::list<Block>::iterator;
  using const_iterator = std::list<Block>::const_iterator;


private:
  // std::list<Block> _blocks;
  Machine *_machine;
  std::string _filename;
};

} // namespace cncpp

#endif // PROGRAM_HPP