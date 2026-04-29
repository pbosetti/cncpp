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

using namespace std;
using namespace cncpp;

Program::Program(const std::string &f, Machine *m) : _filename(f), _machine(m) {
  load(_filename);
}

Program::Program(Machine *m) : _machine(m) {}

Program::~Program() {}

std::string Program::desc(bool colored) const {
  ostringstream ss;
  for (auto &current_block : *this) {
    ss << current_block.desc(colored) << endl;
  }
  return ss.str();
}

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
    if (line[0] == '#')
      continue;
    *this << line;
  }
  file.close();
}

Program &Program::operator<<(const std::string &line) {
  if (size() > 0) { // this is not the first block
    emplace_back(line, back());
  } else { // this is the first block
    emplace_back(line);
  }
  back().parse(_machine);
  return *this;
}

block_iterator Program::load_next() {
  if (_current == end()) {
    _current = begin();
  } else {
    _current++;
  }
  _done = (_current == end());
  return _current;
}

void Program::rewind() {
  _current = begin();
  _done = false;
}

void Program::reset() {
  clear();
  rewind();
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
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <file.g>" << endl;
    return EXIT_FAILURE;
  }
  Machine machine{};
  Program program{&machine};
  try {
    program.load(argv[1]);
  } catch (exception &e) {
    cerr << fg::red << "Error: " << e.what() << fg::reset << endl;
    return EXIT_FAILURE;
  }

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
      cout << format("{:},{:},{:},{:},{:},{:},{:},{:}", b.n(), t_tot, t, l, s,
                    pos.x(), pos.y(), pos.z())
          << endl;
      t_tot += machine.tq();
    });
  }

  return EXIT_SUCCESS;
}

#endif // CNCPP_TEST_PROGRAM