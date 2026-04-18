/*
  ____  _            _           _
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/


Implements a G-code block, which is a collection of G-code commands that are
executed together. Each block can contain multiple commands, and each command
can have multiple parameters.
*/

#pragma once

#include "defines.hpp"
#include "point.hpp"
#include <functional>

namespace cncpp {

class Machine {};

class Block {
public:
  enum class BlockType { RAPID = 0, LINE, CWA, CCWA, NO_MOTION };

  struct Profile {
    data_t a, d;
    data_t f, l;
    data_t fs, fe;
    data_t dt_1, dt_m, dt_2;
    data_t dt;
    data_t current_acc; // along an arc
    data_t lambda(data_t t, data_t &s);
  };

  // LIFECYCLE =================================================================
  Block(std::string line);
  Block(std::string line, Block &prev);
  ~Block();
  std::string desc(bool colored = true);
  Block &operator=(Block &b);

  // OPERATIONS/OPERATORS ======================================================
  Block &parse(const Machine *m);
  data_t lambda(data_t time, data_t &speed);
  Point interpolate(data_t lambda);
  Point interpolate(data_t time, data_t &lambda, data_t &speed);
  void walk(std::function<void(Block &b, data_t t, data_t l, data_t s)> func);

  // ACCESSORS =================================================================
  std::string line() const { return _line; }
  size_t n() const { return _n; }
  data_t dt() const { return _profile.dt; }
  BlockType type() const { return _type; }
  size_t tool() const { return _tool; }
  data_t feedrate() const { return _feedrate; }
  data_t arc_feedrate() const { return _arc_feedrate; }
  data_t spindle() const { return _spindle; }
  data_t length() const { return _length; }
  const Point &target() const { return _target; }
  const Point &center() const { return _center; }
  const Point &delta() const { return _delta; }
  size_t m() const { return _m; }
  const Profile &profile() const { return _profile; }
  bool parsed() const { return _parsed; }

  Block *prev = nullptr;
  Block *next = nullptr;

private:
  std::string _line;                // original G-Code line, eg "N01 G00 X100"
  size_t _n = 0;                    // block number
  Point _target = Point();          // block destination
  Point _center = Point();          // arc center
  Point _delta = Point();           // block projections
  data_t _length = 0;               // block length
  data_t _i = 0, _j = 0, _r = 0;    // arc parameters
  data_t _theta_0 = 0, _dtheta = 0; // arc initial and included angles
  data_t _feedrate = 0;             // feedrate
  data_t _arc_feedrate = 0;         // feedrate along the arc
  data_t _spindle = 0;              // spindle rate
  data_t _acc = 0;                  // actual acceleration
  size_t _m = 0;                    // M command argument
  size_t _tool = 0;                 // current tool number
  BlockType _type = BlockType::NO_MOTION; // type of block
  Profile _profile;                       // feedrate profile
  bool _parsed = false;                   // has it been parsed?
  Machine const *_machine = nullptr;      // machine we're running on

  // UTILITIES =================================================================
  bool parse_token(string &token);

}; // class Block

} // namespace cncpp