#include <vector>
#include <immintrin.h>
#include <random>
#include <iostream>
#include <cstdlib> // Random
#include <algorithm> // std::mins
#include "Eval-perf.h"
#include "exo1.h"



int main(){


    int N;
    size_t n;
    std::cout<<"Donnez la valeur de n : "<<std::endl;
    std::cin >> n;
    // Valeurs initiales 

    std::vector<float> A,v,temp,y;
    size_t k;
    A.resize(n*n,2.);
    v.resize(n,5);
    temp.resize(n,0.);
    y.resize(n,0.);
    

    EvalPerf PE;



    PE.start();

    mat2vec_transp(n,A,v,y,temp,k);
    //mat2vec(n,A,v,y,temp,k);


    PE.stop();

    N = 4*n*n - 2*n;
    std::cout << "nb cycles: " << PE.nb_cycle() << std::endl;
    std::cout << "nb secondes: " << PE.second() << std::endl;
    std::cout << "nb millisecondes: " << PE.millisecond() << std::endl;
    std::cout << "CPI: " << PE.CPI(N) << std::endl;
    std::cout << "IPC: " << PE.IPC(N) << std::endl;
    //std::cout << int32ReduceAddSIMD(vec) << std::endl;
}