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
#include <mads.hpp>
#include <agent.hpp>
#include <agent_app.hpp>
#include <thread>
#include <chrono>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "../fsm.hpp"
#include "../machine.hpp"
#include "../program.hpp"
#include "../timer.hpp"

using namespace chrono_literals;
using json = nlohmann::json;

struct FsmData {
  std::unique_ptr<Mads::Agent> agent;
  std::unique_ptr<cncpp::Machine> machine;
  std::unique_ptr<cncpp::Program> program;
};

int main(int argc, char *argv[]) {
  std::filesystem::path exec = argv[0];
  std::string agent_name = exec.stem().string();
  std::string settings_path = "tcp://localhost:9092";
  std::string machine_file = "machine.toml";
  std::chrono::milliseconds timer_interval = 10ms;
  std::chrono::milliseconds timer_max_wait = 15ms;
  bool non_blocking = false;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <program.gcode>" << std::endl;
    return 1;
  }
  FsmData data = {
    .agent = std::make_unique<Mads::Agent>(agent_name, settings_path),
  };
  data.agent->init();
  data.agent->set_high_watermark(1);
  data.agent->connect();
  data.agent->info();

  auto settings = data.agent->get_settings();
  if (settings.contains("machine")) {
    machine_file = settings["machine"].get<std::string>();
  }
  if (settings.contains("timer_interval_ms")) {
    timer_interval = std::chrono::milliseconds(settings["timer_interval_ms"].get<int>());
  }
  if (settings.contains("timer_max_wait_ms")) {
    timer_max_wait = std::chrono::milliseconds(settings["timer_max_wait_ms"].get<int>());
  }

  data.machine = std::make_unique<cncpp::Machine>(machine_file);

  // Initialize FSM
  Timer<> timer(timer_interval, timer_max_wait);
  timer.start();
  auto fsm = cncpp::FiniteStateMachine(&data);
  fsm.set_timing_function([&]() {
    try {
      timer.wait_throw();
    } catch (const TimerError &e) {
      std::cerr << "Timer error: " << e.what() << std::endl;
    }
  });
  fsm.run([&](FsmData &s) {
    // here put everything that shall run at each loop iteration
    data.agent->receive(non_blocking);
  });

  // Shutdown procedure
  data.agent->disconnect();
}