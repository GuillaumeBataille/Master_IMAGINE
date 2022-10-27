#include "Eval-perf.h"
#include <chrono>
#include <x86intrin.h>

EvalPerf::EvalPerf(){}

uint64_t EvalPerf::rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void EvalPerf::start(){
    this->t0 = std::chrono::high_resolution_clock::now();
    this->c0 = rdtsc();
}

void EvalPerf::stop(){
    this->t1 = std::chrono::high_resolution_clock::now();
    this->c1 = rdtsc();
}

uint64_t EvalPerf::nb_cycle(){
    return c1-c0;
}

double EvalPerf::second(){
    return std::chrono::duration<double>(t1-t0).count();
}

double EvalPerf::millisecond(){
    return std::chrono::duration<double, std::milli>(t1-t0).count();
}

double EvalPerf::CPI(size_t N){
    return static_cast<double>((c1-c0))/N;
}

double EvalPerf::IPC(size_t N){
    return N/static_cast<double>((c1-c0));
}

double EvalPerf::Gflops(size_t N){
    return static_cast<double>(N)/second();
}