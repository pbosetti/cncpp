// Block class implementation

#include "block.hpp"
#include <fmt/format.h>
#include <fmt/color.h>
#include <sstream>

using namespace std;
using namespace cncpp;
using namespace fmt;



// LIFECYCLE ===================================================================

Block::Block(string &line) : _line(line) {}

Block::Block(string &line, Block &prev) : Block(line) {
  _prev = &prev;
  prev._next = this;
  // or, equivalently:
  // _prev->_next = this;
}


std::string Block::desc(bool colored) const {}

Block &Block::parse(const Machine *m) {
  if (!m) throw runtime_error("Cannot parse a block without a Machine");

  stringstream ss(_line);
  string token;

  while (ss >> token) {
    if (token[0] == "#" || token[0] == ";") {
      break;
    }
    parse_token(token);
  }

}



// UTILITIES ===================================================================

void Block::parse_token(string const &t) {
  
}