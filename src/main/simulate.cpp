/*
  ____  _                 _       _       
 / ___|(_)_ __ ___  _   _| | __ _| |_ ___ 
 \___ \| | '_ ` _ \| | | | |/ _` | __/ _ \
  ___) | | | | | | | |_| | | (_| | ||  __/
 |____/|_|_| |_| |_|\__,_|_|\__,_|\__\___|
                                          
*/

#include "../cncpp.hpp"
#include <iostream>
#include <rang.hpp>
#include <fmt/core.h>

using namespace std;
using namespace cncpp;
using namespace rang;
using namespace fmt;

using bt = Block::BlockType;

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    cerr << style::bold << "Usage: " << argv[0] 
         << " <machine.yml> <program.gcode>" << style::reset << endl;
    return 1;
  }

  // Load machine
  Machine machine;
  try {
    machine.load(argv[1]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what()
         << style::reset << fg::reset << endl;
    return 2;
  }
  cerr << style::bold << "Machine: " << style::reset << endl
       << machine.desc() << endl;

  // Load part program
  Program program(&machine);
  try {
    program.load(argv[2]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what()
         << style::reset << fg::reset << endl;
    return 3;
  }

  cerr << style::bold << "Parsing program " << argv[2] << style::reset << endl
       << program.desc() << endl;


  // Go through the whole program, skipping over rapid blocks
  data_t t_tot = 0;
  Point pos;
  cout << "n,type,t_tot,t,lambda,feedrate,X,Y,Z" << endl;
  for (auto &b : program) {
    if (b.type() == bt::RAPID || b.type() == bt::NO_MOTION) {
      cerr << fg::yellow << "Skipping block " << b.line() << fg::reset << endl;
      continue;
    }
    b.walk([&](Block &b, data_t t, data_t lambda, data_t feedrate) {
      pos = b.interpolate(lambda);
      t_tot += machine.tq();

      cout << format("{:},{:},{:.3f},{:.3f},{:.6f},{:.3f},{:.3f},{:.3f},{:.3f}", 
                     b.n(), b.type_name(), t_tot, t, lambda, feedrate, 
                     pos.x(), pos.y(), pos.z())
           << endl;
    });
  }

  cerr << style::bold << "Done." << style::reset << endl;
  return 0;
}