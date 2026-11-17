// Program class implementation

#include "program.hpp"
#include <fmt/format.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace cncpp;

/*
  _     _  __                     _
 | |   (_)/ _| ___  ___ _   _  ___| | ___
 | |   | | |_ / _ \/ __| | | |/ __| |/ _ \
 | |___| |  _|  __/ (__| |_| | (__| |  __/
 |_____|_|_|  \___|\___|\__, |\___|_|\___|
                        |___/
*/

Program::Program(const string &filename, Machine *machine)
    : Program(machine) {
  load(filename);
}

Program::Program(Machine *machine) : _machine(machine) {}

string Program::desc(bool colored) const {
  stringstream ss;
  for (auto const &b : _blocks)
    ss << b.desc(colored) << "\n";
  return ss.str();
}

/*
   ___                       _   _
  / _ \ _ __   ___ _ __ __ _| |_(_) ___  _ __  ___
 | | | | '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
 | |_| | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
  \___/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
       |_|
*/

void Program::load(const string &filename, bool append) {
  ifstream file(filename);
  if (!file)
    throw runtime_error("Cannot open file: " + filename);
  if (!append)
    reset();
  _filename = filename;

  string line;
  while (getline(file, line)) {
    // skip blank and comment-only lines: they would yield a block without N
    size_t first = line.find_first_not_of(" \t\r");
    if (first == string::npos || line[first] == ';' || line[first] == '#')
      continue;
    *this << line;
  }
}

Program &Program::operator<<(const string &line) {
  insert(_blocks.size(), line);
  return *this;
}

Block &Program::insert(size_t index, const string &line) {
  if (index > _blocks.size())
    throw out_of_range(fmt::format("Index {} out of range (size {})", index,
                                   _blocks.size()));

  auto it = _blocks.emplace(std::next(_blocks.begin(), index), line);
  Block &b = *it;
  b._prev = (it == _blocks.begin()) ? nullptr : &*std::prev(it);
  b._next = (std::next(it) == _blocks.end()) ? nullptr : &*std::next(it);
  if (b._prev)
    b._prev->_next = &b;
  if (b._next)
    b._next->_prev = &b;

  try {
    reparse_from(&b);
  } catch (...) {
    // roll back: unlink and drop the new block, restore the following ones
    Block *next = b._next;
    if (b._prev)
      b._prev->_next = next;
    if (next)
      next->_prev = b._prev;
    _blocks.erase(it);
    if (next)
      reparse_from(next);
    throw;
  }
  return b;
}

Block &Program::load_next() {
  if (!_started) {
    _started = true;
    _current = _blocks.empty() ? nullptr : &_blocks.front();
  } else if (_current) {
    _current = _current->_next;
  }
  if (!_current)
    throw out_of_range("No more blocks in program");
  return *_current;
}

void Program::rewind() {
  _current = nullptr;
  _started = false;
}

void Program::reset() {
  _blocks.clear();
  _filename.clear();
  rewind();
}

/*
  ____       _            _
 |  _ \ _ __(_)_   ____ _| |_ ___
 | |_) | '__| \ \ / / _` | __/ _ \
 |  __/| |  | |\ V / (_| | ||  __/
 |_|   |_|  |_| \_/ \__,_|\__\___|
*/

void Program::reparse_from(Block *b) {
  for (; b; b = b->_next)
    b->parse(_machine);
}

#ifdef CNCPP_TEST_PROGRAM

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <gcode file>" << endl;
    return 1;
  }

  Machine machine;
  Program program(&machine);
  try {
    program.load(argv[1]);
  } catch (exception const &e) {
    cerr << "Error loading " << argv[1] << ": " << e.what() << endl;
    return 1;
  }

  data_t t_tot = 0.0;
  cout << "t_tot,t_blk,n,lambda,speed,x,y,z" << endl;
  for (Block &block : program) {
    auto type = block.type();
    if (type != Block::BlockType::LINE && type != Block::BlockType::CWA &&
        type != Block::BlockType::CCWA)
      continue;
    block.walk([&](Block &b, data_t t, data_t l, data_t s) {
      Point p = b.interpolate(l);
      cout << fmt::format("{:.3f},{:.3f},{},{:.6f},{:.3f},{:.3f},{:.3f},{:.3f}",
                          t_tot + t, t, b.n(), l, s, p.x(), p.y(), p.z())
           << "\n";
    });
    t_tot += block.profile().dt;
  }
  return 0;
}

#endif // CNCPP_TEST_PROGRAM
