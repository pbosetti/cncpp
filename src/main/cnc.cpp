
#include <thread>
#include <chrono>
#include <filesystem>
#include "fsm.hpp"
#include "../cncpp.hpp"
#include "../timer.hpp"

using namespace std;
using double_d = std::chrono::duration<double>;

struct FsmData {
  string program_file;
  unique_ptr<cncpp::Machine> machine;
  unique_ptr<cncpp::Program> program;
  unique_ptr<Timer<double_d, false>> timer;
  data_t t_tot = 0, t_blk = 0;
};

int main(int argc, char *argv[]) {
  string machine_file = "machine.toml";

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <program.gcode> [machine.toml]" << endl;
    return 1;
  }
  string program_file = argv[1];

  if (argc >= 3) {
    machine_file = argv[2];
  }

  FsmData data{
    .program_file = program_file,
    .machine = make_unique<cncpp::Machine>(machine_file)
  };

  double_d timer_interval(data.machine->tq());
  double_d timer_max_interval(data.machine->tq_max());
  data.timer = make_unique<Timer<double_d, false>>(timer_interval, timer_max_interval);
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
    // put here any operation that must be executed at each loop
  });

  cerr << "Program execution completed." << endl;
}