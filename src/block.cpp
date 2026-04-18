/*
  ____  _            _           _               
 | __ )| | ___   ___| | __   ___| | __ _ ___ ___ 
 |  _ \| |/ _ \ / __| |/ /  / __| |/ _` / __/ __|
 | |_) | | (_) | (__|   <  | (__| | (_| \__ \__ \
 |____/|_|\___/ \___|_|\_\  \___|_|\__,_|___/___/
                                                 
Block class implementation
 */

#include "block.hpp"
#include <iostream>
#include <sstream>

 using namespace std;
 using namespace cncpp;


 // LIFECYCLE ==================================================================
Block::Block(string line) : _line(line), _n(0) {}

Block::Block(string line, Block &prev) : Block(line) {
  *this = prev;
}

Block::~Block() {
  cerr << "Block " << _line << " destroyed." << endl;
}

std::string Block::desc(bool colored = true) {

}

Block &Block::operator=(Block &b) {
  if (!b._parsed) throw runtime_error("Previous block has not been parsed");
  _tool = b._tool;
  _feedrate = b._feedrate;
  _spindle = b._spindle;
  _n = b._n + 1;
  _target.reset();
  prev = &b;
  b.next = this;
}



 // OPERATIONS/OPERATORS =======================================================

Block &Block::parse(const Machine *m) {
  _machine = m;
  stringstream ss(_line);
  string token;

  while ( ss >> token ) {
    try {
      if (!parse_token(token)) break;
    } catch (runtime_error &e) {
      stringstream es;
      es << "Parsing error at line: " << _line << endl;
      es << "Token: " << token << endl;
      es << "Exception: " << e.what() << endl;
      throw runtime_error(es.str());
    }
  }
  // Modal behavior
  _target.modal(prev->target());
  _delta = _target.delta(prev->target());
  // TODO: set _acc
  _length = _delta.length();

  // Calculate profile
  switch (_type)
  {
  case BlockType::LINE:
    // calculate segment profile
    break;
  
  case BlockType::CWA:
  case BlockType::CCWA:
    // calculate arc profile
    break;

  default:
    break;
  }

  _parsed = true;
  return *this;
}

data_t Block::lambda(data_t time, data_t &speed) {
  if (!_parsed) throw runtime_error("Block not parsed");
  return _profile.lambda(time, speed);
}

Point Block::interpolate(data_t lambda) {
  if (!_parsed) throw runtime_error("Block not parsed");
  Point result = Point();

  // Interpolation:
  // x(t) = x(0) + d_x * lambda(t)
  // y(t) = y(0) + d_y * lambda(t)
  // z(t) = z(0) + d_z * lambda(t)

  if (_type == BlockType::LINE) {
    result.x(prev->target().x() + _delta.x() * lambda);
    result.y(prev->target().y() + _delta.y() * lambda);
  } else if ( _type == BlockType::CWA || _type == BlockType::CCWA ) {
    
  } else {
    throw runtime_error("Cannot interpolate block" + _line);
  }
  result.z(prev->target().z() + _delta.z() * lambda);


  return result;
}

Point Block::interpolate(data_t time, data_t &lambda, data_t &speed) {

}

void Block::walk(std::function<void(Block &b, data_t t, data_t l, data_t s)> func) {

}




 // ACCESSORS ==================================================================





 // PRIVATE METHODS ============================================================

bool Block::parse_token(string &token) {
  bool res = true;

  return res;
}