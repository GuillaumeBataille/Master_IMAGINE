
#include <chrono>
#include <iostream>
#include <x86intrin.h>

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

class EvalPerf {

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> time_start;
  std::chrono::time_point<std::chrono::high_resolution_clock> time_stop;
  size_t cycle_start = 0;
  size_t cycle_stop = 0;

public:
  void start(){
    time_start = std::chrono::high_resolution_clock::now();
    cycle_start = rdtsc();}

  void stop() {
    time_stop = std::chrono::high_resolution_clock::now();
    cycle_stop = rdtsc();}
  
  uint64_t cycles() { return cycle_stop-cycle_start;}

  auto milliseconds() { 
    return std::chrono::duration<double, std::milli>(time_stop-time_start).count();
    }

  auto seconds() { 
    return std::chrono::duration<double>(time_stop-time_start).count();
    }
  
  double CPI(size_t N)  {
    return double(cycle_stop-cycle_start)/double(N);
    }

  double IPC(long int N) { 
    return N/double(cycle_stop-cycle_start);
    }
  

  double Gflops(long int N) { 
    return double(N)/seconds();
    }
  
};

