#ifndef EVALPERF_H
#define EVALPERF_H

#include <x86intrin.h>
#include <cstdint>
#include <chrono>
#include <iostream>

class EvalPerf
{
private:
    std::chrono::high_resolution_clock::time_point t0, t1;
    uint64_t c0, c1;

    uint64_t rdtsc();

public:
    EvalPerf();
    void start();
    void stop();
    void print(size_t N);                 //Afficher les données
    void compute(int N, const char *nom); // Sauvergarder la sortie standard dans un fichier
    uint64_t nb_cycle();
    double second();
    double millisecond();
    double CPI(size_t N);
    double IPC(size_t N);
    double Gflops(size_t N);
};

#endif