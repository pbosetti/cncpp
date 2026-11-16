/*
  ____                                             _               
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___     ___| | __ _ ___ ___ 
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \   / __| |/ _` / __/ __|
 |  __/| | | (_) | (_| | | | (_| | | | | | | | (__| | (_| \__ \__ \
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|  \___|_|\__,_|___/___/
                  |___/                                            
*/

#pragma once

#include "defines.hpp"
#include "point.hpp"
#include "block.hpp"
#include "machine.hpp"
#include <list>


namespace cncpp {

class Program : public Object {
public:
  // Lifecicle: ================================================================
  Program(const std::string &filename, Machine *machine);

  Program(Machine *machine);

  // disable Program p2(p1);
  Program(const Program &) = delete;
  // disable Program p1(m); Program p2 = p1;
  Program &operator=(const Program &) = delete; 

  std::string desc(bool colored = true) const override;


  // Operations: ===============================================================

  void load(const std::string &filename, bool apend = false);

  Program &operator<<(const std::string &line);

  Block &insert(size_t index, const std::string &line);

  Block &load_next();

  void rewind();

  void reset();

  // Accessors: ================================================================
  Block * current() const { return _current; }
  size_t size() const { return _blocks.size(); }
  bool done() const { return _started && !_current; }
  
  std::list<Block>::iterator begin() { return _blocks.begin(); }
  std::list<Block>::iterator end() { return  _blocks.end(); }
  std::list<Block>::const_iterator begin() const { return _blocks.begin(); }
  std::list<Block>::const_iterator end() const { return  _blocks.end(); }
  
  
  
  
  private:
  Machine *_machine = nullptr;
  std::string _filename = "";
  std::list<Block> _blocks;
  Block *_current = nullptr;
  bool _started = false;
  
  void reparse_from(Block *b);



};




} // namespace cncpp