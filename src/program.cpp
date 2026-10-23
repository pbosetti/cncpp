/*
  ____
 |  _ \ _ __ ___   __ _ _ __ __ _ _ __ ___
 | |_) | '__/ _ \ / _` | '__/ _` | '_ ` _ \
 |  __/| | | (_) | (_| | | | (_| | | | | | |
 |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|
                  |___/
*/

#include "program.hpp"
#include "defines.hpp"
#include <fstream>
#include <iterator>

using namespace std;
using namespace cncpp;

// LIFECYCLE ===================================================================
Program::Program(const std::string &f, Machine *m) : _machine(m), _filename(f) {
  load(_filename);
}

Program::Program(Machine *m) : _machine(m) {}

std::string Program::desc(bool colored) const {
  ostringstream ss;
  for (auto &current_block : _blocks) {
    ss << current_block.desc(colored) << endl;
  }
  return ss.str();
}

// OPERATORS/OPERATIONS ========================================================
void Program::load(const string &filename, bool append) {
  _filename = filename;

  ifstream file(_filename);
  if (!file.is_open()) {
    throw runtime_error("Could not open " + _filename);
  }
  if (!append)
    reset();
  string line;
  while (getline(file, line)) {
    size_t first = line.find_first_not_of(" \t\r");
    if (first == string::npos || line[first] == '#' || line[first] == ';')
      continue; // blank or comment line
    *this << line;
  }
}

Program &Program::operator<<(const std::string &line) {
  insert(_blocks.size(), line);
  return *this;
}

Block &Program::insert(size_t index, const std::string &line) {
  if (index > _blocks.size())
    throw out_of_range("Cannot insert block at position " + to_string(index));
  auto pos = next(_blocks.begin(), index);
  Block *before = (pos == _blocks.begin()) ? nullptr : &*std::prev(pos);
  Block *after = (pos == _blocks.end()) ? nullptr : &*pos;

  // link the new block between the two neighbors
  Block &b = *_blocks.emplace(pos, line);
  b._prev = before;
  b._next = after;
  if (before)
    before->_next = &b;
  if (after)
    after->_prev = &b;

  try {
    reparse_from(&b);
  } catch (...) {
    // unlink and remove the new block, then restore the following blocks
    if (before)
      before->_next = after;
    if (after)
      after->_prev = before;
    _blocks.erase(std::prev(pos));
    if (after)
      reparse_from(after);
    throw;
  }
  return b;
}

Block *Program::load_next() {
  if (!_started) {
    _started = true;
    _current = _blocks.empty() ? nullptr : &_blocks.front();
  } else if (_current) {
    _current = _current->next();
  }
  return _current;
}

void Program::rewind() {
  _current = nullptr;
  _started = false;
}

void Program::reset() {
  _blocks.clear();
  rewind();
}

// PRIVATE =====================================================================
void Program::reparse_from(Block *b) {
  for (; b; b = b->next())
    b->parse(_machine);
}

/*
  _____         _                     _
 |_   _|__  ___| |_   _ __ ___   __ _(_)_ __
   | |/ _ \/ __| __| | '_ ` _ \ / _` | | '_ \
   | |  __/\__ \ |_  | | | | | | (_| | | | | |
   |_|\___||___/\__| |_| |_| |_|\__,_|_|_| |_|

*/
#ifdef CNCPP_TEST_PROGRAM

#include <fmt/format.h>
#include <rang.hpp>
using namespace rang;
using namespace fmt;

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <machine.json> <file.g>" << endl;
    return EXIT_FAILURE;
  }
  string machine_data{argv[1]};
  Machine machine{machine_data};
  Program program{&machine};
  try {
    program.load(argv[2]);
  } catch (exception &e) {
    cerr << fg::red << "Error: " << e.what() << fg::reset << endl;
    return EXIT_FAILURE;
  }
  cerr << machine << endl;
  cerr << program << endl;

  cerr << "Sequence of position (to stdout only):" << endl;
  cout << "n,t_tot,t,lambda,s,x,y,z" << endl;
  // Loop over all the blocks here:
  data_t t_tot = 0.0;
  for (auto &block : program) {
    // skip rapid/nomotion blocks because those are not interpolated
    if (block.type() == Block::BlockType::RAPID || block.type() == Block::BlockType::NO_MOTION) continue;
    // loop within a block
    block.walk([&](Block &b, data_t t, data_t l, data_t s) {
      Point pos = b.interpolate(l);
      cout << fmt::format("{:},{:},{:},{:},{:},{:},{:},{:}", b.n(), t_tot, t, l, s,
                    pos.x(), pos.y(), pos.z())
          << endl;
      t_tot += machine.tq();
    });
  }

  return EXIT_SUCCESS;
}

#endif // CNCPP_TEST_PROGRAM