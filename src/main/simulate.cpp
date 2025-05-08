
#include "../cncpp.hpp"
#include <iostream>
#include <rang.hpp>
#include <fmt/core.h>

using namespace std;
using namespace cncpp;
using namespace fmt;
using namespace rang;

using bt = Block::BlockType;


int main(int argc, const char *argv[]) {
  if (argc < 3) {
    cerr << style::bold << "Usage: " << argv[0] 
         << " <machine.yml> <program.gcode>" << style::reset << endl;
    return 1;
  }

  // Load Machine
  Machine machine;
  try {
    machine.load("machine.yml");
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what()
         << style::reset << fg::reset << endl;
    return 1;
  }
  cerr << style::bold << "Machine: " << style::reset << endl
       << machine.desc() << endl;

  // Load program
  Program program(&machine);
  try {
    program.load(argv[2]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what()
         << style::reset << fg::reset << endl;
    return 2;
  }

  cerr << style::bold << "Parsing program " << argv[2] << style::reset << endl;
  cerr << program.desc() << endl;

  cerr << style::bold << "Simulating to CSV format..." << style::reset << endl;

  data_t t_tot = 0;
  Point pos;
  cout << "n,type,t_tot,t,lambda,feedrate,X,Y,Z" << endl;
  for (auto &b : program) {
    if (b.type() == bt::RAPID || b.type() == bt::NO_MOTION) {
      cerr << fg::yellow << "Skipping block " << b.line() << fg::reset << endl;
      continue;
    }
    b.walk([&](Block &b, data_t t, data_t lambda, data_t s) {
      pos = b.interpolate(lambda);
      t_tot += machine.tq();

      cout << format("{:},{:},{:.3f},{:.3f},{:.6f},{:.3f},{:.3f},{:.3f},{:.3f}\n", b.n(), b.type_name(), t_tot, t, lambda, s, pos.x(), pos.y(), pos.z());
    });
  }


  cerr << style::bold << "Done." << style::reset << endl;
  return 0;
}