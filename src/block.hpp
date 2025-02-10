/*
  ____  _            _           _                                       
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___                         
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|                        
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \                        
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/                        
                                                                         
Block class header file
Author: Paolo Bosetti, 2024
*/
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "defines.hpp"
#include "point.hpp"
#include "machine.hpp"
#include <map>

using namespace std;


namespace cncpp {

class Block final : Object {

  struct Profile {
    data_t a, d;             // actual accelerations
    data_t f, l;             // actual feedrate and length
    data_t fs, fe;           // initial and final feedrate
    data_t dt_1, dt_m, dt_2; // durations
    data_t dt;               // total duration
    data_t current_acc;       // current acceleration
    data_t lambda(data_t t, data_t &s);         // lambda function
  };

public:

  enum class BlockType {
    RAPID = 0, 
    LINE = 1, 
    CWA = 2, 
    CCWA = 3, 
    NO_MOTION
  };

  static const map<BlockType, string> types;

  // Lifecycle
  Block(string line, Block &prev);
  Block(string line);
  ~Block();
  string desc(bool colored = true) const override;

  // Methods
  void parse(const Machine *machine);
  data_t lambda(data_t time, data_t &speed);
  Point interpolate(data_t lambda);
  Point interpolate(data_t time, data_t &lambda, data_t &speed);

  // Accessors
  data_t dt() const { return _profile.dt; }
  data_t length() const { return _length; }
  data_t dtheta() const { return _dtheta; }
  BlockType type() const { return _type; }
  string line() const { return _line; }
  size_t n() const { return _n; }
  data_t r() const { return _r; }
  data_t feedrate() const { return _feedrate; }
  Point center() const { return _center; }
  Point target() const { return _target; }
  Profile profile() const { return _profile; }
  void walk(std::function<void(Block &, data_t t, data_t l, data_t s)> f);


private:
  // Attributes
  const Machine *_machine;
  Profile _profile;
  string _line;
  BlockType _type = BlockType::RAPID;
  size_t _n = 0;
  size_t _tool = 0;
  size_t _m = 0;
  data_t _feedrate = 0;
  data_t _arc_feedrate = 0;
  data_t _spindle = 0;
  Point _target = Point();
  Point _delta = Point();
  Point _center = Point();
  data_t _length = 0;
  data_t _i = 0, _j = 0, _r = 0;
  data_t _theta_0 = 0, _dtheta = 0;
  data_t _acc = 0;
  bool _parsed = false;

  void parse_token(string token);
  Point start_point() const;
  void compute();
  void arc();

public:
  Block *prev;
  Block *next;

};

} // namespace cncpp

#endif // BLOCK_HPP