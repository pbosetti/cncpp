/*
  ____  _            _           _               
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___ 
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/
  
Class Implementation
*/

#include "block.hpp"
#include "defines.hpp"
#include <fmt/color.h>
#include <fmt/format.h>
#include <rang.hpp>
#include <sstream>
#include <cmath>


using namespace std;
using namespace cncpp;
using namespace fmt;
using namespace rang;

// map from block types to string representation, to be used as in:
//   types[BlockType::RAPID] => "Rapid"
const map<Block::BlockType, string> Block::types = {
  {BlockType::RAPID, "Rapid"},
  {BlockType::LINE, "Line"},
  {BlockType::CWA, "CW Arc"},
  {BlockType::CCWA, "CCW Arc"},
  {BlockType::NO_MOTION, "No Motion"}
};

data_t Block::Profile::lambda(data_t t, data_t &s) {
  data_t r;
  current_acc = 0.0;

  if (t < 0) {
    r = 0.0;
    s = 0.0;
  } else if (t < dt_1) { // Acceleration
    r = a * pow(t, 2) / 2.0;
    s = a * t;
    current_acc = a;
  } else if (t < dt_1 + dt_m) { // Maintenance
    r = f * (dt_1 / 2.0 + (t - dt_1));
    s = f;
    current_acc = 0;
  } else if (t < dt_1 + dt_m + dt_2) { // Deceleration
    data_t t_2 = dt_1 + dt_m;
    r = f * dt_1 / 2.0 + f * (dt_m + t - t_2) +
        d / 2.0 *(pow(t, 2) + pow(t_2, 2)) - d * t * t_2;
    s = f + d * (t - t_2);
    current_acc = d;
  } else {
    r = 1.0;
    s = 0.0;
    current_acc = 0.0;
  }

  r /= l;
  s *= 60;
  return r;
}



/*
  ____        _     _ _        __  __      _   _               _     
 |  _ \ _   _| |__ | (_) ___  |  \/  | ___| |_| |__   ___   __| |___ 
 | |_) | | | | '_ \| | |/ __| | |\/| |/ _ \ __| '_ \ / _ \ / _` / __|
 |  __/| |_| | |_) | | | (__  | |  | |  __/ |_| | | | (_) | (_| \__ \
 |_|    \__,_|_.__/|_|_|\___| |_|  |_|\___|\__|_| |_|\___/ \__,_|___/
                                                                     
*/

// LIFECYCLE -----------------------------------------------------------------
Block::Block(string line) : _line(line), _n(0) {}

Block::Block(string line, Block &p) : Block(line) {
  *this = p; // copy from previous object
}

Block::~Block() {
  if (_debug) 
    cerr << style::italic << format("Block {:>3} destroyed.", _n)
         << style::reset << endl;
}

Block &Block::operator=(Block &b) {
  _tool = b._tool;
  _feedrate = b._feedrate;
  _spindle = b._spindle;
  _n = b._n + 1;
  _target.reset();
  b.next = this;
  prev = &b;
  return *this;
}

string Block::desc(bool colored) const {
  if (!_parsed) {
    return format("[{:>3}] {:} (not parsed yet)", _n, _line);
  }

  stringstream ss;
  // a color to be used for different block types
  auto color = color::red; // default for interpolated blocks
  if (_type == BlockType::NO_MOTION)
    color = color::gray;
  else if (_type == BlockType::RAPID)
    color = color::magenta;
  
  // fmt formatting fields:
  // {:}      -> prints number or string as-is
  // {:>3}    -> right align, 3 chars wide
  // {:0>2}   -> pad with "0", right align, 2 chars wide
  // {:-^9}   -> pad with "-", center align, 9 chars wide
  // {:>5.0f} -> right align, 5 chars wide, 0 decimals float
  ss << format("[{:>3}] ", _n);
  if (colored)
    ss << format("G{:0>2} ", styled(static_cast<int>(_type), fmt::fg(color)));
  else
    ss << format("G{:0>2} ", static_cast<int>(_type));
  ss << format("({:-^9}) ", Block::types.at(_type)) << _target.desc();
  ss << format(" F{:>5.0f} S{:>4.0f} ", _feedrate, _spindle);
  ss << format("T{:0>2} M{:0>2} ", _tool, _m);
  ss << format("L{:>6.2f}mm DT{:>6.2f}s", _length, _profile.dt);
  return ss.str();
}



// METHODS -------------------------------------------------------------------
void Block::parse(const Machine *m) {
  _machine = m;
  // initilize a stringstream with _line:
  stringstream ss(_line);
  string token;

  // ss >> token returns a word at a time; returns false at the end
  while (ss >> token) {
    try {
      // this is gonna be long, we factor it out to a dedicated private method:
      parse_token(token);
    } catch (CNCError &e) {
      stringstream ss;
      ss << "Parsing error at line: " << _line << endl;
      ss << "Token: " << token << endl;
      ss << "Exception: " << e.what() << endl;
      throw CNCError(ss.str(), this);
    }
  }

  // Modal (i.e. inherited) fields
  _target.modal(start_point());
  _delta = _target.delta(start_point());
  _acc = _machine->A();
  _length = _delta.length();

  // Interpolated motion
  switch (_type) {
    case BlockType::LINE:
      _acc = _machine->A();
      _arc_feedrate = _feedrate;
      compute();
      break;
    case BlockType::CWA:
    case BlockType::CCWA:
      calc_arc();
      _arc_feedrate = min(
        _feedrate,
        pow(3.0 / 4.0 * pow(_machine->A(), 2) * pow(_r, 2), 0.25) * 60
      );
      compute();
      break;
    default:
      break;
  }
  // done, set the flag:
  _parsed = true;
}

// Just a wrapper to the profile lambda:
data_t Block::lambda(data_t time, data_t &speed) {
  if (!_parsed) throw CNCError("Block not parsed", this);
  return _profile.lambda(time, speed);
}

Point Block::interpolate(data_t lambda) {
  if (!_parsed) throw CNCError("Block not parsed", this);
  Point result = Point();
  Point p0 = start_point();
  
  // 1. the block describes a segment
  // x(t) = x(0) + d_x * lambda(t)
  // y(t) = y(0) + d_y * lambda(t)
  // z(t) = z(0) + d_z * lambda(t)
  if (_type == BlockType::LINE) {
    result.x(p0.x() + _delta.x() * lambda);
    result.y(p0.y() + _delta.y() * lambda);
  }
  
  // 2. the block describes an arc
  else if (_type == BlockType::CWA || _type == BlockType::CCWA) {
    data_t angle = _theta_0 + _dtheta * lambda;
    result.x(_center.x() + _r * cos(angle));
    result.y(_center.y() + _r * sin(angle));
  }
  // Z increases linearly for both arcs and segments
  result.z(p0.z() + _delta.z() * lambda);
  return result;
}

Point Block::interpolate(data_t time, data_t &lambda, data_t &speed) {
  lambda = this->lambda(time, speed);
  return interpolate(lambda);
}

void Block::walk(function<void(Block const &b, data_t t, data_t l, data_t s)> f) {
  if (!_parsed) throw CNCError("Block not parsed", this);
  data_t t = 0.0, l, s;
  // 1.0 / 10.0 != 1.0
  while (t < _profile.dt + _machine->tq() / 2.0) {
    l = lambda(t, s);
    f(*this, t, l, s);
    t += _machine->tq();
  }
}





/*
  ____       _            _                        _   _               _     
 |  _ \ _ __(_)_   ____ _| |_ ___   _ __ ___   ___| |_| |__   ___   __| |___ 
 | |_) | '__| \ \ / / _` | __/ _ \ | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` / __|
 |  __/| |  | |\ V / (_| | ||  __/ | | | | | |  __/ |_| | | | (_) | (_| \__ \
 |_|   |_|  |_| \_/ \__,_|\__\___| |_| |_| |_|\___|\__|_| |_|\___/ \__,_|___/
                                                                             
*/
void Block::parse_token(string token) {
  char cmd = toupper(token[0]);
  string arg = token.substr(1);
  if (arg.empty()) throw CNCError("Empty command argument", this);
  // cover all possible/supported ISO commands:
  switch(cmd) {
  case 'N':
    _n = stoi(arg); // "123" => 123; "hello" => 0
    if (prev && _n <= prev->_n) 
      throw CNCError("Block number must be increasing: " + to_string(prev->_n), this);
    break;

  case 'G':
    _type = static_cast<BlockType>(stoi(arg));
    if (_type > BlockType::NO_MOTION)
      throw CNCError("Unknown G type", this);
    break;
    
  case 'X':
    _target.x(stod(arg));
    break;

  case 'Y':
    _target.y(stod(arg));
    break;

  case 'Z':
    _target.z(stod(arg));
    break;

  case 'I':
    _i = stod(arg);
    break;

  case 'J':
    _j = stod(arg);
    break;

  case 'R':
    _r = stod(arg);
    break;

  case 'F':
    _feedrate = stod(arg);
    break;

  case 'S':
    _spindle = stod(arg);
    break;

  case 'T':
    _tool = stoi(arg);
    break;

  case 'M':
    _m = stoi(arg);
    break;
  
  default:
    stringstream ss;
    ss << "Unknown/unsupported command: '" << token << "'";
    throw CNCError(ss.str(), this);
    break;
  }
}

Point Block::start_point() {
  return prev ? prev->target() : _machine->zero();
}


void Block::compute() {
  data_t dt, dt_1, dt_m, dt_2, dq;
  data_t f_m, &l = _length;
  data_t &A = _acc, a, d;

  f_m = _arc_feedrate / 60.0;
  dt_1 = f_m / A;
  dt_2 = dt_1;
  dt_m = l / f_m - (dt_1 + dt_2) / 2.0;

  if (dt_m > 0) { // long block, trapezoid
    dt = _machine->quantize(dt_1 + dt_m + dt_2, dq);
    dt_m = dt_m + dq;
    f_m = (2*l) / (dt_1 + dt_2 + 2*dt_m);
  } else { // short block, trinagle
    dt_1 = dt_2 = sqrt(l / A);
    dt = _machine->quantize(dt_1 + dt_2, dq);
    dt_m = 0;
    dt_2 = dt_2 + dq;
    f_m = 2*l / (dt_1 + dt_2);
  }
  a = f_m / dt_1;
  d = -(f_m / dt_2);
  _profile.dt_1 = dt_1;
  _profile.dt_2 = dt_2;
  _profile.dt_m = dt_m;
  _profile.a = a;
  _profile.d = d;
  _profile.f = f_m;
  _profile.dt = dt;
  _profile.l = l;
}

void Block::calc_arc() {
  
}



