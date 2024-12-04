
#include "../cncpp.hpp"
#include "../fsm.hpp"
#include <thread>
#include <fmt/core.h>
#include <rang.hpp>

#include "../fsm.cpp"

using namespace std;
using namespace cncpp;
using namespace rang;

struct FSMData {
  Program program;
  Machine machine;
  FSMData(string ini_file) : machine(ini_file), program(&machine) {}
};

int main(int argc, char** argv) {
  FSMData data(argv[1]);
  FSM::FiniteStateMachine fsm(&data);
  try {
    data.program.load(argv[2]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what() << fg::reset
         << style::reset << endl;
    return 1;
  }
  cout << fg::green << style::bold << "MACHINE:" << style::reset 
       << fg::reset << endl;
  cout << data.machine.desc(true) << endl;
  cout << fg::green << style::bold << "PROGRAM:" << style::reset 
       << fg::reset << endl;
  cout << data.program.desc(true) << endl;

  fsm.set_timing_function([]() {
    this_thread::sleep_for(chrono::milliseconds(5));
  });
  // fsm.run(FSM::STATE_INIT);
  return 0;
}