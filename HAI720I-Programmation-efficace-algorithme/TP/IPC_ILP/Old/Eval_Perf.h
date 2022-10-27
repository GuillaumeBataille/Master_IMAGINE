#include <chrono>
#include <x86intrin.h>
#include <iostream>

//fonction utilisee pour chopper le cycle a un instant t.
uint64_t  rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi<<32) | lo;
}


class Eval_Perf{
    private:
    std::chrono::high_resolution_clock::time_point time_start; // Chrono.now
    std::chrono::high_resolution_clock::time_point time_stop;// Chrono.now
    long int cycle_start;//Fonction du tp
    long int cycle_stop;//Fonction du tp

    public:
    Eval_Perf(){};

    void start(){
        std::cout<<"Debut de start()"<<std::endl;
        time_start = std::chrono::high_resolution_clock::now();
        cycle_start = rdtsc();
        std::cout<<"Fin de start()"<<std::endl;
    };

    void stop(){
        std::cout<<"Debut de stop()"<<std::endl;
        time_stop = std::chrono::high_resolution_clock::now();
        cycle_stop = rdtsc();
        std::cout<<"Fin de stop()"<<std::endl;
    };

    long int cycle(){
        long int res;
        res = cycle_stop - cycle_start;
        return res;
         
    };


    double second(){
        
        std::chrono::duration<float> second = std::chrono::duration< std::chrono::seconds>(time_stop-time_start);
    };

    double millisecond(){
        return std::chrono::duration_cast< std::chrono::seconds>(time_stop-time_start)/1000000;
    };

    double CPI();

    double IPC();


};