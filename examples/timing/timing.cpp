#include <iostream>
#include <chrono>
#include <thread>
#include <array>
#include <sched.h>
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace std::chrono;
using to_ms = duration<double, ratio<1, 1000>>;

// using a template allows the compiler to optimize the code
// by removing the if statement
template <long period_ms=0, bool alrm=false>
vector<double> free_run(unsigned int n) {
  auto start = high_resolution_clock::now();
  auto now = start;
  static auto last = start;
  vector<double> deltas;
  deltas.reserve(n);
  // don't declare variables in loops!
  duration<long long, std::nano> elapsed = 0s, delta = 0s;

  for (unsigned int i = 0; i < n; i++) {
    // Update times
    now = high_resolution_clock::now();
    elapsed = (now - start);
    deltas.push_back(to_ms(now - last).count());
    last = now;

    cout << i << ": " << to_ms(elapsed).count() 
         << " ms, " << deltas.back() << " ms" << endl;

    // this is a compile-time if statement
    // if false, it won't be compiled
    if constexpr (period_ms > 0) {
      if constexpr (alrm) {
        usleep(period_ms * 1000 * 5);
      } else {
        this_thread::sleep_for(milliseconds(period_ms));
      }
    }
  }
  return deltas;
}

void stats(vector<double> &x) {
  x.erase(x.begin());
  double sum = 0;
  for (auto i : x) {
    sum += i;
  }
  double mean = sum / x.size();
  double sq_sum = 0;
  for (auto i : x) {
    sq_sum += (i - mean) * (i - mean);
  }
  double stdev = sqrt(sq_sum / x.size());
  cout << "Mean: " << mean << " ms" << endl;
  cout << "Stdev: " << stdev << " ms" << endl;
}



int main(int argc, const char *argv[]) {
  unsigned int n = 10;
  if (argc > 1) {
    n = atoi(argv[1]);
  }

  vector<double> deltas;

  // desired interval in milliseconds
  constexpr unsigned int period = 100;

  // set up the recurring timer alarm
  // this raises a SIGALRM signal every period
  struct itimerval itimer;
  // first interval:
  itimer.it_interval.tv_sec = 0;
  itimer.it_interval.tv_usec = period * 1000;
  // subsequent intervals:
  itimer.it_value.tv_sec = 0;
  itimer.it_value.tv_usec = period * 1000;

  // Free run: 100% of one CPU core
  cout << "Free run without period" << endl;
  deltas = free_run(n);
  stats(deltas);

  // pauses for period milliseconds every iteration
  cout << endl << "Free run with period " << period << " ms" << endl;
  deltas = free_run<period>(n);
  stats(deltas);

  // repeats every iteration every period milliseconds 
  cout << endl << "Free run with ALRM " << period << " ms" << endl;
  setitimer(ITIMER_REAL, &itimer, NULL);
  signal(SIGALRM, [](int signo) {});
  deltas = free_run<period, true>(n);
  stats(deltas);

  return 0;
}