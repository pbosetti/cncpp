

#include "block.hpp"
#include "cncpp.hpp"
#include <fmt/color.h>
#include <fmt/format.h>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <rang.hpp>
#include <sstream>

using namespace std;
using namespace cncpp;
using namespace fmt;

const map<Block::BlockType, string> Block::types = {
    {BlockType::RAPID, "Rapid"},
    {BlockType::LINE, "Line"},
    {BlockType::CWA, "CW Arc"},
    {BlockType::CCWA, "CCW Arc"},
    {BlockType::NO_MOTION, "No Motion"}};

data_t Block::Profile::lambda(data_t t, data_t &s) {
  data_t r;
  current_acc = 0.0;
  if (t < 0) {
    r = 0.0;
    s = 0.0;
  } else if (t < dt_1) { // acceleration
    r = a * pow(t, 2) / 2.0;
    s = a * t;
    current_acc = a;
  } else if (t < dt_1 + dt_m) { // maintenance
    r = f * (dt_1 / 2.0 + (t - dt_1));
    s = f;
    current_acc = 0.0;
  } else if (t < dt_1 + dt_m + dt_2) { // deceleration
    data_t t_2 = dt_1 + dt_m;
    r = f * dt_1 / 2.0 + f * (dt_m + t - t_2) +
        d / 2.0 * (pow(t, 2) + pow(t_2, 2)) - d * t * t_2;
    s = f + d * (t - t_2);
    current_acc = d;
  } else {
    r = 1.0;
    s = 0.0;
    current_acc = 0.0;
    return r;
  }

  r /= l;
  s *= 60; // convert to mm/min
  return r;
}

/*
  ____        _     _ _                       _   _               _
 |  _ \ _   _| |__ | (_) ___   _ __ ___   ___| |_| |__   ___   __| |___
 | |_) | | | | '_ \| | |/ __| | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` / __|
 |  __/| |_| | |_) | | | (__  | | | | | |  __/ |_| | | | (_) | (_| \__ \
 |_|    \__,_|_.__/|_|_|\___| |_| |_| |_|\___|\__|_| |_|\___/ \__,_|___/

*/
Block::Block(string line) : _line(line), _n(0) {}

Block::Block(string line, Block &p) : Block(line) {
  *this = p;
  p.next = this;
  prev = &p;
  // Reset non-modal parameters:
  _type = BlockType::NO_MOTION;
  _target.reset();
  _n = prev->n() + 1;
  _line = line;
  _parsed = false;
  _m = 0;
}

Block::~Block() {
  if (_debug)
    cout << rang::style::italic << fmt::format("Block {:>3} destroyed.", _n)
         << rang::style::reset << endl;
}

data_t Block::lambda(data_t t, data_t &s) {
  return _profile.lambda(t, s);
}

void Block::parse(const Machine *machine) {
  _machine = machine;
  stringstream ss(_line);
  string token;

  while (ss >> token) {
    parse_token(token);
  }

  _target.modal(start_point());
  _delta = _target.delta(start_point());
  _acc = _machine->A();
  _length = _delta.length();

  switch (_type) {
  case BlockType::LINE:
    _acc = _machine->A();
    _arc_feedrate = _feedrate;
    compute();
    break;
  case BlockType::CWA:
  case BlockType::CCWA:
    arc();
    _arc_feedrate =
        min(_feedrate,
            pow(3.0 / 4.0 * pow(_machine->A(), 2) * pow(_r, 2), 0.25) * 60);
    compute();
    break;
  default:
    break;
  }

  _parsed = true;
}

Point Block::interpolate(data_t lambda) {
  Point result = _machine->setpoint();
  Point p0 = start_point();

  // 1. the block describes a segment
  // x(t) = x(0) + d_x * lambda(t)
  // y(t) = y(0) + d_y * lambda(t)
  if (_type == BlockType::LINE) {
    result.x(p0.x() + _delta.x() * lambda);
    result.y(p0.y() + _delta.y() * lambda);
  }

  // 2. the block describes an arc
  // x(t) = x_c + R cos(theta_0 + dtheta * lambda(t))
  // y(t) = y_c + R sin(theta_0 + dtheta * lambda(t))
  else if (_type == BlockType::CWA || _type == BlockType::CCWA) {
    data_t angle = _theta_0 + _dtheta * lambda;
    result.x(_center.x() + _r * cos(angle));
    result.y(_center.y() + _r * sin(angle));
  } else {
    throw(CNCError("Unexpected block type in interpolation", this));
  }
  result.z(p0.z() + _delta.z() * lambda);
  return result;
}

Point Block::interpolate(data_t time, data_t &lambda, data_t &speed) {
  lambda = this->lambda(time, speed);
  return interpolate(lambda);
}

string Block::desc(bool colored) const {
  if (!_parsed) {
    return fmt::format("[{:0>3}] {:} (parse error)", _n, _line);
  }
  stringstream ss;
  auto color = color::red;
  using col_t = optional<fmt::color>;
  ss << format("[{:0>3}] ", _n);
  if (_type == BlockType::NO_MOTION) {
    color = color::gray;
  } else if (_type == BlockType::RAPID) {
    color = color::magenta;
  } else {
    color = color::cyan;
  }
  ss << format("G{:0>2} ", styled(static_cast<int>(_type), fg(color)))
     << format("({:-^9}) ", Block::types.at(_type)) << target().desc()
     << format(" F{:>5.0f} ", _feedrate) << format("S{:>4.0f} ", _spindle)
     << format("T{:0>2} ", _tool) << format("M{:0>2} ", _m)
     << format("L{:>6.2f}mm ", _length) << format("DT{:>6.2f}s ", _profile.dt);
  return ss.str();
}

void Block::walk(std::function<void(Block &, data_t t, data_t l, data_t s)> f) {
  data_t t = 0, l, s;
  while (t < _profile.dt + _machine->tq()) {
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
  switch (cmd) {
  case 'N':
    _n = stoi(arg);
    if (prev && _n <= prev->n()) {
      throw CNCError("Block number must be greater than previous block", this);
    }
    break;

  case 'G':
    _type = static_cast<BlockType>(stoi(arg));
    if (_type > BlockType::NO_MOTION) {
      throw CNCError("Unknown G-code", this);
    }
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
    ss << "Unknown token '" << token << "'";
    throw CNCError(ss.str().c_str(), this);
    break;
  }
}

Point Block::start_point() const {
  return prev ? prev->target() : _machine->zero();
}

void Block::compute() {
  data_t &A = _acc, a, d;
  data_t dt, dt_1, dt_2, dt_m, dq;
  data_t f_m, &l = _length;

  f_m = _arc_feedrate / 60.0;
  dt_1 = f_m / A;
  dt_2 = dt_1;
  dt_m = l / f_m - (dt_1 + dt_2) / 2.0;

  if (dt_m > 0) { // Trapezoidal profile
    dt = _machine->quantize(dt_1 + dt_m + dt_2, dq);
    dt_m = dt_m + dq;
    f_m = (2 * l) / (dt_1 + dt_2 + 2 * dt_m);
  } else { // Triangular profile
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

void Block::arc() {
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
      throw CNCError(
          fmt::format("Arc endpoints mismatch error ({:})", _r - r2).c_str(),
          this);
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

/*
  _____         _
 |_   _|__  ___| |_
   | |/ _ \/ __| __|
   | |  __/\__ \ |_
   |_|\___||___/\__|

*/

#ifdef BLOCK_MAIN
#include <iostream>
#include <fstream>

class Blocks {
public:
  Blocks(Machine *machine) : _machine(machine) {
    cout << _machine->desc() << endl;
  }

  ~Blocks() { cout << "Destroying blocks list..." << endl; }

  Blocks &operator<<(string line) {
    if (_blocks.size() > 0) {
      _blocks.emplace_back(line, _blocks.back());
    } else {
      _blocks.emplace_back(line);
    }
    _blocks.back().parse(_machine);
    return *this;
  }

  void inspect() {
    for (auto &b : _blocks) {
      cout << b.desc()
           << ", previous: " << (b.prev ? to_string(b.prev->n()) : "(null)")
           << endl;
    }
  }

  Block &last() { return _blocks.back(); }

private:
  list<Block> _blocks;
  Machine *_machine;
};

int main() {
  Machine machine("machine.ini");
  Blocks blocks(&machine);
  Block *test_block = nullptr;

  try {
    blocks << "N01 G00 X1 Y1 Z1";
    blocks << "N02 G1 Y2 Z2 T10 F2000"
           << "G01 X200";
    test_block = &blocks.last();

    blocks << "N4 G01 z100.3 y150 F3000";
    blocks << "G50 M10"
           << "g00 x100 y100 z100";
    ;
  } catch (CNCError &e) {
    cerr << "Error: " << e.what() << " in " << e.who() << endl;
  }

  blocks.inspect();

  cout << "Test block: " << test_block->desc() << endl;

  ofstream out("test.csv");

  out << "T, lambda, speed, acc, X, Y, Z" << endl;
  test_block->walk([&out](Block &b, data_t t, data_t l, data_t s) {
    Point pos = b.interpolate(l);
    out << fmt::format("{:6.3f}, {:7.6f}, {:7.3f}, {:7.3f}, {:7.3f}, {:7.3f}, {:7.3f}", t, l, s, b.profile().current_acc, pos.x(), pos.y(), pos.z()) << endl;
  });
  out.close();

  cout << "Exiting..." << endl;
}

#endif