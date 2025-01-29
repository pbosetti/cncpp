
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
  data_t t_tot;
  data_t t_blk;
  FSMData(string ini_file) : machine(ini_file), program(&machine) {}
};


int main(int argc, char** argv) {
  FSMData data(argv[1]);
  FSM::FiniteStateMachine fsm(&data);
  openlog("CCNC v" VERSION, LOG_PID, LOG_USER);
  syslog(LOG_INFO, "Starting CNC program");
  try {
    data.program.load(argv[2]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what() << fg::reset
         << style::reset << endl;
    return 1;
  }
  cerr << fg::green << style::bold << "MACHINE:" << style::reset 
       << fg::reset << endl
       << data.machine.desc(true) << endl
       << fg::green << style::bold << "PROGRAM:" << style::reset 
       << fg::reset << endl
       << data.program.desc(true) << endl;

  fsm.set_timing_function([]() {
    this_thread::sleep_for(chrono::milliseconds(5));
  });
#ifdef DEBUG
  fsm.run([&fsm](FSMData &d){
    cout << "State: " << fsm.state_name() << endl;
  });
#else
  fsm.run();
#endif
  return 0;
}