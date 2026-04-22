/*
  ____  _            _           _
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/

Block class implementation
 */

#include "block.hpp"
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cncpp;
using namespace fmt;

// LIFECYCLE ==================================================================
Block::Block(string line) : _line(line), _n(0) {}

Block::Block(string line, Block &prev) : Block(line) { *this = prev; }

Block::~Block() { cerr << "Block " << _line << " destroyed." << endl; }

std::string Block::desc(bool colored) {
  if (!_parsed) {
    throw runtime_error("Block not parsed yet");
  }

  stringstream ss;
  auto color = color::green;
  if (_type == BlockType::NO_MOTION)
    color = color::gray;
  else if (_type == BlockType::RAPID)
    color = color::red;

  // remember fmt format strings:
  // {:} -> prints as is
  // {:>3} -> right align, 3 chars wide
  // {:0>2} -> pad with "0", right align, 2 chars wide
  // {:-^9} -> pad with "-", center align, 9 chars wide
  // {:>5.0f} -> right align, 5 chars wide, 0 decimals float
  ss << format("[{:>3}] ", _n);
  if (colored) {
    ss << format("G{:0>2} ", styled(static_cast<int>(_type), fg(color)));
  } else {
    ss << format("G{:0>2} ", static_cast<int>(_type));
  }
  ss << _target.desc(colored);
  ss << format(" F{:>5.0f} S{:>4.0f} ", _feedrate, _spindle);
  ss << format("T{:0>2} M{:0>2} ", _tool, _m);
  ss << format("L{:>6.2f}mm DT{:>6.2f}s", _length, _profile.dt);
  return ss.str();
}

Block &Block::operator=(Block &b) {
  if (!b._parsed)
    throw runtime_error("Previous block has not been parsed");
  _tool = b._tool;
  _feedrate = b._feedrate;
  _spindle = b._spindle;
  _n = b._n + 1;
  _target.reset();
  prev = &b;
  b.next = this;
  return *this;
}

// OPERATIONS/OPERATORS =======================================================

Block &Block::parse(const Machine *m) {
  _machine = m;
  stringstream ss(_line);
  string token;

  while (ss >> token) {
    try {
      if (!parse_token(token))
        break;
    } catch (runtime_error &e) {
      stringstream es;
      es << "Parsing error at line: " << _line << endl;
      es << "Token: " << token << endl;
      es << "Exception: " << e.what() << endl;
      throw runtime_error(es.str());
    }
  }
  // Modal behavior
  _target.modal(start_point());
  _delta = _target.delta(start_point());
  _acc = _machine->A();
  _length = _delta.length();

  // Calculate profile
  switch (_type) {
  case BlockType::LINE:
    // calculate segment profile
    _arc_feedrate = _feedrate;
    compute();
    break;

  case BlockType::CWA:
  case BlockType::CCWA:
    // calculate arc profile
    calc_arc();
    _arc_feedrate =
        min(_feedrate, pow(pow(_machine->A(), 2) * pow(_r, 2), 0.25) * 60);
    compute();
    break;

  default:
    break;
  }

  _parsed = true;
  return *this;
}

data_t Block::lambda(data_t time, data_t &speed) {
  if (!_parsed)
    throw runtime_error("Block not parsed");
  return _profile.lambda(time, speed);
}

Point Block::interpolate(data_t lambda) {
  if (!_parsed)
    throw runtime_error("Block not parsed");
  Point result = Point();
  Point p0 = start_point();

  // Interpolation:
  // x(t) = x(0) + d_x * lambda(t)
  // y(t) = y(0) + d_y * lambda(t)
  // z(t) = z(0) + d_z * lambda(t)

  // 1. block is a straight segment
  if (_type == BlockType::LINE) {
    result.x(p0.x() + _delta.x() * lambda);
    result.y(p0.y() + _delta.y() * lambda);
  }
  // Block is an arc
  else if (_type == BlockType::CWA || _type == BlockType::CCWA) {
    data_t angle = _theta_0 + _dtheta * lambda;
    result.x(_center.x() + _r * cos(angle));
    result.y(_center.y() + _r * sin(angle));
  }
  // Something is wrong
  else {
    throw runtime_error("Cannot interpolate block" + _line);
  }
  result.z(prev->target().z() + _delta.z() * lambda);
  return result;
}

Point Block::interpolate(data_t time, data_t &lambda, data_t &speed) {
  lambda = this->lambda(time, speed);
  return interpolate(lambda);
}

// b.walk([&](Block &b, data_t t, data_t l, data_t s) {
//   // do something with b, t, l, and s
// });
void Block::walk(
    std::function<void(Block &b, data_t t, data_t l, data_t s)> func) {
  data_t t = 0.0, l, s;
  while (t < _profile.dt) {
    l = lambda(t, s);
    func(*this, t, l, s);
    t += _machine->tq();
  }
}

// PRIVATE METHODS ============================================================

bool Block::parse_token(string &token) {
  bool res = true;
  char cmd = toupper(token[0]); // g -> G, and G -> G
  string arg = token.substr(1);

  if (cmd == '#' || cmd == ';') // comments
    return false;
  if (arg.empty()) // empty argument
    throw runtime_error("Empty command argument");

  switch (cmd) {
  case 'N':
    _n = stoi(arg);
    if (prev && _n <= prev->_n) // block number is not increasing
      throw runtime_error("Block number must be increasing: " +
                          to_string(prev->_n));
    break;

  case 'G':
    _type = static_cast<BlockType>(stoi(arg));
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
    throw runtime_error(ss.str());
    break;
  }

  return res;
}

Point Block::start_point() { return prev ? prev->target() : _machine->zero(); }

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
    f_m = (2 * l) / (dt_1 + dt_2 + 2 * dt_m);
  } else { // short block, trinagle
    dt_1 = dt_2 = sqrt(l / A);
    dt = _machine->quantize(dt_1 + dt_2, dq);
    dt_m = 0;
    dt_2 = dt_2 + dq;
    f_m = 2 * l / (dt_1 + dt_2);
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
  data_t x0, y0, z0, xc, yc, xf, yf, zf;
  Point p0 = start_point();
  x0 = p0.x();
  y0 = p0.y();
  z0 = p0.z();
  xf = _target.x();
  yf = _target.y();
  zf = _target.z();

  if (_r) { // if the radius is given
    data_t dx = _delta.x();
    data_t dy = _delta.y();
    data_t dxy2 = pow(dx, 2) + pow(dy, 2);
    data_t sq = sqrt(-pow(dy, 2) * dxy2 * (dxy2 - 4 * _r * _r));
    // signs table
    // sign(r) | CW(-1) | CCW(+1)
    // --------------------------
    //      -1 |     +  |    -
    //      +1 |     -  |    +
    int s = (_r > 0) - (_r < 0);
    s *= (_type == BlockType::CCWA ? 1 : -1);
    xc = x0 + (dx - s * sq / dxy2) / 2.0;
    yc = y0 + dy / 2.0 + s * (dx * sq) / (2 * dy * dxy2);
  } else { // if I,J are given
    data_t r2;
    _r = hypot(_i, _j);
    xc = x0 + _i;
    yc = y0 + _j;
    r2 = hypot(xf - xc, yf - yc);
    if (fabs(_r - r2) > _machine->error()) {
      throw runtime_error(
          fmt::format("Arc endpoints mismatch error ({:})", _r - r2).c_str());
    }
  }
  _center.x(xc);
  _center.y(yc);
  _theta_0 = atan2(y0 - yc, x0 - xc);
  _dtheta = atan2(yf - yc, xf - xc) - _theta_0;
  // we need the net angle so we take the 2PI complement if negative
  if (_dtheta < 0)
    _dtheta = 2 * M_PI + _dtheta;
  // if CW, take the negative complement
  if (_type == BlockType::CWA)
    _dtheta = -(2 * M_PI - _dtheta);
  //
  _length = hypot(zf - z0, _dtheta * _r);
  // from now on, it's safer to drop the sign of radius angle
  _r = fabs(_r);
}