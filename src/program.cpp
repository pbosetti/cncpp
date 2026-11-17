#include "program.hpp"
#include <sstream>
#include <iterator>
#include <fstream>

using namespace std;
using namespace cncpp;


// Lifecycle: ==================================================================

Program::Program(const std::string &f, Machine *m) : Program(m) {
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


// Operations: =================================================================

Program &Program::operator<<(const string &line) {
  insert(_blocks.size(), line);
  return *this;
}


Block &Program::insert(size_t index, const string &line) {
  if (index > _blocks.size())
    throw out_of_range("Cannot insert block at position " + to_string(index));

  // get the iterator to the next element (possibly a new one)
  auto pos = std::next(_blocks.begin(), index);
  // pointers to previous and next blocks
  Block *before = (pos == _blocks.begin()) ? nullptr : &*std::prev(pos);
  Block *after = (pos == _blocks.end()) ? nullptr : &*pos;

  // Emplace a new Block instance in the new position
  Block &b = *_blocks.emplace(pos, line);
  b._prev = before;
  b._next = after;
  // adjust pointers of neighboring blocks
  if (before) before->_next = &b;
  if (after) after->_prev = &b;

  // reparse the program from the newly inserted block onward
  try {
    reparse_from(&b);
  } catch (...) {
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


void Program::load(const string &filename, bool append) {
  _filename = filename;
  ifstream file(_filename);
  if (!file.is_open()) {
    throw runtime_error("Could not open " + _filename);
  }
  if (!append) reset();

  string line;
  while (getline(file, line)) {
    // find first occurrence of a non-whitespace character
    size_t first = line.find_first_not_of(" \t\r\n");
    if (
      first == string::npos || // first occurence is at the end of the longest possible string
      line[first] == '#' || // first character is a #
      line[first] == ';'    // first character is a ;
    ) continue;
    *this << line;
  }
}



Block &Program::load_next() {
  if (!_started) {
    _started = true;
    _current = _blocks.empty() ? nullptr : &_blocks.front();
  } else if (_current) {
    _current = _current->next();
  }
  if (!_current)
    throw out_of_range("Invalid block selected");
  return *_current;
}

void Program::rewind() {
  _current = nullptr;
  _started = false;
}

void Program::reset() {
  _blocks.clear();
  rewind();
}


// Private methods =============================================================
void Program::reparse_from(Block *b) {
  for (; b; b = b->next()) {
    b->parse(_machine);
  }
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

int main(int argc, const char *argv[]) {
  // requires exactly one command line argument: the filename of the G-code 
  // program to load
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return EXIT_FAILURE;
  }

  Machine machine{};
  Program program{&machine};
  
  // Try loading the program or exit
  try {
    program.load(argv[1]);
  } catch (exception &e) {
    cerr << "Error loading program: " << e.what() << endl;
    cerr << "Program state after loading attempt:" << endl;
    cerr << program << endl;
    return EXIT_FAILURE;
  }
  cerr << program << endl;

  // Loop over all blocks and print the interpolated points at each time quantum
  data_t t_tot = 0.0;
  cout << "n,t_tot,t,lambda,speed,x,y,z" << endl;
  for (auto &block : program) {
    // jump over non-interpolating blocks
    if (block.type() == Block::BlockType::RAPID || block.type() == Block::BlockType::NO_MOTION) continue;
    // loop over a block positioning sequence
    block.walk([&](Block &b, data_t t, data_t l, data_t s) {
      Point p = b.interpolate(l);
      cout << fmt::format("{:},{:},{:},{:},{:},{:},{:},{:}", b.n(), t_tot, t, l, s, p.x(), p.y(), p.z()) << endl;
      t_tot += machine.tq();
    });
  }

  return EXIT_SUCCESS;
}


#endif // CNCPP_TEST_PROGRAM