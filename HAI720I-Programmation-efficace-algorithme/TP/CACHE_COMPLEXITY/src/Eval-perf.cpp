#include "Eval-perf.h"
#include <chrono>
#include <x86intrin.h>
#include <string.h>

EvalPerf::EvalPerf() {}

uint64_t EvalPerf::rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc"
                         : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

void EvalPerf::start()
{
    this->t0 = std::chrono::high_resolution_clock::now();
    this->c0 = rdtsc();
}

void EvalPerf::stop()
{
    this->t1 = std::chrono::high_resolution_clock::now();
    this->c1 = rdtsc();
}

uint64_t EvalPerf::nb_cycle()
{
    return c1 - c0;
}

double EvalPerf::second()
{
    return std::chrono::duration<double>(t1 - t0).count();
}

double EvalPerf::millisecond()
{
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

double EvalPerf::CPI(size_t N)
{
    return static_cast<double>((c1 - c0)) / N;
}

double EvalPerf::IPC(size_t N)
{
    return N / static_cast<double>((c1 - c0));
}

double EvalPerf::Gflops(size_t N)
{
    return static_cast<double>(N) / second();
}

void EvalPerf::print(size_t N)
{
    std::cout << " Nbr de flops choisi :" << N << std::endl;
    std::cout << "nb cycles: " << nb_cycle() << std::endl;
    std::cout << "nb secondes: " << second() << std::endl;
    std::cout << "nb millisecondes: " << millisecond() << std::endl;
    std::cout << "CPI: " << CPI(N) << std::endl;
    std::cout << "IPC: " << IPC(N) << std::endl;
}

void EvalPerf::compute(int N, const char *name)
{
    std::cout << "a" << std::endl;
    std::string s = "Performance/";
    s += name;
    FILE *f = fopen(s.c_str(), "wb");
    if (f == NULL)
    {
        std::cout << " NULL" << std::endl;
    }
    std::cout << "f :" << s << std::endl;
    fputs(name, f);
    std::cout << "a" << std::endl;
    fputs("\n nombres de flops : ", f);
    fputs(std::to_string(N).c_str(), f);
    fputs("\n nbrcycles: ", f);
    fputs(std::to_string(nb_cycle()).c_str(), f);
    fputs("\n nbrsecondes: ", f);
    fputs(std::to_string(second()).c_str(), f);
    fputs("\n nbrmillisecondes: ", f);
    fputs(std::to_string(millisecond()).c_str(), f);
    fputs("\n CPI: ", f);
    fputs(std::to_string(CPI(N)).c_str(), f);
    fputs("\n IPC: ", f);
    fputs(std::to_string(IPC(N)).c_str(), f);
    fclose(f);
}
