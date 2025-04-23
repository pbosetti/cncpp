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
  Point p = Point();
  return p;
}


void Block::compute() {

}

void Block::calc_arc() {
  
}