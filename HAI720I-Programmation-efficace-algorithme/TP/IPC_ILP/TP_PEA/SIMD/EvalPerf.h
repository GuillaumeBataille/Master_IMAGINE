#ifndef __EVAL_PERF_H
#define __EVAL_PERFR_H
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
  std::chrono::time_point<std::chrono::high_resolution_clock> _t0, _t1;
  uint64_t _c0, _c1;

public:

  void start(){_t0 = std::chrono::high_resolution_clock::now(); _c0=rdtsc();}

  void stop() {_t1 = std::chrono::high_resolution_clock::now(); _c1=rdtsc();}
  

  void clear() {_t0=_t1; _c1=_c0;}

  auto milliseconds() const { return std::chrono::duration<double, std::milli>(_t1 -_t0).count();}

  auto seconds() const { return std::chrono::duration<double>(_t1 -_t0).count();}
  
  uint64_t cycles() const { return _c1-_c0;}
  
  double CPI(size_t N) const { return double(_c1-_c0)/N;}

  double IPC(size_t N) const { return N/double(_c1-_c0);}
  
  double Gflops(size_t N) const { return double(N)/seconds();}
  
};

#endif 
