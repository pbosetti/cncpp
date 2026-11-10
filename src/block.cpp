// Block class implementation

#include "block.hpp"
#include <fmt/color.h>
#include <fmt/format.h>
#include <numbers>
#include <sstream>

using namespace std;
using namespace cncpp;
using namespace fmt;

using numbers::pi;

// UTILITY FUNCTIONS ===========================================================

static data_t to_number(string const &s) {
  size_t pos = 0;
  data_t v = stod(s, &pos);
  if (pos != s.size()) // s = "1.3X100"
    throw runtime_error("Invalid number: " + s);
  return v;
}

static int to_int(string const &s) {
  data_t v = to_number(s);
  if (v != floor(v))
    throw runtime_error("Expecting an integer: " + s);
  return static_cast<int>(v);
}

// PROFILE STRUCT ==============================================================

Block::Profile Block::Profile::trapezoid(data_t l, data_t f, data_t A,
                                         Machine const &m) {
  Profile p;
  p.l = l;
  if (p.l <= 0)
    return p;

  data_t f_m = f / 60.0; // mm/min -> mm/s
  data_t dq = 0.0;
  p.dt_1 = p.dt_2 = f_m / A;
  p.dt_m = l / f_m - (p.dt_1 + p.dt_2) / 2.0;

  // Long block, trapezoid
  if (p.dt_m > 0) {
    p.dt = m.quantize(p.dt_1 + p.dt_m + p.dt_2, dq);
    p.dt_m += dq;
  }
  // Short block, triangle
  else {
    p.dt_1 = p.dt_2 = sqrt(l / A);
    p.dt = m.quantize(p.dt_1 + p.dt_2, dq);
    p.dt_m = 0;
    p.dt_2 += dq;
  }

  p.f = (2 * l) / (p.dt_1 + p.dt_2 + 2 * p.dt_m);
  p.a = p.f / p.dt_1;
  p.d = -(p.f / p.dt_2);
  return p;
}

data_t Block::Profile::lambda(data_t t, data_t &s) {
  data_t r;
  current_acc = 0.0;

  if (l <= 0) { // zero-length block: already at the end
    s = 0.0;
    return 1.0;
  }

  if (t < 0) {
    r = 0.0;
    s = 0.0;
  } else if (t < dt_1) {
    r = a * pow(t, 2) / 2.0;
    s = a * t;
    current_acc = a;
  } else if (t < dt_1 + dt_m) {
    r = f * (dt_1 / 2.0 + (t - dt_1));
    s = f;
  } else if (t < dt_1 + dt_m + dt_2) {
    data_t t_2 = dt_1 + dt_m;
    r = f * dt_1 / 2.0 + f * (dt_m + t - t_2) +
        d / 2.0 * (pow(t, 2) + pow(t_2, 2)) - d * t * t_2;
    s = f + d * (t - t_2);
    current_acc = d;
  } else {
    r = l;
    s = 0.0;
  }

  r /= l;
  s *= 60;
  return r;
}

// LIFECYCLE ===================================================================

Block::Block(const string line) : _line(line) {}

Block::Block(const string line, Block &prev) : Block(line) {
  _prev = &prev;
  prev._next = this;
  // or, equivalently:
  // _prev->_next = this;
}

std::string Block::desc(bool colored) const {
  if (!_parsed)
    throw runtime_error("Block has not been parsed");

  stringstream ss;
  auto color = color::green;
  if (_type == BlockType::NO_MOTION)
    color = color::gray;
  else if (_type == BlockType::RAPID)
    color = color::red;

  // fmt codes are:
  // {:} -> print as is
  // {:>3} -> right align, 3 chars wide
  // {:0>2} -> right align, 2 chars, pad with 0
  // {:-^9} -> pad with "-", center align, 9 chars
  // {:>5.3f} -> right align, float with three decimals, 5 wide

  int gn = static_cast<int>(_type);
  ss << fmt::format("[{:>3}] ", _n); // -> "[ 10] "
  if (colored) {
    ss << fmt::format("G{:0>2} ", styled(gn, fmt::fg(color)));
  } else {
    ss << fmt::format("G{:0>2} ", gn);
  }
  ss << _target.desc(colored);
  ss << fmt::format(" F{:>5.0f} S{:>4.0f}", _feedrate, _spindle);
  ss << fmt::format(" T{:0>2} M{:0>2}", _tool, _m);
  ss << fmt::format(" L{:>6.2f}mm DT{:>6.2f}", _length, _profile.dt);

  return ss.str();
}

// INTERFACE ===================================================================

Block &Block::parse(const Machine *m) {
  if (!m)
    throw runtime_error("Cannot parse a block without a Machine");

  stringstream ss(_line);
  string token;

  if (_prev) {
    if (!_prev->_parsed)
      throw runtime_error("Previous block has not been parsed");
    _type = _prev->_type;
    _tool = _prev->_tool;
    _feedrate = _prev->_feedrate;
    _spindle = _prev->_spindle;
    _start = _prev->_target;
  } else {
    _start = m->zero();
  }

  while (ss >> token) {
    if (token[0] == '#' || token[0] == ';') {
      break;
    }
    parse_token(token); // token is "G01"
  }

  _target.modal(_start);
  _delta = _target - _start;
  _length = _delta.length();
  _tq = m->tq();

  // Calculate the velocity profile
  data_t feed = min(_feedrate, m->fmax());

  switch (_type) {
  case BlockType::LINE:
    if (feed <= 0)
      throw runtime_error("Feedrate must be strictly positive");
    _profile = Profile::trapezoid(_length, feed, m->A(), *m);
    break;

  case BlockType::CWA:
  case BlockType::CCWA:
    if (feed <= 0)
      throw runtime_error("Feedrate must be strictly positive");
    calc_arc(m->max_error());
    feed = min(feed, sqrt(m->A() * _r) * 60);
    _profile = Profile::trapezoid(_length, feed, m->A(), *m);
    break;

  default:
    break;
  }

  _parsed = true;
  return *this;
}

data_t Block::lambda(data_t time, data_t &speed) {
  if (!_parsed)
    throw runtime_error("Block has not been parsed");
  return _profile.lambda(time, speed);
}

Point Block::interpolate(data_t lambda) {
  if (!_parsed)
    throw runtime_error("Block has not been parsed");
  Point result = Point();

  // 1. block is a line segment
  if (_type == BlockType::LINE) {
    result.x(_start.x() + _delta.x() * lambda);
    result.y(_start.y() + _delta.y() * lambda);
  }
  // 2. block is an arc
  else if (_type == BlockType::CWA || _type == BlockType::CCWA) {
    data_t angle = _theta_0 + _dtheta * lambda;
    result.x(_center.x() + _r * cos(angle));
    result.y(_center.y() + _r * sin(angle));
  }
  // 3. block cannot be interpolated
  else {
    throw runtime_error("Cannot interpolate block :" + _line);
  }
  result.z(_start.z() + _delta.z() * lambda);
  return result;
}

Point Block::interpolate(data_t time, data_t &lambda, data_t &speed) {
  lambda = this->lambda(time, speed);
  return interpolate(lambda);
}

void Block::walk(
    std::function<void(Block &b, data_t time, data_t distance, data_t feedrate)> func) {

  if (!_parsed)
    throw runtime_error("Block has not been parsed");

  data_t speed = 0.0, t = 0.0, l = 0.0;
  size_t steps = static_cast<size_t>(llround(_profile.dt / _tq));

  for (size_t i = 0; i <= steps; i++) {
    t = i * _tq;
    l = lambda(t, speed);
    func(*this, t, l, speed);
  }
}

// PRIVATE METHODS =============================================================

void Block::parse_token(string const &t) {
  char cmd = toupper(t[0]); // g -> G, G -> G
  string arg = t.substr(1);

  if (arg.empty())
    throw runtime_error("Empty command arguments are not accepted");

  switch (cmd) {
  case 'N':
    _n = to_int(arg);
    if (_n < min_n())
      throw runtime_error("Bloclk numbers must be increasing");
    break;

  case 'G': {
    int g = to_int(arg);
    if (g < static_cast<int>(BlockType::RAPID) ||
        g > static_cast<int>(BlockType::NO_MOTION))
      throw runtime_error("Unsupported G code: G" + arg);
    _type = static_cast<BlockType>(g);
    break;
  }

  case 'X':
    _target.x(to_number(arg));
    break;

  case 'Y':
    _target.y(to_number(arg));
    break;

  case 'Z':
    _target.z(to_number(arg));
    break;

  case 'I':
    _i = to_number(arg);
    break;

  case 'J':
    _j = to_number(arg);
    break;

  case 'R':
    _r = to_number(arg);
    break;

  case 'F':
    _feedrate = to_number(arg);
    break;

  case 'S':
    _spindle = to_number(arg);
    break;

  case 'T':
    _tool = to_int(arg);
    break;

  case 'M':
    _m = to_int(arg);
    break;

  default:
    throw runtime_error("Unknown/usupported command: '" + t + "'");
  }
}

size_t Block::min_n() {
  if (_prev)
    return _prev->_n + 1;
  else
    return 1;
}

void Block::calc_arc(data_t max_error) {
  data_t x0, y0, xc, yc, xf, yf;
  x0 = _start.x();
  y0 = _start.y();
  xf = _target.x();
  yf = _target.y();

  if (_r) { // if the radius is given
    data_t dx = _delta.x();
    data_t dy = _delta.y();
    // signs table
    // sign(r) | CW(-1) | CCW(+1)
    // --------------------------
    //      -1 |     +  |    -
    //      +1 |     -  |    +
    int s = (_r > 0) - (_r < 0);
    s *= (_type == BlockType::CCWA ? 1 : -1);
    data_t d = hypot(dx, dy);
    data_t sq = sqrt(pow(_r, 2) - pow(d, 2) / 4.0) / d;
    xc = (x0 + xf) / 2.0 - s * dy * sq;
    yc = (y0 + yf) / 2.0 + s * dx * sq;

  } else { // if I,J are given
    data_t r2;
    _r = hypot(_i, _j);
    xc = x0 + _i;
    yc = y0 + _j;
    r2 = hypot(xf - xc, yf - yc);
    if (fabs(_r - r2) > max_error) {
      throw runtime_error(
          fmt::format("Block {:}: Arc endpoints mismatch error ({:} vs: {:})",
                      _n, _r - r2, max_error));
    }
  }
  _center.x(xc);
  _center.y(yc);
  _theta_0 = atan2(y0 - yc, x0 - xc);
  _dtheta = atan2(yf - yc, xf - xc) - _theta_0;
  // we need the net angle: positive for CCW, negative for CW. When start and
  // end points coincide, the arc is a full circle
  if (_type == BlockType::CCWA && _dtheta <= 0)
    _dtheta += 2 * pi;
  else if (_type == BlockType::CWA && _dtheta >= 0)
    _dtheta -= 2 * pi;
  // from now on, it's safer to drop the sign of radius angle
  _r = fabs(_r);
  // helical motion: the Z travel adds to the arc length
  _length = hypot(_dtheta * _r, _delta.z());
  if (isnan(_length) || isinf(_length)) {
    throw runtime_error(
        fmt::format("Block {:}: Invalid arc radius: {}", _n, _r));
  }
}



#ifdef CNCPP_TEST_BLOCK

// To test with complot:
// complot p1 --aes "x=x y=y"
// build/block_test | complot p1: --type lines -

int main() {
  Machine m{};

  Block b1{"N01 G00 X100 Y100 z200"};
  Block b2{"N02 G00 Z150", b1.parse(&m)};
  Block b3{"n3 g01 x50 y20 z110 t1 f1000 s200 M3", b2.parse(&m)};
  Block b4{"n4 G03 x40 y10 r10", b3.parse(&m)};
  b4.parse(&m);

  cerr << b1 << endl << b2 << endl << b3 << endl << b4 << endl;
  cout << "t,lambda,feedrate,x,y,z" << endl;
  b4.walk([](Block &b, data_t t, data_t l, data_t s) {
    auto p = b.interpolate(l);
    cout << fmt::format("{:},{:},{:},{:},{:},{:}", t, l, s, p.x(), p.y(), p.z()) << endl;
  });

  return 0;
}

#endif