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

class Block final : Object {
public:

  struct Profile {
    data_t a, d;                              // acceleration and deceleration
    data_t f, l;                              // feedrate and length
    data_t fs, fe;                            // start and end feedrate
    data_t dt_1, dt_m, dt_2;                  // partial times
    data_t dt;                                // total time
    data_t current_acc;                       // current acceleration on arc
    data_t lambda(data_t t, data_t &s);       // lambda function
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
  Block &operator=(Block &b); // b1 = b2; or b1.operator=(b2)

  // METHODS -------------------------------------------------------------------
  Block &parse(const Machine *m);
  data_t lambda(data_t time, data_t &speed);
  Point interpolate(data_t lambda);
  Point interpolate(data_t time, data_t &lambda, data_t &speed);
  void walk(function<void(Block &b, data_t t, data_t l, data_t s)> f);

  // ACCESSORS -----------------------------------------------------------------
  string line() const { return _line; }
  size_t n() const { return _n; }
  BlockType type() const { return _type; }
  string type_name() const { return types.at(_type); }
  size_t tool() const { return _tool; }
  data_t feedrate() const { return _feedrate; }
  data_t arc_feedrate() const { return _arc_feedrate; }
  data_t spindle() const { return _spindle; }
  data_t length() const { return _length; }
  Point target() const { return _target; }
  Point center() const { return _center; }
  Point delta() const { return _delta; }
  size_t m() const { return _m; }
  // We'll be able to use it as: b.profile().dt
  const Profile &profile() const { return _profile; }
  

private:
  const Machine *_machine = nullptr; //pointer to the machine object
  Profile _profile;                  // speed profile of the block
  BlockType _type = BlockType::NO_MOTION;
  string _line;                      // the original G-code line
  size_t _n = 0;                     // block number
  size_t _tool = 0;                  // tool number
  data_t _feedrate = 0;              // feedrate
  data_t _arc_feedrate = 0;          // feedrate for arcs
  data_t _spindle = 0;               // spindle speed
  Point _target = Point();           // target point
  Point _center = Point();           // center point for arcs
  Point _delta = Point();            // projections
  data_t _length = 0;                // length of the block
  data_t _i = 0, _j = 0, _r = 0;     // arc parameters
  data_t _theta_0 = 0, _dtheta = 0;  // arc angle parameters
  data_t _acc = 0;                   // actual acceleration
  size_t _m = 0;                     // M command
  bool _parsed = false;              // block has been parsed?

  // PRIVATE METHODS -----------------------------------------------------------
  void parse_token(string token);
  Point start_point(); // block starting point (prev target or machine init)
  void compute();      // velocity profile
  void calc_arc();     // calculate arc parameters

public:
  Block *prev = nullptr;
  Block *next = nullptr;

};


} // namespace cncpp

#endif // BLOCK_HPP