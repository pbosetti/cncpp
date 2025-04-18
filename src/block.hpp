/*
  ____  _            _           _               
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___ 
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/
                                                 
Represents a block of G-code, including parsing and interpolation.
The Block class is responsible for parsing a line of G-code, determining the 
type of motion (linear, circular, etc.), and calculating the necessary 
parameters for interpolation.
 */
#ifndef BLOCK_HPP
#define BLOCK_HPP

// INCLUDES AND DEFINES --------------------------------------------------------
#include "defines.hpp"
#include "point.hpp"
#include "machine.hpp"
#include <map>
#include <functional>

// NAMESPACES AND CONSTANTS ----------------------------------------------------
using namespace std;

namespace cncpp {

class Block : Object {
public:

  struct Profile {
    data_t a, d;
    data_t f, l;
    data_t fs, fe;
    data_t dt_1, dt_m, dt_2;
    data_t dt;
    data_t current_acc;
    data_t lambda(data_t t, data_t &s) const;
  };

  enum class BlockType {
    RAPID = 0,
    LINE,
    CWA,
    CCWA,
    NO_MOTION
  };

  // types[BlockType::LINE] => "linear"
  static const map<BlockType, string> types;

  // LIFECYCLE -----------------------------------------------------------------
  Block(string line);
  Block(string line, Block &prev);
  ~Block();
  string desc(bool colored = true) const override;

  // METHODS -------------------------------------------------------------------
  void parse(const Machine *m);
  data_t lambda(data_t time, data_t &speed) const;
  Point interpolate(data_t lambda) const;
  Point interpolate(data_t time, data_t &lambda, data_t &speed) const;
  void walk(function<void(Block const &b, data_t t, data_t l, data_t s)> f) const;

  // ACCESSORS -----------------------------------------------------------------
  string line() const { return _line; }
  size_t n() const { return _n; }
  size_t tool() const { return _tool; }
  data_t feedrate() const { return _feedrate; }
  data_t arc_feedrate() const { return _arc_feedrate; }
  data_t spindle() const { return _spindle; }
  data_t length() const { return _length; }
  Point target() const { return _target; }
  Point center() const { return _center; }
  Point delta() const { return _delta; }
  // We'll be able to use it as: b.profile().dt
  Profile profile() const { return _profile; }
  

private:
  Machine *_machine;
  Profile _profile;
  BlockType _type = BlockType::RAPID;
  string _line;
  size_t _n = 0;
  size_t _tool = 0;
  data_t _feedrate = 0;
  data_t _arc_feedrate = 0;
  data_t _spindle = 0;
  Point _target = Point();
  Point _center = Point();
  Point _delta = Point();
  data_t _length = 0;
  data_t _i = 0, _j = 0, _r = 0;
  data_t _theta_0 = 0, _dtheta = 0;
  data_t _acc = 0;

public:
  Block *prev, *next;

};


}

#endif // BLOCK_HPP