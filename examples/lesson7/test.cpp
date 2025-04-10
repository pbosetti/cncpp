// How to measure time in C++

#include<iostream>
#include<vector>
#include<chrono> // For timing
#include<thread>
#include<sys/time.h> // For timing
#include<unistd.h>
#include<csignal>
#include<cmath>

using namespace std;
using namespace std::chrono;
using to_ms = duration<double, std::ratio<1, 1000>>;

template <long int period_ms = 0, bool alrm = false>
vector<double> free_run(unsigned int n) {
  auto start = high_resolution_clock::now();
  auto now = start; // temporary
  static auto last = start; // temporary
  vector<double> deltas;
  deltas.reserve(n);
  // Loop to measure
  duration<long long int, std::nano> elapsed = 0s, delta = 0s; // temporary
  for (unsigned int i = 0; i < n; i++) {
    // Update the times
    now = high_resolution_clock::now();
    elapsed = (now - start);
    deltas.push_back(to_ms(now - last).count());
    last = now; // update the temporary

    cout << 
      i << ": " << to_ms(elapsed).count() <<
      "ms, " << deltas.back() << " ms" << endl;

    // Compile-time if statement
    if constexpr (period_ms > 0) {
      if constexpr (alrm) {
        usleep(period_ms * 1000);
      } else {
        this_thread::sleep_for(milliseconds(period_ms));
      }
    }
  }

  deltas.erase(deltas.begin()); // erase the first element of deltas
  return deltas;
}

// Statistics on execution time
void stats(vector<double> const & x) {
  // Mean
  double sum = 0;
  for (double const i : x) {
    sum += i; // sum = sum + i;
  }
  double mean = sum / x.size();
  // Std. deviation
  double sq_sum = 0;
  for (double i : x){
    sq_sum += (i - mean)*(i - mean); // std::pow((i - mean, 2)
  }
  double stddev = sqrt(sq_sum / x.size());

  cout << "Mean : " << std::scientific << mean << endl;
  cout << "StdD : " << std::scientific << stddev << endl;
}


int main()
{
  auto start = high_resolution_clock::now();
  double a, b, c;
  a = 1;
  b = 2;
  c = a++ - ++b;
  auto stop = high_resolution_clock::now();
  auto elapsed_time = to_ms(stop - start).count();
  std::cout << 
    "Start:   " << start.time_since_epoch().count() << endl <<
    "Stop:    " << stop.time_since_epoch().count()  << endl <<
    "Elapsed: " << elapsed_time  << endl;

  // Use free_run
  constexpr unsigned int n = 10;
  vector<double> deltas;

  // No sleep time
  deltas = free_run(n);
  stats(deltas);

  // With sleep time
  constexpr unsigned int period = 1000; // ms
  deltas = free_run<period>(n);
  stats(deltas);

  // Setup the sigalarm
  struct itimerval itimer;
  // First interval
  itimer.it_interval.tv_sec = 0;
  itimer.it_interval.tv_usec = period * 1000;
  // Subsequent intervals
  itimer.it_value.tv_sec = 0;
  itimer.it_value.tv_usec = period * 1000;

  setitimer(ITIMER_REAL, &itimer, NULL);
  signal(SIGALRM, [](int signo) {});
  deltas = free_run<period, true>(n);
  stats(deltas);

  return 0;
}