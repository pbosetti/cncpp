/*
   ____ _   _  ____ ____  ____                  
  / ___| \ | |/ ___|  _ \|  _ \    _____  _____ 
 | |   |  \| | |   | |_) | |_) |  / _ \ \/ / _ \
 | |___| |\  | |___|  __/|  __/  |  __/>  <  __/
  \____|_| \_|\____|_|   |_|      \___/_/\_\___|
                                                
Main executable.
Usage: cncpp <machine.yml> <program.gcode>
*/

#include "../cncpp.hpp"
#include "../fsm.hpp"
#include "../timer.hpp"
#include <iostream>
#include <rang.hpp>
#include <chrono>


using namespace std;
using namespace rang;
using namespace cncpp;
using namespace chrono;

// Where to store long-lasting objects. This instance will be available to
// all FSM state and transition functions
struct FSMData {
  Program program;
  Machine machine;
  data_t t_tot, t_blk;
  FSMData(string yaml_file) : machine(yaml_file), program(&machine) {}
};

int main(int argc, const char *argv[]) {
  // Check for CLI arguments
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <machine.yml> <program.gcode>" << endl;
    return 1;
  }

  // Create objects
  FSMData data(argv[1]);
  FiniteStateMachine fsm(&data);

  // Load the G-code
  try {
    data.program.load(argv[2]);
  } catch (exception &e) {
    cerr << fg::red << style::bold << "Error: " << e.what() << style::reset << fg::reset << endl;
    return 2;
  }
  cerr << fg::green << style::bold << "MACHINE:" << style::reset 
       << fg::reset << endl
       << data.machine.desc(true) << endl
       << fg::green << style::bold << "PROGRAM:" << style::reset 
       << fg::reset << endl
       << data.program.desc(true) << endl;

  // Timing: we use the Timer class provided in timer.hpp
  data_t tq = data.machine.tq();
  // duration<double>(0.25) creates a time interval of 0.25 seconds
  Timer timer(duration<double>(tq), duration<double>(tq * 2));
  timer.start();  // Enables the timer

  fsm.set_timing_function([&timer]() {
    timer.wait();  // waits for a timer event every tq s, or maximum 2 * tq
  });

  // More refined:
  fsm.set_timing_function([&timer]() {
    try {
      timer.wait_throw();
    } catch (exception &e) {
      cerr << fg::red << "Realtime not satisfied, exiting" << fg::reset << endl;
      cerr << "Message: " << e.what() << endl;
      cncpp::stop_requested = true;
    }
  });


  // Start FSM execution
#ifdef DEBUG
  // when in debug, print a message every time you enter a state function
  fsm.run();
  // fsm.run([&fsm](FSMData &d){
  //   cerr << "State: " << fsm.state_name() << endl;
  // });
#else
  fsm.run();
#endif

  timer.stop();

  return 0;
}