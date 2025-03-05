#include <chrono>
#include <sys/time.h>

using namespace std;

class timer {
private:
  chrono::duration<double> _interval, _elapsed;
  chrono::duration<double> _initial_wait;
public:
  timer(chrono::duration<double> interval, chrono::duration<double> initial_wait = chrono::duration<double>(0)) {
    _interval = interval;
    _initial_wait = initial_wait;
    _elapsed = chrono::duration<double>(0);
  }

  void set(chrono::duration<double> interval, chrono::duration<double> initial_wait = chrono::duration<double>(0)) {
    _interval = interval;
    _initial_wait = initial_wait;
    struct itimerval timer;
    timer.it_value.tv_sec = _initial_wait.count();
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = _interval.count();
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);
  }

  ~timer() {
    struct itimerval timer;
    timerclear(&timer.it_value);
    timerclear(&timer.it_interval);
    setitimer(ITIMER_REAL, &timer, NULL);
  };
};