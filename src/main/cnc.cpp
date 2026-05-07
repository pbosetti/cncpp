/*
  _____ ____  __  __   __  __       _       
 |  ___/ ___||  \/  | |  \/  | __ _(_)_ __  
 | |_  \___ \| |\/| | | |\/| |/ _` | | '_ \ 
 |  _|  ___) | |  | | | |  | | (_| | | | | |
 |_|   |____/|_|  |_| |_|  |_|\__,_|_|_| |_|
                                            
Warning: this is a bare-bones template for a FSM-based agent. It is not meant to be used as-is, but rather to be adapted to the specific needs of the user. 

To compile it with CMake, use the following CMakeLists.txt:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the installed MADS package
find_package(Mads REQUIRED)
message(STATUS "Found Mads: ${Mads_DIR}")

add_executable(fsm path/to/main.cpp)
target_link_libraries(fsm PRIVATE Mads::Mads)
*/

#include <thread>
#include <chrono>
#include <filesystem>
#include "../fsm.hpp"
#include "../machine.hpp"
#include "../program.hpp"
#include "../timer.hpp"

using namespace std;
using double_d = std::chrono::duration<double>;

struct FsmData {
  string program_file;
  unique_ptr<cncpp::Machine> machine;
  unique_ptr<cncpp::Program> program;
  unique_ptr<Timer<double_d, true>> timer;
  data_t t_tot = 0, t_blk = 0;
};

int main(int argc, char *argv[]) {
  string machine_file = "machine.toml";
  bool non_blocking = false;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <program.gcode> [machine.toml]" << endl;
    return 1;
  }
  string program_file = argv[1];
  if (argc == 3) {
    machine_file = argv[2];
  }
  FsmData data = {
    .program_file = program_file,
    .machine = std::make_unique<cncpp::Machine>(machine_file)
  };

  // Initialize FSM
  double_d timer_interval(data.machine->tq());
  double_d timer_max_wait(data.machine->tq_max());
  data.timer = make_unique<Timer<double_d, true>>(timer_interval, timer_max_wait);
  data.timer->start();
  auto fsm = cncpp::FiniteStateMachine(&data);
  fsm.set_timing_function([&]() {
    try {
      data.timer->wait_throw();
    } catch (const TimerError &e) {
      cerr << "Timer error: " << e.what() << endl;
    }
  });
  fsm.run([&](FsmData &s) {
    // here put everything that shall run at each loop iteration
  });
  cerr << "Program execution completed." << endl;
}