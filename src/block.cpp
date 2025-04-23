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
  return 0;
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
  p.next = this;
  prev = &p;
  // Reset non-modal parameters
  _type = BlockType::NO_MOTION;
  _target.reset();
  _n = prev->n() + 1;
  _line = line;
}

Block::~Block() {
  if (_debug) 
    cerr << style::italic << format("Block {:>3} destroyed.", _n)
         << style::reset << endl;
}

string Block::desc(bool colored) const {
  if (!_parsed) {
    // [  7] G00 x100 (not parsed yet)
    return format("[{:>3}] {:} (not parsed yet)", _n, _line);
  }

  stringstream ss;
  auto color = color::red;
  ss << format("[{:>3}] ", _n);
  if (_type == BlockType::NO_MOTION)
    color = color::gray;
  else if (_type == BlockType::RAPID)
    color = color::magenta;
  ss << format("G{:0>2} ", styled(static_cast<int>(_type), fmt::fg(color)));
  ss << format("({:-^9}) ", Block::types.at(_type)) << _target.desc();
  ss << format(" F{:>5.0f} S{:>4.0f} ", _feedrate, _spindle);
  ss << format("T{:0>2} M{:0>2} ", _tool, _m);
  ss << format("L{:>6.2f}mm DT{:>6.2f}s", _length, _profile.dt);
  return ss.str();
}



// METHODS -------------------------------------------------------------------
void Block::parse(const Machine *m) {
  _machine = m;
  stringstream ss(_line);
  string token;

  while (ss >> token) {
    try {
      parse_token(token);
    } catch (CNCError &e) {
      stringstream ss;
      ss << "Parsing error at line: " << _line << endl;
      ss << "Token: " << token << endl;
      ss << "Exception: " << e.what() << endl;
      throw CNCError(ss.str(), this);
    }
  }

  // Modal fields
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

  _parsed = true;
}

data_t Block::lambda(data_t time, data_t &speed) {
  return _profile.lambda(time, speed);
}

Point Block::interpolate(data_t lambda) const {
  Point p = Point();
  return p;
}

Point Block::interpolate(data_t time, data_t &lambda, data_t &speed) const {
  Point p = Point();
  return p;
}

void Block::walk(function<void(Block const &b, data_t t, data_t l, data_t s)> f) const {

}





/*
  ____       _            _                        _   _               _     
 |  _ \ _ __(_)_   ____ _| |_ ___   _ __ ___   ___| |_| |__   ___   __| |___ 
 | |_) | '__| \ \ / / _` | __/ _ \ | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` / __|
 |  __/| |  | |\ V / (_| | ||  __/ | | | | | |  __/ |_| | | | (_) | (_| \__ \
 |_|   |_|  |_| \_/ \__,_|\__\___| |_| |_| |_|\___|\__|_| |_|\___/ \__,_|___/
                                                                             
*/
void Block::parse_token(string token) {

}

Point Block::start_point() {

}


void Block::compute() {

}

void Block::calc_arc() {
  
}