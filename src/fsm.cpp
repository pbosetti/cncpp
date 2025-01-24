/******************************************************************************
Finite State Machine
Project: src/fsm.dot
Description: C-CNC

Generated by gv_fsm ruby gem, see https://rubygems.org/gems/gv_fsm
gv_fsm version 0.4.0
Generation date: 2024-10-16 12:59:15 +0200
Generated from: src/fsm.dot
The finite state machine has:
  8 states
  7 transition functions

******************************************************************************/

#include <syslog.h>
#include "fsm.hpp"
#include <keystroker.h>
    
using namespace std;
using namespace rang;
    

// SEARCH FOR Your Code Here FOR CODE INSERTION POINTS!


namespace FSM {

/*  ____  _        _       
 * / ___|| |_ __ _| |_ ___ 
 * \___ \| __/ _` | __/ _ \
 *  ___) | || (_| | ||  __/
 * |____/ \__\__,_|\__\___|
 *                         
 *   __                  _   _                 
 *  / _|_   _ _ __   ___| |_(_) ___  _ __  ___ 
 * | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 * |  _| |_| | | | | (__| |_| | (_) | | | \__ \
 * |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
 */                                             

// Function to be executed in state STATE_INIT
// valid return states: STATE_IDLE, STATE_STOP
template<class T> 
state_t do_init(T &data) {
  state_t next_state = FSM::STATE_IDLE;
  data.machine.connect();
  cerr << fg::green << "Connected to machine "
       << style::bold << data.machine.mqtt_host()
       << style::reset << fg::reset << endl;
  return next_state;
}

// Function to be executed in state STATE_IDLE
// valid return states: NO_CHANGE, STATE_IDLE, STATE_LOAD_BLOCK, STATE_STOP, STATE_GO_TO_ZERO
// SIGINT triggers an emergency transition to STATE_STOP
template<class T> 
state_t do_idle(T &data) {
  state_t next_state = FSM::NO_CHANGE;

  cerr << "Press " << fg::green << "<space>" << fg::reset
       << " to run, " << fg::blue << "z" << fg::reset
       << " to go to zero, " << fg ::red << "q" << fg::reset
       << " to quit" << endl;
       
  char key = keystroker::read_key();

  switch(key) {
  case ' ':
    next_state = FSM::STATE_LOAD_BLOCK;
    break;
  case 'q':
  case 'Q':
    next_state = FSM::STATE_STOP;
    break;
  case 'z':
  case 'Z':
    next_state = FSM::STATE_GO_TO_ZERO;
    break;
  default:
    break;
  }
  
  return next_state;
}

// Function to be executed in state STATE_STOP
// valid return states: NO_CHANGE
template<class T> 
state_t do_stop(T &data) {
  state_t next_state = FSM::STATE_STOP;
  data.machine.listen_stop();
  cerr << fg::red << style::bold << "STOP" << fg::reset << style::reset << endl;
  return next_state;
}

// Function to be executed in state STATE_LOAD_BLOCK
// valid return states: STATE_IDLE, STATE_NO_MOTION, STATE_RAPID_MOTION, STATE_INTERP_MOTION
template<class T> 
state_t do_load_block(T &data) {
  state_t next_state = FSM::STATE_RAPID_MOTION;
  /* Your Code Here */
  
  return next_state;
}

// Function to be executed in state STATE_GO_TO_ZERO
// valid return states: NO_CHANGE, STATE_IDLE, STATE_GO_TO_ZERO
// SIGINT triggers an emergency transition to STATE_STOP
template<class T> 
state_t do_go_to_zero(T &data) {
  state_t next_state = FSM::UNIMPLEMENTED;
  /* Your Code Here */
  
  return next_state;
}

// Function to be executed in state STATE_NO_MOTION
// valid return states: STATE_LOAD_BLOCK
template<class T> 
state_t do_no_motion(T &data) {
  state_t next_state = FSM::UNIMPLEMENTED;
  /* Your Code Here */
  
  return next_state;
}

// Function to be executed in state STATE_RAPID_MOTION
// valid return states: NO_CHANGE, STATE_LOAD_BLOCK, STATE_RAPID_MOTION
// SIGINT triggers an emergency transition to STATE_STOP
template<class T> 
state_t do_rapid_motion(T &data) {
  state_t next_state = FSM::NO_CHANGE;
  /* Your Code Here */
  
  return next_state;
}

// Function to be executed in state STATE_INTERP_MOTION
// valid return states: NO_CHANGE, STATE_LOAD_BLOCK, STATE_INTERP_MOTION
// SIGINT triggers an emergency transition to STATE_STOP
template<class T> 
state_t do_interp_motion(T &data) {
  state_t next_state = FSM::UNIMPLEMENTED;
  /* Your Code Here */
  
  return next_state;
}


/*  _____                    _ _   _              
 * |_   _| __ __ _ _ __  ___(_) |_(_) ___  _ __   
 *   | || '__/ _` | '_ \/ __| | __| |/ _ \| '_ \
 *   | || | | (_| | | | \__ \ | |_| | (_) | | | | 
 *   |_||_|  \__,_|_| |_|___/_|\__|_|\___/|_| |_| 
 *                                                
 *   __                  _   _                 
 *  / _|_   _ _ __   ___| |_(_) ___  _ __  ___ 
 * | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 * |  _| |_| | | | | (__| |_| | (_) | | | \__ \
 * |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
 */                                              

// This function is called in 1 transition:
// 1. from idle to load_block
template<class T>
void reset(T &data) {
  /* Your Code Here */
}

// This function is called in 1 transition:
// 1. from idle to go_to_zero
template<class T>
void begin_zero(T &data) {
  /* Your Code Here */
}

// This function is called in 1 transition:
// 1. from load_block to rapid_motion
template<class T>
void begin_rapid(T &data) {
  /* Your Code Here */
}

// This function is called in 1 transition:
// 1. from load_block to interp_motion
template<class T>
void begin_interp(T &data) {
  /* Your Code Here */
}

// This function is called in 1 transition:
// 1. from rapid_motion to load_block
template<class T>
void end_rapid(T &data) {
  /* Your Code Here */
}

// This function is called in 1 transition:
// 1. from interp_motion to load_block
template<class T>
void end_interp(T &data) {
  /* Your Code Here */
}

// This function is called in 1 transition:
// 1. from go_to_zero to idle
template<class T>
void end_zero(T &data) {
  /* Your Code Here */
}


}; // namespace FSM


// Example usage:
#ifdef TEST_MAIN
#include <unistd.h>
#include <thread>

struct Data {
  int count;
};

int main() {
  Data data = {1};
  auto fsm = FSM::FiniteStateMachine(&data);
  fsm.set_timing_function([]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  });
  fsm.run([&](Data &s) {
    std::cout << "State: " << fsm.state() << " data: " << s.count << std::endl;
  });
  return 0;
}
#endif // TEST_MAIN
