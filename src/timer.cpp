/*
  _____ _                            _               
 |_   _(_)_ __ ___   ___ _ __    ___| | __ _ ___ ___ 
   | | | | '_ ` _ \ / _ \ '__|  / __| |/ _` / __/ __|
   | | | | | | | | |  __/ |    | (__| | (_| \__ \__ \
   |_| |_|_| |_| |_|\___|_|     \___|_|\__,_|___/___/
                                                     
Signal-based timed loop
*/
#include <chrono>
#include <errno.h>  // for errno
#include <signal.h> // for signal
#include <sstream>
#include <stdexcept> // for runtime_error
#include <string.h>  // for strerror
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

using namespace std;
using namespace chrono;

template <typename DurationType = duration<double>> class Timer {
public:
  template <typename IntervalType, typename MaxWaitType>
  explicit Timer(IntervalType interval, MaxWaitType max_wait) {
    _interval = duration_cast<DurationType>(interval);
    _max_wait = duration_cast<DurationType>(max_wait);
    set();
  }

  void set() {
    time_to_time_struct(_interval, _rep.it_interval);
    time_to_time_struct(_interval, _rep.it_value);
    time_to_time_struct(_max_wait, _rqtp);
  }

  string what() {
    stringstream ss;
    ss << "Interval: " << _rep.it_value.tv_sec + _rep.it_value.tv_usec / 1.0E6
       << endl
       << "Max wait: " << _rqtp.tv_sec + _rqtp.tv_nsec / 1.0E9 << endl;
    return ss.str();
  }

  void start() {
    struct itimerval itimer;
    // First interval:
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 250 * 1000;
    // Subsequent intervals:
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 250 * 1000;

    if (setitimer(ITIMER_REAL, &_rep, NULL) != 0) {
      throw runtime_error(std::strerror(errno));
    }
    signal(SIGALRM, [](int signo) {});
  }

  void stop() {
    struct itimerval timer;
    timerclear(&timer.it_value);
    timerclear(&timer.it_interval);
    setitimer(ITIMER_REAL, &timer, NULL);
    signal(SIGALRM, SIG_DFL);
  }

  DurationType remaining() {
    auto sec = seconds(_rmtp.tv_sec);
    auto nsec = nanoseconds(_rmtp.tv_nsec);
    return duration_cast<DurationType>(sec + nsec);
  }

  DurationType elapsed() {
    return _max_wait - remaining();
  }

  int wait() {
    if (nanosleep(&_rqtp, &_rmtp) == 0) {
      _rmtp.tv_sec = _rmtp.tv_nsec = 0;
      return 0;
    } else {
      return -1;
    }
  }

  void wait_throw() {
    if (wait() == 0) 
      throw runtime_error("Timer: max allocated time exceeded");
  }

  ~Timer() { stop(); };

private:
  DurationType _interval;
  DurationType _max_wait;
  struct itimerval _rep;
  struct timespec _rqtp, _rmtp;

  template <typename T, typename S> static int time_to_time_struct(T d, S &ts) {
    ts.tv_sec = duration_cast<seconds>(d).count();
    if constexpr (is_same<S, struct timeval>::value) {
      ts.tv_usec = duration_cast<microseconds>(d).count() - ts.tv_sec * 1E6;
    } else if constexpr (is_same<S, struct timespec>::value) {
      ts.tv_nsec = duration_cast<nanoseconds>(d).count() - ts.tv_sec * 1E9;
    } else {
      return -1;
    }
    return 0;
  }
};




/*
  _____ _____ ____ _____ 
 |_   _| ____/ ___|_   _|
   | | |  _| \___ \ | |  
   | | | |___ ___) || |  
   |_| |_____|____/ |_|  
                         
*/

#ifdef TIMER_MAIN

#include <iostream>

bool Running = true;

int main(int argc, const char *argv[]) {
  double delay = 0.1;
  if (argc == 2)
    delay = atof(argv[1]);

  signal(SIGINT, [](int signo) { Running = false; });

  duration<double> d(delay);
  duration<double> max_d(delay * 2); // 1 second

  Timer t(d, max_d); // Default template parameter is duration<double> in secs
  // Or:
  // Timer<milliseconds> t(milliseconds(200), milliseconds(1000));

  cout << t.what();
  t.start();

  while (Running) {
    t.wait();
    cout << "Elapsed (s): " << t.elapsed().count() << " s" << endl;
  }

  t.stop();

  cout << "Done." << endl;
  return 0;
}

#endif